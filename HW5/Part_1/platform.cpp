#include <SFML/Graphics.hpp>
#include "GameItem.h"
#include "platform.h"

Platform::Platform(sf::Vector2f size, sf::Color color, int posX, int posY) : GameItem(size, posX, posY)
{     
    setFillColor(color);
    setOutlineColor(sf::Color::Black);
    setOutlineThickness(2);
};

void Platform::onEvent(GameEvent *e) {
    if (e->type == "StaticPlatCollision") {
        Character *player = static_cast<Character *>(e->parameters["Character"].m_asGameItem);
        Platform *platform = static_cast<Platform *>(e->parameters["Platform"].m_asGameItem);
        player->setPosition(player->getPosition().x, platform->getPosition().y - player->getGlobalBounds().height);
        player->allowJump();
    }
}