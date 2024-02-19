#include <SFML/Graphics.hpp>
#include "SpawnPoint.h"

int spawnX;
int spawnY;
bool spawnActive;
sf::Vector2f spawnPosition;

SpawnPoint::SpawnPoint(sf::Vector2f size, int posX, int posY) : GameItem(size, posX, posY)
{
    spawnPosition = sf::Vector2f(spawnX, spawnY);
}

void SpawnPoint::shiftRight(int shiftAmount)
{
    spawnX = spawnX + shiftAmount;
    spawnPosition = sf::Vector2f(spawnX, spawnY);
}

void SpawnPoint::shiftLeft(int shiftAmount)
{
    spawnX = spawnX - shiftAmount;
    spawnPosition = sf::Vector2f(spawnX, spawnY);
}

sf::Vector2f SpawnPoint::getPosition()
{
    return spawnPosition;
}

void SpawnPoint::updatePosition(int posX, int posY)
{
    spawnPosition = sf::Vector2f(spawnX, spawnY);
}

bool SpawnPoint::isActive()
{
    return spawnActive;
}

void SpawnPoint::activate()
{
    spawnActive = true;
}

void SpawnPoint::deactivate()
{
    spawnActive = false;
}