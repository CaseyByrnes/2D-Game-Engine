#include "EventManager.h"
#include "DeathZone.h"

EventManager::EventManager(Timeline *gameTimeline) {
    anchorTimeline = gameTimeline;
    priorityQueue = new std::priority_queue<std::pair<float, GameEvent *>, std::vector<std::pair<float, GameEvent *>>, GameEventCompare>;
}
    
void EventManager::subscribe(std::string e_name, GameItem *gameItem) {
    handlers[e_name].push_back(gameItem);
}
    
void EventManager::unsubscribe(std::string e_name, GameItem *gameItem) {
    auto start = handlers[e_name].begin();
    auto end = handlers[e_name].end();
    auto toBeRemoved = std::find(start, end, gameItem);
    if (toBeRemoved != end) {
        int idx = std::distance(start, toBeRemoved);
        handlers[e_name].erase(start + idx);
    }
}
 
void EventManager::raise(GameEvent *e) {
    std::unique_lock<std::mutex> cv_lock(eMutex);

    priorityQueue->push(std::make_pair(e->priority, e));   
}

void EventManager::handle(GameEvent *e) {
    // auto currentHandlers = handlers.find(e->type);
    // if (currentHandlers != handlers.end()) {
    //     for (auto& func : currentHandlers->second) {
    //         func->onEvent(e);
    //     }
    // }

    if (e->type == "PlayerMovement") {
        static_cast<Character *>(e->parameters["Character"].m_asGameItem)->onEvent(e);

    } else if (e->type == "PlayerDeath") {
        static_cast<DeathZone *>(e->parameters["Deathzone"].m_asGameItem)->onEvent(e);

    } else if (e->type == "PlayerSpawn") {
        static_cast<SpawnPoint *>(e->parameters["SpawnPoint"].m_asGameItem)->onEvent(e);

    } else if (e->type == "StaticPlatCollision") {
        static_cast<Platform *>(e->parameters["Platform"].m_asGameItem)->onEvent(e);
        
    } else if (e->type == "FloatingPlatCollision") {
        static_cast<FloatingPlatform *>(e->parameters["Platform"].m_asGameItem)->onEvent(e);
        
    } else if (e->type == "Scroll") {
        static_cast<SideBoundary *>(e->parameters["SideBoundary"].m_asGameItem)->onEvent(e);

    } else if (e->type == "PlayerJump") {
        static_cast<Character *>(e->parameters["Character"].m_asGameItem)->onEvent(e);

    } else if (e->type == "PlayerRight") {
        static_cast<Character *>(e->parameters["Character"].m_asGameItem)->onEvent(e);

    } else if (e->type == "PlayerLeft") {
        static_cast<Character *>(e->parameters["Character"].m_asGameItem)->onEvent(e);

    } 
}