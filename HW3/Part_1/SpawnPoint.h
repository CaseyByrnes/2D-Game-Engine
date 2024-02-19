#pragma once
#include <SFML/Graphics.hpp>
#include "GameItem.h"

class SpawnPoint : GameItem::GameItem
{
    public:
        SpawnPoint(sf::Vector2f size, int posX, int posY);
        void updatePosition(int posX, int posY);
        sf::Vector2f getPosition();
        void shiftRight(int shiftAmount);
        void shiftLeft(int shiftAmount);
        bool isActive();
        void activate();
        void deactivate();
};