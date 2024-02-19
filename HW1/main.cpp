#include <iostream>

#include <SFML/Graphics.hpp>
#include "character.h"
#include "platform.h"
#include "floatingPlatform.h"

using namespace sf;

int main()
{
    //Create game window
    sf::RenderWindow gameWindow;
    const int WINDOW_HEIGHT = 600;
    const int WINDOW_WIDTH = 800;
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

    int gravity = 5;

    //Create static and floating platforms
    Platform staticPlatform = Platform(sf::Vector2f(796, 50), sf::Color(102, 51, 0), 2, 548);
    FloatingPlatform floating = FloatingPlatform(sf::Vector2f(100, 25), sf::Color(102, 0, 204), 600, 330, 150, true, false, 2);

    //Game Loop
    while(gameWindow.isOpen())
    {
        sf::Event event;
        while(gameWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                gameWindow.close();
                break;
        }

        //Check if closing
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) ) {
            gameWindow.close();
            break;
        }

        //Check for user input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            player.moveLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            player.moveRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            player.jump(120);
        }

        //Check left side of screen
        if (player.getPosition().x < 0) {
            player.setPosition(2, player.getPosition().y);
        }
        //Check top side of screen
        if (player.getPosition().y < 0) {
            player.setPosition(player.getPosition().x, 2);
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
        player.move(gravity);

        floating.move();

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