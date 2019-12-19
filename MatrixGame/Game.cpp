#include "Game.hpp"

Game::Game() {}

Game::Game(int startLevel, LedControl *lc, Spaceship *spaceship) {
  this->startLevel = startLevel;
  this->lc = lc;
  this->spaceship = spaceship;
  initializeScore();
  level = startLevel;
  gameIsOn = true;
  memcpy(currentGrid, emptyGrid, 8);
}

void Game::initializeScore() {
  switch (startLevel)
  {
  case 0:
    score = 0;
    break;
  case 1:
    score = 1;
    break;
  case 2:
    score = 10;
    break;
  case 3:
    score = 15;
    break;
  case 4:
    score = 22;
    break;
  case 5:
    score = 30;
    break;
  case 6:
    score = 35;
    break;
  case 7:
    score = 40;
    break;
  case 8:
    score = 45;
    break;
  default:
    score = 0;
    break;
  }
}

int Game::getScore() {
  return score;
}

int Game::getLevel() {
  return level;
}

int Game::getLives() {
  return lives;
}

bool Game::checkIfGameStarted() {
  return gameIsOn;
}

bool Game::checkIfGameOver() {
  return gameIsOver;
}

void Game::run() {
  moveMeteors();
  createMeteor();
  updateScore();
  updateLevel();
  checkMeteorsHitSpaceship();
  updateSpeedByLevel();

}

void Game::createMeteor() {
  if (millis() - previousCreateMeteorTime < intervalCreateMeteor) {
    return;
  }
  previousCreateMeteorTime = millis();

  Meteor newMeteor = Meteor(random(8), intervalMovementMeteor);
  for (int i = 0; i < MAX_METEORS; i++) {
    if (meteorsTaken[i] == false) {
      meteorsTaken[i] = true;
      meteors[i] = newMeteor;
      break;
    }
  }
}

void Game::moveMeteors() {
  for (int i = 0; i < MAX_METEORS; i++) {
    if (meteorsTaken[i] == false) {
      continue;
    }

    if (!meteors[i].canRemove() && meteors[i].canMove()) {
      meteors[i].move();
    }
  }
}

void Game::checkMeteorsHitSpaceship() {
  for (int i = 0; i < MAX_METEORS; i++) {
    if (meteorsTaken[i] == false) {
      continue;
    }
    if (checkMeteorHitSpaceship(meteors[i])) {
      lives--;
      doOnLifeLost();
      meteors[i] = Meteor();
      meteorsTaken[i] = false;

      if (lives == -1) {
        gameIsOver = true;
      } 
      return;
    }
  }
}

bool Game::checkMeteorHitSpaceship(Meteor meteor) {
  if (meteor.getCoordY() == 1 && meteor.getCoordX() == spaceship->getCoordX()) {
    return true;
  }

  if (meteor.getCoordY() == 0 && meteor.getCoordX() == spaceship->getCoordX() - 1) {
    return true;
  }

  if (meteor.getCoordY() == 0 && meteor.getCoordX() == spaceship->getCoordX() + 1) {
    return true;
  }

  if (meteor.getCoordY() == 0 && meteor.getCoordX() == spaceship->getCoordX()) {
    return true;
  }

  return false;
}

void Game::updateScore() {
  for (int i = 0; i < MAX_METEORS; i++) {
    if (meteorsTaken[i] == true && meteors[i].canRemove()) {
      meteorsTaken[i] = false;
      score++;
    }
  }
}

void Game::updateLevel() {
  if (score > 0 && score < 10) {
    level = 1;
  } else if (score >= 10 && score < 15){
    level = 2;
  } else if (score >= 15 && score < 22) {
    level = 3;
  } else if (score >= 22 && score < 30) {
    level = 4;
  } else if (score >= 30 && score < 35) {
    level = 5;
  } else if (score >= 35 && score < 40) {
    level = 6;
  } else if (score >= 40 && score < 45) {
    level = 7;
  } else if (score >= 45 && score < 50) {
    level = 8;
  } else if (score >= 50 && score < 55) {
    level = 9;
  } else if (score >= 55) {
    level = 10;
  }
}

void Game::updateSpeedByLevel() {
  switch (level)
  {
  case 0:
    intervalCreateMeteor = 3000;
    intervalMovementMeteor = 1000;
    break;
  case 1:
    intervalCreateMeteor = 2500;
    intervalMovementMeteor = 800;
    break;
  case 2:
    intervalCreateMeteor = 2000;
    intervalMovementMeteor = 600;
    break;
  case 3:
    intervalCreateMeteor = 1800;
    intervalMovementMeteor = 500;
    break;
  case 4:
    intervalCreateMeteor = 1600;
    intervalMovementMeteor = 400;
    break;
  case 5:
    intervalCreateMeteor = 1300;
    intervalMovementMeteor = 350;
    break;
  case 6:
    intervalCreateMeteor = 1100;
    intervalMovementMeteor = 300;
    break;
  case 7:
    intervalCreateMeteor = 1000;
    intervalMovementMeteor = 250;
    break;
  case 8:
    intervalCreateMeteor = 1000;
    intervalMovementMeteor = 200;
    break;
  case 9:
    intervalCreateMeteor = 900;
    intervalMovementMeteor = 200;
    break;
  case 10:
    intervalCreateMeteor = 800;
    intervalMovementMeteor = 100;
    break;
  default:
    break;
  }
}

void Game::blinkGrid(int times) {
  for (int i = 0; i < times; i++) {
    for (int row = 0; row < 8; row++) {
      lc->setColumn(0, row, B11111111);
    }
    delay(50);
    
    for (int row = 0; row < 8; row++) {
      lc->setColumn(0, row, B00000000);
    }
    delay(50);
  }
}

void Game::doOnLifeLost() {
  blinkGrid(1);
}

void Game::resetGame() {
  gameIsOver = false;
  gameIsOn = false;
}

void Game::draw() {
  drawGrid(currentGrid);
  drawSpaceship(spaceship->getCoordX());
  drawMeteors();
}

void Game::drawGrid(byte *grid) {
  for (int row = 0; row < 8; row++) {
    lc->setColumn(0, row, grid[row]);
  }
}

void Game::drawSpaceship(int coord) {
  int shift = coord - 1;
  currentGrid[1] = B01000000 >> shift;
  currentGrid[0] = B11100000 >> shift;
}

void Game::drawMeteors() {
  for (int i = 0; i < MAX_METEORS; i++) {
    if (meteorsTaken[i] == false) {
      continue;
    }
    lc->setLed(0, meteors[i].getCoordX(), meteors[i].getCoordY(), true);
  }
}