#include <condition_variable>
#include <mutex>
#include <thread>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputSubsystem.h"
#include "character.h"
#include "GameItem.h"

Character *inputPlayer = nullptr;
Timeline *mainAnchor = nullptr;
EventManager *e_Manager = nullptr;

InputSubsystem::InputSubsystem(Character *player, Timeline *mainAnchor, EventManager *eManager)
{
    inputPlayer = player;
    mainAnchor = mainAnchor;
    e_Manager = eManager;
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
        GameEvent *e = new GameEvent("PlayerLeft", mainAnchor, 0);
        
        variant *param = new variant;
        param->m_Type = param->TYPE_GAMEITEM;
        param->m_asGameItem = player;
        e->add_parameter("Character", param);

        variant *param2 = new variant;
        param2->m_Type = param2->TYPE_FLOAT;
        param2->m_asFloat = movementSpeed * playerSpeed;
        e->add_parameter("Left", param2);

        e_Manager->raise(e);
    }
    if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        GameEvent *e = new GameEvent("PlayerRight", mainAnchor, 0);
        
        variant *param = new variant;
        param->m_Type = param->TYPE_GAMEITEM;
        param->m_asGameItem = player;
        e->add_parameter("Character", param);

        variant *param2 = new variant;
        param2->m_Type = param2->TYPE_FLOAT;
        param2->m_asFloat = movementSpeed * playerSpeed;
        e->add_parameter("Right", param2);

        e_Manager->raise(e);
    }
    if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        GameEvent *e = new GameEvent("PlayerJump", mainAnchor, 0);
        
        variant *param = new variant;
        param->m_Type = param->TYPE_GAMEITEM;
        param->m_asGameItem = player;
        e->add_parameter("Character", param);
        
        variant *param2 = new variant;
        param2->m_Type = param2->TYPE_FLOAT;
        param2->m_asFloat = 120;
        e->add_parameter("Jump", param2);

        e_Manager->raise(e);
    }
}
