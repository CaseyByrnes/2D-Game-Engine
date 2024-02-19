#pragma once
#include <SFML/Graphics.hpp>

class GameItem: public sf::RectangleShape
{
    public:
        GameItem(sf::Vector2f size, int posX, int posY);
        void shiftRight(int shiftAmount);
        void shiftLeft(int shiftAmount);
};
