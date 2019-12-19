#include "Arduino.h"

#define MIN_X_SPACESHIP 1
#define MAX_X_SPACESHIP 6

class Spaceship
{
private:
  int coordX = 1;
  
public:
  Spaceship();
  void moveLeft();
  void moveRight();
  int getCoordX();
};

