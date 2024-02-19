#include <SFML/Graphics.hpp>
#include "GameItem.h"
#include "platform.h"

Platform::Platform(sf::Vector2f size, sf::Color color, int posX, int posY) : GameItem(size, posX, posY)
{     
    setFillColor(color);
    setOutlineColor(sf::Color::Black);
    setOutlineThickness(2);
};