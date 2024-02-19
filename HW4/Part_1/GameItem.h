#pragma once
#include <SFML/Graphics.hpp>
#include "Timeline.h"

class GameItem;

struct variant {
    enum Type{
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_GAMEITEM,
        // TYPE_POSITION
    };
    Type m_Type;
    union {
        int m_asInt;
        float m_asFloat;
        GameItem *m_asGameItem;
        // sf::Vector2f *m_asPosition;
    };
};

class GameEvent {
    public:
    std::string type; 
    std::map<std::string, struct variant> parameters;
    float priority;
    GameEvent(std::string e_type, Timeline *anchor, float e_priority);
    void add_parameter(std::string paramString, struct variant *parameter);
};

class GameItem: public sf::RectangleShape
{
    public:
        GameItem(sf::Vector2f size, int posX, int posY);
        void shiftRight(int shiftAmount);
        void shiftLeft(int shiftAmount);
        virtual void onEvent(GameEvent *e) = 0;
};

