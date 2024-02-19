#pragma once
#include <SFML/Graphics.hpp>
#include "GameItem.h"
#include "character.h"
#include "EventManager.h"
#include "SpawnPoint.h"

class DeathZone: public GameItem::GameItem
{
    public:
        DeathZone(sf::Vector2f size, int posX, int posY, Timeline *anchor, EventManager *eMan);
        bool isActive();
        void activate();
        void deactivate();
        void onEvent(GameEvent *e);
};