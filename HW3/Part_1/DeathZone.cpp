#include <SFML/Graphics.hpp>
#include "DeathZone.h"
#include "GameItem.h"

bool active;

DeathZone::DeathZone(sf::Vector2f size, int posX, int posY) : GameItem(size, posX, posY)
{
    active = true;
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