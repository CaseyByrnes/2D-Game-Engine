#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <SFML/Graphics.hpp>
#include <zmq.hpp>
#include <string>
#include <unistd.h>
#include <vector>
#include <assert.h>
#include <iostream>
#include "Timeline.h"
#include "character.h"
#include "platform.h"
#include "floatingPlatform.h"
#include "InputSubsystem.h"
#include "GameThread.h"

using namespace sf;

void movePlatformWrapper(GameThread *gt, FloatingPlatform *platform, int movementSpeed)
{
    gt->movePlatform(platform, movementSpeed);
};

void inputWrapper(GameThread *gt, InputSubsystem *inputs, int movementSpeed)
{
    gt->checkInputs(inputs, movementSpeed);
};

void connectClientsWrapper(GameThread *gt, bool *running, ServerStruct *serverInfo, int *connectedClients) {
    gt->connectClients(running, serverInfo, connectedClients);
}

void connectToServerWrapper(GameThread *gt, bool *running, ClientStruct *clientInfo) {
    gt->connectToServer(running, clientInfo);
}


int main()
{   
    std::mutex m;
    std::condition_variable cv;
    int socketPort = 5556;
    bool running = true;
    std::vector<ClientStruct> clientStructArray = {};

    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    // subscriber.setsockopt(ZMQ_CONFLATE, "", 0);

    subscriber.connect("tcp://localhost:5560");

    //Create game window
    sf::RenderWindow gameWindow;
    const int WINDOW_HEIGHT = 600;
    const int WINDOW_WIDTH = 800;
    int TICSIZE = 6;
    gameWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Server", sf::Style::Close);
    gameWindow.setPosition(sf::Vector2i(50, 50));
    gameWindow.setFramerateLimit(60);

    //Create player character
    Character player = Character(30, sf::Color(255, 255, 255));
    sf::Texture texture;
    if (!texture.loadFromFile("image.png")) {
        return 1;
    }
    texture.setSmooth(true);
    player.setTexture(&texture);
    sf::Vector2f startPosition = sf::Vector2f(100.f, 510.f);
    player.setPosition(startPosition);
    
    Timeline mainTimeline(NULL, TICSIZE);
    int64_t last_time = mainTimeline.getTime();
    Timeline platformTimeline(&mainTimeline, TICSIZE);


    ClientStruct clientInfo;
    clientInfo.characterPosition = player.getPosition();
    GameThread serverConnector(0, NULL, &m, &cv);
    std::thread connectionThread(connectToServerWrapper, &serverConnector, &running, &clientInfo);
    
    //Create static and floating platforms
    Platform staticPlatform = Platform(sf::Vector2f(796, 50), sf::Color(102, 51, 0), 2, 548);
    FloatingPlatform floating = FloatingPlatform(sf::Vector2f(100, 25), sf::Color(102, 0, 204), 600, 330, 150, true, false, &platformTimeline);

    bool hasFocus = true;
    
    //Game Loop
    while(running)
    {
        //Get frame delta info
        int64_t current_time = mainTimeline.getTime();
        int64_t frame_delta;
        if (mainTimeline.isPaused()) {
            frame_delta = 0;
        } else {
            frame_delta = current_time - last_time;
        }
        if (frame_delta < 0 || frame_delta > 100){
            frame_delta = 0;
        }
        last_time = current_time;

        //Process events: closing, gain/loss of focus, pause/unpause
        sf::Event event;
        while(gameWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
                break;
            } else if (event.type == sf::Event::GainedFocus) {
                hasFocus = true;
            } else if (event.type == sf::Event::LostFocus) {
                hasFocus = false;
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    if (mainTimeline.isPaused()) {
                        mainTimeline.unpause();
                    } else if (!mainTimeline.isPaused()) {
                        mainTimeline.pause();
                    } 
                }
            }
        }
        //Process if closing via Escape key
        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            gameWindow.close();
            running = false;
        }  


        //Clear the game window and render objects
        gameWindow.clear(sf::Color(117, 206, 239));

        //Receive message from publisher and convert message into ServerStruct
        zmq::message_t buffer;
        subscriber.recv(buffer);
        ServerStruct *serverInfo = static_cast<struct ServerStruct *>(buffer.data());

        //Draw any characters from other clients
        int numPlayers = serverInfo->ids.size();
        for (int i = 0; i < numPlayers; i++) {
            if (clientInfo.id != serverInfo->ids.at(i)) {
                Character otherPlayer = Character(30, sf::Color(255, 255, 255));
                otherPlayer.setPosition(serverInfo->characterPositions.at(i));
                sf::Texture texture;
                if (!texture.loadFromFile("image.png")) {
                        return 1;
                }
                texture.setSmooth(true);
                player.setTexture(&texture);
                gameWindow.draw(otherPlayer);
                std::cout << "Character drawn\n";
            }
        }

        //Set the positions of the platforms from the received ServerStruct
        staticPlatform.setPosition(serverInfo->platformPosition);
        floating.setPosition(serverInfo->floatingPosition);

        //Draw this clients player and the platforms from the server
        gameWindow.draw(player);
        gameWindow.draw(staticPlatform);
        gameWindow.draw(floating);
        gameWindow.display();

    } 

    //Close the subscriber socket and set the running flag to false.
    subscriber.close();
    running = false;

    //Join the connection thread with the main thread.
    connectionThread.join();
    std::cout << "Shutting Down\n";
    return 0;
}