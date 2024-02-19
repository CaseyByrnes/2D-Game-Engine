#pragma once
#include <SFML/Graphics.hpp>
#include "GameItem.h"

class SideBoundary: public GameItem::GameItem
{
    public:
        SideBoundary(sf::Vector2f size, int posX, int posY);
        bool isActive();
        void activate();
        void deactivate();
        void onEvent(GameEvent *e);
};
