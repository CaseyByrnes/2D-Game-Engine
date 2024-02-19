#pragma once
#include <SFML/Graphics.hpp>

class Platform: public sf::RectangleShape 
{
    public:
        Platform(sf::Vector2f, sf::Color, int posX, int posY);
};