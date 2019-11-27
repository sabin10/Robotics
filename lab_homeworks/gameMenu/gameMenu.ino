#include <LiquidCrystal.h>
#include <EEPROM.h>


// Joystick

const int pinSW = A3; // digital pin connected to switch output
const int pinX = A1; // A1 - analog pin connected to X output
const int pinY = A0; // A0 - analog pin connected to Y output
int switchValue;
int xValue = 0;
int yValue = 0;
const unsigned int minThreshold = 400;
const unsigned int maxThreshold = 800;

unsigned long previousMillisJoystick = 0;
unsigned long intervalJoystick = 200;


// LCD

const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// Menu

enum menu {
  main,
  play,
  highscore,
  settings,
  gameOver
};
menu currentMenuDisplay = main;


// Main Menu

// value in range 0,2 which indicates where cursor will be in main menu
unsigned int mainMenuCurrentCursor = 0;
unsigned long currentMillis = 0;

void printMainMenu() {
  lcd.setCursor(1, 0);
  lcd.print("Play");
  lcd.setCursor(7, 0);
  lcd.print("Highscore");
  lcd.setCursor(4, 1);
  lcd.print("Settings");
}

void printCursorMainMenu() {
  if (mainMenuCurrentCursor == 0) {
    lcd.setCursor(0,0);
  } else if (mainMenuCurrentCursor == 1) {
    lcd.setCursor(6,0);
  } else if (mainMenuCurrentCursor == 2) {
    lcd.setCursor(3,1);
  } else {
    lcd.setCursor(0,0);
  }
  lcd.print(">");
}

void clearCursorMainMenu() {
  if (mainMenuCurrentCursor == 0) {
    lcd.setCursor(0,0);
  } else if (mainMenuCurrentCursor == 1) {
    lcd.setCursor(6,0);
  } else if (mainMenuCurrentCursor == 2) {
    lcd.setCursor(3,1);
  } else {
    lcd.setCursor(0,0);
  }
  lcd.print(" ");
}


// Play Menu

unsigned int livesValue = 3;
unsigned int startingLevelValue = 0;
unsigned int currentLevelValue = startingLevelValue;
unsigned int currentScoreValue = 0;

// make sure to reinitialize with 0 everytime leave play menu
unsigned long currentMillisPlay = 0;

void printPlayMenu() {
  // Lives
  lcd.setCursor(0, 0);
  lcd.print("Lives: ");
  lcd.setCursor(7, 0);
  lcd.print(livesValue);

  // Level
  lcd.setCursor(9, 0);
  lcd.print("Lvl: ");
  lcd.setCursor(14, 0);
  lcd.print(currentLevelValue);

  // Score
  lcd.setCursor(4, 1);
  lcd.print("Score: ");
  currentScoreValue = currentLevelValue * 3;
  int startPointScore = currentScoreValue < 10 ? 11 : 10;
  lcd.setCursor(startPointScore, 1);
  lcd.print(currentScoreValue);
}

unsigned long previousMillisLevel = 0;
unsigned long intervalLevel = 2000;
unsigned long previousMillisGameOver = 0;


// Game Over Menu

void printGameOverMenu() {
  lcd.setCursor(0, 0);
  lcd.print("Congratulations!");
  lcd.setCursor(0, 1);
  lcd.print("Press bt to exit");
}


// Highscore Menu

int highscoreValue = 0;

void printHighscoreMenu() {
  lcd.setCursor(0, 0);
  lcd.print("Highscore: ");
  lcd.setCursor(11, 0);
  lcd.print(highscoreValue);
  lcd.setCursor(0, 1);
  lcd.print("Press bt to exit");
}


// Settings Menu

unsigned int currentSettingsCursor = 0; // in range 0-1
bool didPressOnSettings = false; // true when something in settings was selected

void printSettingsMenu() {
  lcd.setCursor(1, 0);
  lcd.print("Name: SABIN");
  lcd.setCursor(1, 1);
  lcd.print("Start Level: ");
  lcd.setCursor(14, 1);
  lcd.print(startingLevelValue);
}

void printCursorSettings() {
  if (currentSettingsCursor == 0) {
    lcd.setCursor(0, 0);
  } else {
    lcd.setCursor(0, 1);
  }

  if (!didPressOnSettings) {
    lcd.print(">");
  } else {
    lcd.print("<");
  }
}

void clearCursorSettings() {
  if (currentSettingsCursor == 0) {
    lcd.setCursor(0, 0);
  } else {
    lcd.setCursor(0, 1);
  }
  lcd.print(" ");
}


// Change screens

void toMainMenu() {
  lcd.clear();
  currentMenuDisplay = main;
}

