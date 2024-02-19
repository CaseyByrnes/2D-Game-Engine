#include <SFML/Graphics.hpp>
#include "character.h"

bool isJumping = false;
bool canJump = false;
int jumpHeight = 0;
int maxJumpHeight = 0;
int currentHeight = 0;
bool isFalling = true;

Character::Character(int size, sf::Color color) : sf::CircleShape(size)
{     
   setOutlineThickness(5.f);
   setFillColor(color);
   setOutlineColor(sf::Color::White);
   setOutlineThickness(2);
};

bool Character::characterJumping()
{
   return isJumping;
};

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
};

void Character::move(int gravity) {
   if (isJumping) {
      currentHeight = getPosition().y;
      if (currentHeight <= maxJumpHeight) {
         isFalling = true;
         isJumping = false;
         canJump = false;
      }
      else {
         setPosition(getPosition().x, getPosition().y - 5);
      }
   }

   if (isFalling) {
      setPosition(getPosition().x, getPosition().y + gravity);
   }

}

void Character::moveLeft()
{
   setPosition(sf::Vector2f(getPosition().x - 5, getPosition().y));
};

void Character::moveRight()
{
   setPosition(sf::Vector2f(getPosition().x + 5, getPosition().y));
};