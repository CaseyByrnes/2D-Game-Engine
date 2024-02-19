#pragma once
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

class GameObject
{
    public:
        GameObject();
        void newPlayer(int size, sf::Texture *texture, sf::Color color, sf::Vector2f startPosition);
        Character *getPlayer();
        int newCharacter(int size, sf::Color);
        std::vector<Character*> *getCharacters();
        int newStaticPlatform(sf::Vector2f size, sf::Color color, int posX, int posY);
        std::vector<Platform*> *getStaticPlatforms();
        int newFloatingPlatform(sf::Vector2f size, sf::Color color, int posX, int posY, int moveDistance, bool vertical, bool horizontal, Timeline *anchor);
        std::vector<FloatingPlatform*> *getFloatingPlatforms();
        int newGameThread(int id, GameThread *other, std::mutex *m, std::condition_variable *cv);
        std::vector<GameThread*> *getGameThreads();
        int newTimeline(Timeline *anchor, int64_t tic);
        std::vector<Timeline*> *getTimelines();
        void newInputSubsystem(Character *player);
        InputSubsystem *getInputSubsystem();
        int newDeathZone(sf::Vector2f size, int posX, int posY);
        std::vector<DeathZone*> *getDeathZones();
        int newSideBoundary(sf::Vector2f size, int posX, int posY);
        std::vector<SideBoundary*> *getSideBoundaries();
        int newSpawnPoint(sf::Vector2f size, int posX, int posY);
        std::vector<SpawnPoint*> *getSpawnPoints();
};  