#pragma once
#include <condition_variable>
#include <mutex>
#include <thread>
#include <SFML/Graphics.hpp>
#include "character.h"
#include "EventManager.h"

class InputSubsystem
{
    Character *player;
    public:
        InputSubsystem(Character *player, Timeline *mainAnchor, EventManager *eManager);
        void checkForInput(bool hasFocus, std::mutex *m, std::condition_variable *cv, int movementSpeed);
};