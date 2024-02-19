#include <SFML/Graphics.hpp>
#include <condition_variable>
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
Timeline *platformTimeline;

FloatingPlatform::FloatingPlatform(sf::Vector2f size, sf::Color color, int posX, int posY, int moveDistance, bool vertical, bool horizontal, Timeline *platformTimeline) : Platform(size, color, posX, posY)
{     
    this->platformTimeline = platformTimeline;
    currentX = posX;
    currentY = posY;
    vert = vertical;
    horiz = horizontal;
    lastX = currentX;
    lastY = currentY;

    maxDistX = posX + moveDistance;
    maxDistY = posY + moveDistance;
    
    minDistX = posX - moveDistance;
    minDistY = posY - moveDistance;
};

void FloatingPlatform::move(std::mutex *m, std::condition_variable *cv, int movementSpeed)
{
    std::unique_lock<std::mutex> cv_lock(*m);
    cv->notify_all();

    if (!platformTimeline->isPaused()) {
	    if (horiz) {
            lastX = currentX;

            if (currentUp) {
                if (lastX - movementSpeed > minDistX) {
                    currentX = lastX - movementSpeed;
                } else {
                    currentX = lastX + movementSpeed;
                    currentUp = false;
                }
            } else {
                if (lastX + movementSpeed < maxDistX) {
                    currentX = lastX + movementSpeed;
                } else {
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
                } else {
                    currentY = lastY - movementSpeed;
                    currentRight = false;
                }
            } else {
                if (lastY - movementSpeed > minDistY) {
                    currentY = lastY - movementSpeed;
                } else {
                    currentY = lastY + movementSpeed;
                    currentRight = true;
                }
            }

        }
    } 

    setPosition(sf::Vector2f(currentX, currentY));
};