#include "Arduino.h"

#define MIN_GRID 0
#define MAX_GRID 7

class Meteor
{
private:
  int coordX;
  int coordY = MAX_GRID;
  unsigned long intervalMovement = 1500; // given by level
  unsigned long previousMovementTime = 0;
  bool isDone = false;

public:
  Meteor();
  Meteor(int coordX, unsigned long intervalMovement);
  void setIntervalMovement(unsigned long intervalMovement);
  bool canMove();
  void move();
  bool canRemove();
  int getCoordX();
  int getCoordY();
};
