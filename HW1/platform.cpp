#include <SFML/Graphics.hpp>
#include "platform.h"

Platform::Platform(sf::Vector2f size, sf::Color color, int posX, int posY) : sf::RectangleShape(size)
{     
    setFillColor(color);
    setOutlineColor(sf::Color::Black);
    setOutlineThickness(2);
    setPosition(sf::Vector2f(posX, posY));
};
