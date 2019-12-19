#include "Arduino.h"
#include <LedControl.h>
#include "Spaceship.hpp"
#include "Meteor.hpp"

#define NUM_LIVES 3
#define MAX_METEORS 8
#define MAX_LEVEL 10

class Game 
{
public:

  Game();
  Game(int startLevel, LedControl *lc, Spaceship *spaceship);
  void initializeScore();
  int getScore();
  int getLives();
  int getLevel();
  bool checkIfGameStarted();
  bool checkIfGameOver();

  void run();
  void createMeteor();
  void moveMeteors();
  void checkMeteorsHitSpaceship();
  bool checkMeteorHitSpaceship(Meteor meteor);
  void updateScore();
  void updateLevel();
  void updateSpeedByLevel();
  void blinkGrid(int times);
  void doOnLifeLost();
  
  void resetGame();

  void draw();
  void drawGrid(byte *grid);
  void drawSpaceship(int coord);
  void drawMeteors();

private:
  LedControl *lc;
  Spaceship *spaceship;
  int startLevel = 0;
  int score = 0;
  int lives = NUM_LIVES;
  int level = 0;
  bool gameIsOn = false;
  bool gameIsOver = false;

  // Meteors
  Meteor meteors[MAX_METEORS] = {};
  bool meteorsTaken[MAX_METEORS] = {};
  unsigned long intervalMovementMeteor = 2000;
  unsigned long intervalCreateMeteor = 4000; // given by level
  unsigned long previousCreateMeteorTime = 0;

  // Grid
  byte emptyGrid[8] = {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
  };
  byte currentGrid[8] = {};
};