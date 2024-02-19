#include <condition_variable>
#include <mutex>
#include <thread>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputSubsystem.h"
#include "character.h"

Character *inputPlayer = nullptr;

InputSubsystem::InputSubsystem(Character *player)
{
    inputPlayer = player;
}

void InputSubsystem::checkForInput(bool hasFocus, std::mutex *m, std::condition_variable *cv, int movementSpeed)
{
    std::unique_lock<std::mutex> cv_lock(*m);
    cv->notify_all();
    int playerSpeed = 1;

    if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ) {
        playerSpeed = 2;
    }
    if (hasFocus && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ) {
        playerSpeed = 1;
    }

    //Check for user input
    if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        inputPlayer->moveLeft(movementSpeed * playerSpeed);
    }
    if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        inputPlayer->moveRight(movementSpeed * playerSpeed);
    }
    if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        inputPlayer->jump(120);
    }
}
