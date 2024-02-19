#pragma once
#include "GameItem.h"
#include <queue>
#include <map>
#include <mutex>
#include "Timeline.h"
#include <utility>
#include "character.h"
#include "SpawnPoint.h"
#include "platform.h"
#include "floatingPlatform.h"
#include "SideBoundary.h"

class GameEventCompare
{
    public:
        bool operator() (std::pair<float, GameEvent *> e1, std::pair<float, GameEvent *> e2) {
            if (e1.first < e2.first) {
                return true;
            } else {
                return false;
            }
        }
};

class EventManager 
{
    public:
        std::priority_queue<std::pair<float, GameEvent *>, std::vector<std::pair<float, GameEvent *>>, GameEventCompare> *priorityQueue;
        EventManager(Timeline *gameTimeline);
        void subscribe(std::string e_name, GameItem *gameItem);
        void unsubscribe(std::string e_name, GameItem *gameItem);
        void raise(GameEvent *e);
        void handle(GameEvent *e);
    private:
        std::vector<GameEvent*> raised_events;
        std::mutex eMutex;
        Timeline *anchorTimeline;
        std::map<std::string, std::vector<GameItem *>> handlers;
};
