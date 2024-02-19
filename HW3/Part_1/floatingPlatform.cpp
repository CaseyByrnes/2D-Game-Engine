#include <SFML/Graphics.hpp>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include "floatingPlatform.h"
#include "Timeline.h"

int currentX = 0;
int currentY = 0;
bool vert = false;
bool horiz = false;
int lastX = 0;
int lastY = 0;

int maxDistX = 0;
int maxDistY = 0;
int minDistX = 0;
int minDistY = 0;

bool currentUp = true;
bool currentRight = true;
Timeline *platformTimeline = nullptr;

FloatingPlatform::FloatingPlatform(sf::Vector2f size, sf::Color color, int posX, int posY, int moveDistance, bool vertical, bool horizontal, Timeline *platformTimeline) : Platform(size, color, posX, posY)
{     
    this->platformTimeline = platformTimeline;
    vert = vertical;
    horiz = horizontal;

    maxDistX = moveDistance;
    maxDistY = moveDistance;
};

void FloatingPlatform::move(std::mutex *m, std::condition_variable *cv, int movementSpeed)
{    
    if (m != NULL && cv != NULL) {
        std::unique_lock<std::mutex> cv_lock(*m);
        cv->notify_all();
    }    

    if (!platformTimeline->isPaused()) {
	    if (horiz) {
            lastX = currentX;

            if (currentRight) {
                if (lastX + movementSpeed < maxDistX) {
                    currentX = lastX + movementSpeed;
                    setPosition(sf::Vector2f(getPosition().x - movementSpeed, getPosition().y));
                } else {
                    currentX = lastX - movementSpeed;
                    currentRight = false;
                    setPosition(sf::Vector2f(getPosition().x + movementSpeed, getPosition().y));
                }
            } else {
                if (lastX - movementSpeed > minDistX) {
                    currentX = lastX - movementSpeed;
                    setPosition(sf::Vector2f(getPosition().x + movementSpeed, getPosition().y));
                } else {
                    currentX = lastX + movementSpeed;
                    currentRight = true;
                    setPosition(sf::Vector2f(getPosition().x - movementSpeed, getPosition().y));
                }
            }
        }

        if (vert) {
            lastY = currentY;

            if (currentUp) {
                if (lastY + movementSpeed < maxDistY) {
                    currentY = lastY + movementSpeed;
                    setPosition(sf::Vector2f(getPosition().x, getPosition().y - movementSpeed));
                } else {
                    currentY = lastY - movementSpeed;
                    currentUp = false;
                    setPosition(sf::Vector2f(getPosition().x, getPosition().y + movementSpeed));
                }
            } else {
                if (lastY - movementSpeed > minDistY) {
                    currentY = lastY - movementSpeed;
                    setPosition(sf::Vector2f(getPosition().x, getPosition().y + movementSpeed));
                } else {
                    currentY = lastY + movementSpeed;
                    currentUp = true;
                    setPosition(sf::Vector2f(getPosition().x, getPosition().y - movementSpeed));
                }
            }

        }
    } 
    
};

bool FloatingPlatform::isVertical() {
    return vert;
}

bool FloatingPlatform::isHorizontal() {
    return horiz;
}

bool FloatingPlatform::isLeft() {
    return !currentRight;
}

bool FloatingPlatform::isRight() {
    return currentRight;
}

bool FloatingPlatform::isUp() {
    return currentUp;
}

bool FloatingPlatform::isDown() {
    return !currentUp;
}