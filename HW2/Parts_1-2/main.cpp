#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <SFML/Graphics.hpp>
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

int main()
{
    //Create game window
    sf::RenderWindow gameWindow;
    const int WINDOW_HEIGHT = 600;
    const int WINDOW_WIDTH = 800;
    int TICSIZE = 6;
    int playerSpeed = 1;

    gameWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My Game", sf::Style::Close);
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

    //Create static and floating platforms
    Platform staticPlatform = Platform(sf::Vector2f(796, 50), sf::Color(102, 51, 0), 2, 548);
    FloatingPlatform floating = FloatingPlatform(sf::Vector2f(100, 25), sf::Color(102, 0, 204), 600, 330, 150, true, false, &platformTimeline);

    //Create input subsystem
    InputSubsystem inputs(&player);

    bool hasFocus = true;

    //Game Loop
    while(gameWindow.isOpen())
    {
        int64_t current_time = mainTimeline.getTime();
        int64_t frame_delta;
        if (mainTimeline.isPaused()) {
            frame_delta = 0;
        } else {
            frame_delta = current_time - last_time;
        }
        if (frame_delta < 0  || frame_delta > 100){
            frame_delta = 0;
        }
        last_time = current_time;

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


        //Check if closing
        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) ) {
            gameWindow.close();
            break;
        }

        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ) {
            playerSpeed = 2;
        }
        if (hasFocus && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ) {
            playerSpeed = 1;
        }


        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
                mainTimeline.changeTic(12);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
                mainTimeline.changeTic(6);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
                mainTimeline.changeTic(3);
            }
        }

        std::mutex m;
        std::condition_variable cv;

        GameThread t1(0, NULL, &m, &cv);
        GameThread t2(1, &t1, &m, &cv);

        std::thread first(movePlatformWrapper, &t1, &floating, frame_delta/2);
        std::thread second(inputWrapper, &t2, &inputs, frame_delta * playerSpeed);


        //Join threads back together
        first.join();
        second.join();

        //Check left side of screen
        if (player.getPosition().x < 0) {
            player.setPosition(2, player.getPosition().y);
        }

        //Check right side of screen
        if (player.getPosition().x + player.getGlobalBounds().width > WINDOW_WIDTH) {
            player.setPosition(WINDOW_WIDTH - player.getGlobalBounds().width, player.getPosition().y);
        }
        //Check bottom side of screen
        if (player.getPosition().y + player.getGlobalBounds().height > WINDOW_HEIGHT) {
            player.setPosition(player.getPosition().x, WINDOW_HEIGHT - player.getGlobalBounds().height);
        }

        //Apply gravity if needed
        player.move(frame_delta);

        //Check for collision with platforms
        if (player.getPosition().y + player.getGlobalBounds().height >= staticPlatform.getPosition().y) {
            player.setPosition(player.getPosition().x, staticPlatform.getPosition().y - player.getGlobalBounds().height);
            player.allowJump();
        }
        if (player.getGlobalBounds().intersects(floating.getGlobalBounds())) {
            float playerHeight = player.getGlobalBounds().height;

            if (player.getPosition().y + playerHeight < floating.getPosition().y &&
                    player.getPosition().y + playerHeight <= floating.getPosition().y + 1 ||
                    player.getPosition().y + playerHeight >= floating.getPosition().y - 1 ) {
                player.setPosition(player.getPosition().x, floating.getPosition().y - playerHeight);
                player.allowJump();
            }
        }


        //Clear the game window and render objects
        gameWindow.clear(sf::Color(117, 206, 239));
        gameWindow.draw(staticPlatform);
        
        gameWindow.draw(player);
        
        gameWindow.draw(floating);

        gameWindow.display();
    }   

    return 0;
}