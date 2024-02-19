#include <SFML/Graphics.hpp>
#include "DeathZone.h"
#include "GameItem.h"
#include "Timeline.h"

bool active;
Timeline *mainTimeline = nullptr;
EventManager *main_event_manager = nullptr;

DeathZone::DeathZone(sf::Vector2f size, int posX, int posY, Timeline *anchor, EventManager *eMan) : GameItem(size, posX, posY)
{
    active = true;
    mainTimeline = anchor;
    main_event_manager = eMan;
}

bool DeathZone::isActive()
{
    return active;
}

void DeathZone::activate()
{
    active = true;
}

void DeathZone::deactivate()
{
    active = false;
}

void DeathZone::onEvent(GameEvent *e) {
    if (e->type == "PlayerDeath") {
        Character *player = static_cast<Character *>(e->parameters["Character"].m_asGameItem);
        SpawnPoint *sPoint = static_cast<SpawnPoint *>(e->parameters["SpawnPoint"].m_asGameItem);

        GameEvent *e = new GameEvent("PlayerSpawn", mainTimeline, 0);

        variant *param = new variant;
        param->m_Type = param->TYPE_GAMEITEM;
        param->m_asGameItem = player;
        e->add_parameter("Character", param);

        variant *param2 = new variant;
        param2->m_Type = param2->TYPE_GAMEITEM;
        param2->m_asGameItem = sPoint;
        e->add_parameter("SpawnPoint", param2);

        main_event_manager->raise(e);
    }
}