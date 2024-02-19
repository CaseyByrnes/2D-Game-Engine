#pragma once
#include <SFML/Graphics.hpp>
#include "floatingPlatform.h"
#include "platform.h"
#include "Timeline.h"

class FloatingPlatform: public Platform::Platform
{
    
    public:
        FloatingPlatform(sf::Vector2f, sf::Color, int posX, int posY, int moveDistance, bool vertical, bool horizontal, Timeline *anchor);
        void move(std::mutex *m, std::condition_variable *cv, int movementSpeed);

    private:
        int currentX;
        int currentY;
        bool vert;
        bool horiz;
        int lastX;
        int lastY;
        int maxDistX;
        int maxDistY;
        int minDistX;
        int minDistY;
        bool currentUp;
        bool currentRight;
        Timeline *platformTimeline;
};