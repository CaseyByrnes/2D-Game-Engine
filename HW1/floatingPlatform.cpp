#include <SFML/Graphics.hpp>
#include "floatingPlatform.h"


int currentX = 0;
int currentY = 0;
bool vert = false;
bool horiz = false;
int lastX = 0;
int lastY = 0;
int movementSpeed = 0;

int maxDistX = 0;
int maxDistY = 0;
int minDistX = 0;
int minDistY = 0;

bool currentUp = true;
bool currentRight = true;


FloatingPlatform::FloatingPlatform(sf::Vector2f size, sf::Color color, int posX, int posY, int moveDistance, bool vertical, bool horizontal, int speed) : Platform(size, color, posX, posY)
{     
    currentX = posX;
    currentY = posY;
    vert = vertical;
    horiz = horizontal;
    lastX = currentX;
    lastY = currentY;
    movementSpeed = speed;

    maxDistX = posX + moveDistance;
    maxDistY = posY + moveDistance;
    
    minDistX = posX - moveDistance;
    minDistY = posY - moveDistance;
};

void FloatingPlatform::move()
{
    if (horiz) {
        lastX = currentX;

        if (currentUp) {
            if (lastX - movementSpeed > minDistX) {
                currentX = lastX - movementSpeed;
            }
            else {
                currentX = lastX + movementSpeed;
                currentUp = false;
            }
        }
        else {
            if (lastX + movementSpeed < maxDistX) {
                currentX = lastX + movementSpeed;
            }
            else {
                currentX = lastX - movementSpeed;
                currentUp = true;
            }
        }
    }

    if (vert) {
        lastY = currentY;

        if (currentRight) {
            if (lastY + movementSpeed < maxDistY) {
                currentY = lastY + movementSpeed;
            }
            else {
                currentY = lastY - movementSpeed;
                currentRight = false;
            }
        }
        else {
            if (lastY - movementSpeed > minDistY) {
                currentY = lastY - movementSpeed;
            }
            else {
                currentY = lastY + movementSpeed;
                currentRight = true;
            }
        }

    }

    setPosition(sf::Vector2f(currentX, currentY));
};