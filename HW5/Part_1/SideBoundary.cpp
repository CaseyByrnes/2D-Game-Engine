#include <SFML/Graphics.hpp>
#include "SideBoundary.h"

bool sideActive;

SideBoundary::SideBoundary(sf::Vector2f size, int posX, int posY) : GameItem(size, posX, posY)
{
    sideActive = true;
}

bool SideBoundary::isActive()
{
    return sideActive;
}

void SideBoundary::activate()
{
    sideActive = true;
}

void SideBoundary::deactivate()
{
    sideActive = false;
}

void SideBoundary::onEvent(GameEvent *e) {
    
}