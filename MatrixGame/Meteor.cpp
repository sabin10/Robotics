#include "Meteor.hpp"

Meteor::Meteor() {}

Meteor::Meteor(int coordX, unsigned long intervalMovement) {
  this->coordX = coordX;
  this->intervalMovement = intervalMovement;
}

void Meteor::setIntervalMovement(unsigned long intervalMovement) {
  this->intervalMovement = intervalMovement;
}

bool Meteor::canMove() {
  if (millis() - previousMovementTime > intervalMovement) {
    return true;
  }
  return false;
}

void Meteor::move() {
  coordY--;
  previousMovementTime = millis();
  if (coordY == -1) {
    isDone = true;
  }
}

bool Meteor::canRemove() {
  return isDone;
}

int Meteor::getCoordX() {
  return coordX;
}

int Meteor::getCoordY() {
  return coordY;
}