#include "Menu.hpp"

// Joystick
const int pinSW = A2; 
const int pinX = A1;
const int pinY = A0; 
int switchValue;
int xValJoystick = 0;
int yValJoystick = 0;
const unsigned int minThreshold = 400;
const unsigned int maxThreshold = 800;

unsigned long previousMillisJoystick = 0;
unsigned long intervalJoystick = 200;

// LCD
const int RS = 7;
const int enable = 6;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// Matrix
LedControl lc = LedControl(12, 11, 10, 1);

// Game
Game game;
Spaceship spaceship;

// Menu
Menu menu = Menu(&lcd, &game);

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  lcd.begin(16, 2);
  // Serial.begin(9600);

  pinMode(pinSW, INPUT_PULLUP);
}

void loop() {
  
  if (game.checkIfGameStarted()) {
    game.draw();

    if (!game.checkIfGameOver()) {
      game.run();
    }
  }

  menu.display();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisJoystick >= intervalJoystick) {
    previousMillisJoystick = currentMillis;

    switchValue = digitalRead(pinSW);
    xValJoystick = analogRead(pinX);
    yValJoystick = analogRead(pinY);

    if (xValJoystick >= maxThreshold) {
      // when x axis ++

      if (game.checkIfGameStarted() && !game.checkIfGameOver()) {
        spaceship.moveRight();
      } else {
        menu.handleXMovement(true);
      }
    }

    if (xValJoystick <= minThreshold) {
      // when x axis --

      if (game.checkIfGameStarted() && !game.checkIfGameOver()) {
        spaceship.moveLeft();
      } else {
        menu.handleXMovement(false);
      }
    }

    if (yValJoystick >= maxThreshold) {
      // when y axis ++
      menu.handleYMovement(true);
    }

    if (yValJoystick <= minThreshold) {
      // when y axis --
      menu.handleYMovement(false);
    }

    if (switchValue == 0) {
      // when joystick buttn is pressed
      if (menu.checkIfGameShouldStart()) {
        // Start Game
        game = Game(menu.getLevelStart(), &lc, &spaceship);
      } 
        menu.handleOnBtnPressed();
    }
  }
}