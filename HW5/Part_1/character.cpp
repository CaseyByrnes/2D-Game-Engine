#include <SFML/Graphics.hpp>
#include <string.h>
#include "character.h"

bool isJumping = false;
bool canJump = false;
int jumpHeight = 0;
int maxJumpHeight = 0;
int currentHeight = 0;
bool isFalling = true;

Character::Character(int size, sf::Color color) : GameItem(sf::Vector2f(size, size), 0, 0)
{     

}

bool Character::characterJumping()
{
   return isJumping;
}

bool Character::checkIsFalling() {
   return isFalling;
}


bool Character::checkCanJump() {
   return canJump;
}

void Character::allowJump() {
   canJump = true;
   isJumping = false;
   isFalling = true;
}

void Character::jump(int height)
{
   if (canJump) {
      isJumping = true;
      isFalling = false;
      canJump = false;
      jumpHeight = height;
      currentHeight = getPosition().y;
      maxJumpHeight = currentHeight - jumpHeight; 
   }
}

void Character::gravity(int movementSpeed) {
   if (isJumping) {
      currentHeight = getPosition().y;
      if (currentHeight <= maxJumpHeight) {
         isFalling = true;
         isJumping = false;
         canJump = false;
      }
      else {
         setPosition(getPosition().x, getPosition().y - (2 * movementSpeed));
      }
   }

   if (isFalling) {
      setPosition(getPosition().x, getPosition().y + (2 * movementSpeed));
   }

}

void Character::moveLeft(int movementSpeed)
{
   setPosition(sf::Vector2f(getPosition().x - movementSpeed, getPosition().y));
}

void Character::moveRight(int movementSpeed)
{
   setPosition(sf::Vector2f(getPosition().x + movementSpeed, getPosition().y));
}

void Character::onEvent(GameEvent *e) {
   // if (e->type == "PlayerMovement") {
   //    std::map<std::string, struct variant>::iterator it;
   //    for (it == e->parameters.begin(); it != e->parameters.end(); ++it) {
   //       std::string type = it->first;
   //       struct variant&data = it->second;

   //       if (type == "Left") {
   //          if (data.m_Type = variant::TYPE_FLOAT) {
   //             Character *player = static_cast<Character *>(e->parameters["Character"].m_asGameItem);
   //             float moveDistance = e->parameters["Left"].m_asFloat;
   //             player->moveLeft(moveDistance);
   //             break;
   //          }
            

   //       } else if (type == "Right") {
   //          if (data.m_Type = variant::TYPE_FLOAT) {
   //             Character *player = static_cast<Character *>(e->parameters["Character"].m_asGameItem);
   //             float moveDistance = e->parameters["Right"].m_asFloat;
   //             player->moveRight(moveDistance);
   //             break;
   //          }
            

   //       }  else if (type == "Jump") {
   //          if (data.m_Type = variant::TYPE_FLOAT) {
   //             Character *player = static_cast<Character *>(e->parameters["Character"].m_asGameItem);
   //             float moveDistance = e->parameters["Jump"].m_asFloat;
   //             player->jump(moveDistance);
   //             break;
   //          }
            

   //       }   
   //    }
   // }





   if (e->type == "PlayerLeft") {
      Character *player = static_cast<Character *>(e->parameters["Character"].m_asGameItem);
      float moveDistance = e->parameters["Left"].m_asFloat;
      player->moveLeft(moveDistance);
      
   } else if (e->type == "PlayerRight") {
      Character *player = static_cast<Character *>(e->parameters["Character"].m_asGameItem);
      float moveDistance = e->parameters["Right"].m_asFloat;
      player->moveRight(moveDistance);
      
   } else if (e->type == "PlayerJump") {
      Character *player = static_cast<Character *>(e->parameters["Character"].m_asGameItem);
      float moveDistance = e->parameters["Jump"].m_asFloat;
      player->jump(moveDistance);
   }
   
   
}