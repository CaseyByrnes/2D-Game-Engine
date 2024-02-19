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
    bool running = true;
    int connectedClients = 0;

    //Create publisher socket
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5560");

    //Create game window
    sf::RenderWindow gameWindow;
    const int WINDOW_HEIGHT = 600;
    const int WINDOW_WIDTH = 800;
    int TICSIZE = 6;
    gameWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Server", sf::Style::Close);
    gameWindow.setPosition(sf::Vector2i(50, 50));
    gameWindow.setFramerateLimit(60);
    
    //Create main and platform timelines
    Timeline mainTimeline(NULL, TICSIZE);
    int64_t last_time = mainTimeline.getTime();
    Timeline platformTimeline(&mainTimeline, TICSIZE);

    //Create static and floating platforms
    Platform staticPlatform = Platform(sf::Vector2f(796, 50), sf::Color(102, 51, 0), 2, 548);
    FloatingPlatform floating = FloatingPlatform(sf::Vector2f(100, 25), sf::Color(102, 0, 204), 600, 330, 150, true, false, &platformTimeline);

    //Create ServerStruct and assign start locations for platforms
    ServerStruct serverInfo;
    serverInfo.floatingPosition = floating.getPosition();
    serverInfo.platformPosition = staticPlatform.getPosition();

    GameThread serverConnector(0, NULL, &m, &cv);
    std::thread connectionThread(connectClientsWrapper, &serverConnector, &running, &serverInfo, &connectedClients);
    
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

        //Check events: closing, gain/loss of focus, pause/unpause
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
        //Check if closing via Escape key
        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            gameWindow.close();
            running = false;
        }

        //Check if pressing T + 1/2/3 to change game speed to slow/default/fast
        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
                mainTimeline.changeTic(TICSIZE * 2);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
                mainTimeline.changeTic(TICSIZE);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
                mainTimeline.changeTic(TICSIZE / 2);
            }
        }
        
        //Use a separate thread to move the floating platform 
        std::mutex m2;
        std::condition_variable cv2;
        GameThread t1(0, NULL, &m2, &cv2);
        std::thread first(movePlatformWrapper, &t1, &floating, frame_delta/2);

        //Join the thread when it is finished
        first.join();

        //Make sure the struct has the most current positions for the platforms
        serverInfo.floatingPosition = floating.getPosition();
        serverInfo.platformPosition = staticPlatform.getPosition();

        //Send the ServerStruct as the data of a message
        zmq::message_t buffer;
        memcpy(buffer.data(), &serverInfo, sizeof(ServerStruct));
        publisher.send(buffer);

        //Clear the game window and render objects
        gameWindow.clear(sf::Color(117, 206, 239));

        //Draw this clients player and the platforms from the server
        gameWindow.draw(staticPlatform);
        gameWindow.draw(floating);
        gameWindow.display();

    } 

    //Close the publisher socket and set the running flag to false
    publisher.close();
    running = false;

    //Join the connection thread with the main thread.
    connectionThread.join();
    std::cout << "Shutting Down\n";
    return 0;
}