void toPlayMenu() {
  lcd.clear();
  currentMenuDisplay = play;
  
  previousMillisLevel = millis();
  previousMillisGameOver = millis();
  currentLevelValue = startingLevelValue;
  currentScoreValue = 0;
}

void toGameOverMenu() {
  // update highscore
  if (currentScoreValue > highscoreValue) {
    highscoreValue = currentScoreValue;
    writeHighscoreMemory();
  }
  
  lcd.clear();
  currentMenuDisplay = gameOver;
}

void toHighscoreMenu() {
  lcd.clear();
  currentMenuDisplay = highscore;
}

void toSettingsMenu() {
  lcd.clear();
  currentMenuDisplay = settings;
}


// EEPROM

const unsigned int highscoreAddress = 0;

void initializeHighscoreValue() {
  int highscoreMemory = EEPROM.read(highscoreAddress);
  if (highscoreMemory == 255) {
    highscoreValue = 0;
  } else {
    highscoreValue = highscoreMemory;
  }
}

void writeHighscoreMemory() {
  int highscoreMemory = EEPROM.read(highscoreAddress);
  if (highscoreMemory == 255) {
    EEPROM.write(highscoreAddress, highscoreValue);
  } else {
    EEPROM.put(highscoreAddress, highscoreValue);
  }
}


// Lifecycle

void setup() {
  pinMode(pinSW, INPUT_PULLUP); //activate pull-up resistor on the
  lcd.begin(16, 2);
  Serial.begin(9600);
  initializeHighscoreValue();
}


void loop() {
  currentMillis = millis();

  if (currentMenuDisplay == main) {
    // Main Menu
    printMainMenu();
    printCursorMainMenu();
  } else if (currentMenuDisplay == play) {
    // Play Menu
    printPlayMenu();
    
    if (currentMillis - previousMillisLevel >= intervalLevel) {
      currentLevelValue += 1;
      previousMillisLevel = currentMillis;
    }

    if (currentLevelValue >= startingLevelValue + 5) {
      toGameOverMenu();
    }
  } else if (currentMenuDisplay == gameOver) {
    // Game Over Menu
    printGameOverMenu();
  } else if (currentMenuDisplay == highscore) {
    // Highscore Menu
    printHighscoreMenu();
  } else if (currentMenuDisplay == settings) {
    printSettingsMenu();
    printCursorSettings();
  }
  
  // Handle Joystick moves
  if (currentMillis - previousMillisJoystick >= intervalJoystick) {
    previousMillisJoystick = currentMillis;
    
    switchValue = digitalRead(pinSW);
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);

    if (xValue > maxThreshold) {
      if (currentMenuDisplay == main) {
        clearCursorMainMenu();
        mainMenuCurrentCursor = mainMenuCurrentCursor == 2 ? 2 : mainMenuCurrentCursor + 1;
      } else if (currentMenuDisplay == settings && !didPressOnSettings) {
        clearCursorSettings();
        currentSettingsCursor = currentSettingsCursor == 1 ? 1 : currentSettingsCursor + 1;
      }
    }

    if (xValue < minThreshold) {
      if (currentMenuDisplay == main) {
        clearCursorMainMenu();
        mainMenuCurrentCursor = mainMenuCurrentCursor == 0 ? 0 : mainMenuCurrentCursor - 1;
      } else if (currentMenuDisplay == settings && !didPressOnSettings) {
        clearCursorSettings();
        if (currentSettingsCursor == 0) {
          toMainMenu();
        } else {
          currentSettingsCursor -= 1;
        }
      }
    }

    if (yValue > maxThreshold) {
      if (currentMenuDisplay == settings) {
        if (currentSettingsCursor == 1) {
          startingLevelValue = startingLevelValue == 99 ? 99 : startingLevelValue + 1;
        }
      }
    }

    if (yValue < minThreshold) {
      if (currentMenuDisplay == settings) {
        if (currentSettingsCursor == 1) {
          startingLevelValue = startingLevelValue == 0 ? 0 : startingLevelValue - 1;
        }
      }
    }

    if (switchValue == 0) {
      if (currentMenuDisplay == main) {
        if (mainMenuCurrentCursor == 0) {
          toPlayMenu();
        } else if (mainMenuCurrentCursor == 1) {
          toHighscoreMenu();
        } else if (mainMenuCurrentCursor == 2) {
          toSettingsMenu();
        }
      } else if (currentMenuDisplay == gameOver) {
        toMainMenu();
      } else if (currentMenuDisplay == highscore) {
        toMainMenu();
      } else if (currentMenuDisplay == settings) {
        didPressOnSettings = !didPressOnSettings;
      }
    }
  }
}
