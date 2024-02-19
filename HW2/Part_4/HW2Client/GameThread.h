#pragma once
#include <condition_variable>
#include <mutex>
#include <vector>
#include <thread>
#include "character.h"
#include "floatingPlatform.h"
#include "InputSubsystem.h"

struct ClientStruct {
  int id = 0;
  sf::Vector2f characterPosition;
  bool paused = false;
};

struct ServerStruct {
    sf::Vector2f platformPosition;
    sf::Vector2f floatingPosition;
    std::vector<sf::Vector2f> characterPositions = {};
    std::vector<int> ids = {};
    bool paused = false;
};

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
        void connectClients(bool *running, ServerStruct *serverInfo, int *connectedClients);
        void connectToServer(bool *running, ClientStruct *clientInfo);
};