#pragma once
#include <condition_variable>
#include <mutex>
#include <thread>
#include "character.h"
#include "floatingPlatform.h"
#include "InputSubsystem.h"

class GameThread
{
    bool busy;
    int id;
    GameThread *other;
    std::mutex *m;
    std::condition_variable *cv;

    public:
        GameThread(int id, GameThread *other, std::mutex *m, std::condition_variable *cv);

        bool isBusy();
        void movePlatform(FloatingPlatform *platform, int movementSpeed);
        void checkInputs(InputSubsystem *inputs, int movementSpeed);
};