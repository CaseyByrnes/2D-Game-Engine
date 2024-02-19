#include <condition_variable>
#include <vector>
#include <iostream>
#include <mutex>
#include <thread>
#include <SFML/Graphics.hpp>
#include "character.h"
#include "GameThread.h"
#include "DeathZone.h"
#include "floatingPlatform.h"
#include "platform.h"
#include "Timeline.h"
#include "InputSubsystem.h"
#include "SpawnPoint.h"
#include "SideBoundary.h"
#include "GameObject.h"

std::vector<Character*> otherCharacters;
std::vector<FloatingPlatform*> floatingPlatforms;
std::vector<Platform*> staticPlatforms;
std::vector<GameThread*> gameThreads;
std::vector<Timeline*> timelines;
std::vector<DeathZone*> deathZones;
std::vector<SideBoundary*> sideBoundaries;
std::vector<SpawnPoint*> spawnPoints;
InputSubsystem *inputs = nullptr;
Character *player = nullptr;

GameObject::GameObject()
{
    otherCharacters = {};
    floatingPlatforms = {};
    staticPlatforms = {};
    gameThreads = {};
    timelines = {};
    deathZones = {};
    sideBoundaries = {};
    spawnPoints = {};
}

void GameObject::newPlayer(int sizeX, int sizeY, sf::Texture *texture, sf::Color color, sf::Vector2f startPosition)
{

    Character *temp = new Character(sizeX, sizeY, color);
    temp->setTexture(texture);
    temp->setPosition(startPosition);
    player = temp;
}

Character *GameObject::getPlayer()
{
    return player;
}

int GameObject::newCharacter(int sizeX, int sizeY, sf::Color color)
{
    Character *temp = new Character(Character(sizeX, sizeY, color));
    otherCharacters.push_back(temp);

    return otherCharacters.size() - 1;
}

std::vector<Character*> *GameObject::getCharacters()
{
    return &otherCharacters;
}

int GameObject::newStaticPlatform(sf::Vector2f size, sf::Color color, int posX, int posY)
{
    Platform *temp = new Platform(size, color, posX, posY);
    staticPlatforms.push_back(temp);
    return staticPlatforms.size() - 1;
}

std::vector<Platform*> *GameObject::getStaticPlatforms()
{
    return &staticPlatforms;
}

int GameObject::newFloatingPlatform(sf::Vector2f size, sf::Color color, int posX, int posY, int moveDistance, bool vertical, bool horizontal, Timeline *anchor)
{
    FloatingPlatform *temp = new FloatingPlatform(size, color, posX, posY, moveDistance, vertical, horizontal, anchor);
    floatingPlatforms.push_back(temp);
    return floatingPlatforms.size() - 1;
}

std::vector<FloatingPlatform*> *GameObject::getFloatingPlatforms()
{
    return &floatingPlatforms;
}

int GameObject::newGameThread(int id, GameThread *other, std::mutex *m, std::condition_variable *cv)
{
    int newID = id;
    if (newID < 0) {
        newID = gameThreads.size();
    }
    GameThread *temp = new GameThread(newID, other, m, cv);
    gameThreads.push_back(temp);
    return gameThreads.size() - 1;
}

std::vector<GameThread*> *GameObject::getGameThreads()
{
    return &gameThreads;
}

int GameObject::newTimeline(Timeline *anchor, int64_t tic)
{
    Timeline *temp = new Timeline(anchor, tic);
    timelines.push_back(temp);
    return timelines.size() - 1;
}

std::vector<Timeline*> *GameObject::getTimelines()
{
    return &timelines;
}

void GameObject::newInputSubsystem(Character *player, Timeline *mainAnchor, EventManager *eManager)
{
    inputs = new InputSubsystem(player, mainAnchor, eManager);
}

InputSubsystem *GameObject::getInputSubsystem()
{
    return inputs;
}

int GameObject::newDeathZone(sf::Vector2f size, int posX, int posY, Timeline *anchor, EventManager *eMan)
{
    DeathZone *temp = new DeathZone(size, posX, posY, anchor, eMan);
    deathZones.push_back(temp);
    return deathZones.size() - 1;
}

std::vector<DeathZone*> *GameObject::getDeathZones()
{
    return &deathZones;
}

int GameObject::newSideBoundary(sf::Vector2f size, int posX, int posY)
{
    SideBoundary *temp = new SideBoundary(size, posX, posY);
    sideBoundaries.push_back(temp);
    return sideBoundaries.size() - 1;
}

std::vector<SideBoundary*> *GameObject::getSideBoundaries()
{
    return &sideBoundaries;
}

int GameObject::newSpawnPoint(sf::Vector2f size, int posX, int posY)
{
    SpawnPoint *temp = new SpawnPoint(size, posX, posY);
    spawnPoints.push_back(temp);
    return spawnPoints.size() - 1;
}

std::vector<SpawnPoint*> *GameObject::getSpawnPoints()
{
    return &spawnPoints;
}