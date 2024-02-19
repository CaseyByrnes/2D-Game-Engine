#pragma once
#include <SFML/Graphics.hpp>
#include "GameItem.h"

class DeathZone: public GameItem::GameItem
{
    public:
        DeathZone(sf::Vector2f size, int posX, int posY);
        bool isActive();
        void activate();
        void deactivate();
};