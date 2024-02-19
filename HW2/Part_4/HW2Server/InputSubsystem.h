#pragma once
#include <condition_variable>
#include <mutex>
#include <thread>
#include <SFML/Graphics.hpp>
#include "character.h"

class InputSubsystem
{
    Character *player;
    public:
        InputSubsystem(Character *player);
        void checkForInput(std::mutex *m, std::condition_variable *cv, int movementSpeed);
};