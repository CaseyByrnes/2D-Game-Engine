#pragma once
#include <SFML/Graphics.hpp>
#include "GameItem.h"

class Character: public GameItem
{
    public:
        Character(int sizeX, int sizeY, sf::Color);
        void jump(int height);
        void moveLeft(int movementSpeed);
        void moveRight(int movementSpeed);
        void moveUp(int movementSpeed);
        void moveDown(int movementSpeed);

        bool characterJumping();
        void allowJump();
        bool checkCanJump();
        bool checkIsFalling();
        void gravity(int gravity);
        void onEvent(GameEvent *e);

    private:
        bool isJumping;
        bool canJump;
        int currentHeight;
        int jumpHeight;
        int maxJumpHeight;
        bool isFalling;

};