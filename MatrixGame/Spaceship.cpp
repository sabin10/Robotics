#include "Spaceship.hpp"

Spaceship::Spaceship(){}

void Spaceship::moveLeft() {
  coordX = coordX > MIN_X_SPACESHIP ? coordX - 1 : coordX;
}

void Spaceship::moveRight() {
  coordX = coordX < MAX_X_SPACESHIP ? coordX + 1 : coordX;
}

int Spaceship::getCoordX() {
  return coordX;
}