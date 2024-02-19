#pragma once
#include <SFML/Graphics.hpp>
#include <string.h>
#include "GameItem.h"
#include "character.h"

class SpawnPoint : public GameItem
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
        void onEvent(GameEvent *e);
};