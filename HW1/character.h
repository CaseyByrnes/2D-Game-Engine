#pragma once
#include <SFML/Graphics.hpp>

class Character: public sf::CircleShape
{
    public:
        Character(int size, sf::Color);
        void jump(int height);
        void moveLeft();
        void moveRight();
        bool characterJumping();
        void allowJump();
        bool checkCanJump();
        bool checkIsFalling();
        void move(int gravity);  

    private:
        bool isJumping;
        bool canJump;
        int currentHeight;
        int jumpHeight;
        int maxJumpHeight;
        bool isFalling;

};