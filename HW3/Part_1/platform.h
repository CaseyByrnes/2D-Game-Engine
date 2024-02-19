#pragma once
#include "GameItem.h"

class Platform: public GameItem 
{
    public:
        Platform(sf::Vector2f, sf::Color, int posX, int posY);
};