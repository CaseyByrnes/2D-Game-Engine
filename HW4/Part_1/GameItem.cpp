#include <SFML/Graphics.hpp>
#include "GameItem.h"

GameItem::GameItem(sf::Vector2f size, int posX, int posY) {
    if (size.x > 0 && size.y > 0) {
        this->setSize(size);
    }
    setPosition(sf::Vector2f(posX, posY));
}

void GameItem::shiftRight(int shiftAmount) {
    setPosition(sf::Vector2f(this->getPosition().x - shiftAmount, this->getPosition().y));
};

void GameItem::shiftLeft(int shiftAmount) {
    setPosition(sf::Vector2f(this->getPosition().x + shiftAmount, this->getPosition().y));
};

GameEvent::GameEvent(std::string e_type, Timeline *anchor, float e_priority) {
    type = e_type;
    priority = anchor->getTime() + e_priority;
}

void GameEvent::add_parameter(std::string paramString, struct variant *parameter) {
    parameters.insert(std::make_pair(paramString, *parameter));
}

