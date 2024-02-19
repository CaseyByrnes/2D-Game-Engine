#include <condition_variable>
#include <mutex>
#include <thread>
#include <SFML/Graphics.hpp>
#include "InputSubsystem.h"
#include "character.h"

Character *player;

InputSubsystem::InputSubsystem(Character *player)
{
    this->player = player;
}

void InputSubsystem::checkForInput(std::mutex *m, std::condition_variable *cv, int movementSpeed)
{
    std::unique_lock<std::mutex> cv_lock(*m);
    cv->notify_all();
    //Check for user input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        player->moveLeft(movementSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        player->moveRight(movementSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        player->jump(120);
    }
}
