#include <condition_variable>
#include <mutex>
#include <thread>
#include "GameThread.h"
#include "character.h"
#include "floatingPlatform.h"
#include "InputSubsystem.h"

bool busy;
int id;
GameThread *other;
std::mutex *m;
std::condition_variable *cv;

GameThread::GameThread(int id, GameThread *other, std::mutex *m, std::condition_variable *cv)
{
    this->id = id;
    if(id == 0) { busy = true; };
    this->m = m;
    this->cv = cv;
}

bool GameThread::isBusy()
{
    std::lock_guard<std::mutex> lock(*m);
    return busy;
}

void GameThread::movePlatform(FloatingPlatform *platform, int movementSpeed)
{
    platform->move(m, cv, movementSpeed);
}

void GameThread::checkInputs(InputSubsystem *inputs, int movementSpeed)
{
    inputs->checkForInput(m, cv, movementSpeed);
}
