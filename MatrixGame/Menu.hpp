#include <LiquidCrystal.h>
#include "Arduino.h"
#include "Game.hpp"
#include <EEPROM.h>

#define NAME_LEN 6
#define ASCII_MIN 65
#define ASCII_MAX 90
#define HIGHSCORE_ADDRESS 0

class Menu 
{
public:
enum Screen {
  MAIN_MENU,
  PLAY_MENU,
  HIGHSCORE_MENU,
  SETTINGS_MENU,
  INFO_MENU,
  GAME_OVER_MENU
};

enum MainScreenCursor {
  PLAY,
  HIGHSCORE,
  SETTINGS,
  INFO
};

enum SettingsScreenCursor {
  NAME,
  LEVEL
};

enum InfoPage {
  ONE,
  TWO
};

  Menu(LiquidCrystal *lcd, Game *game);

  void changeScreen(Screen newScreen);

  void display();
  void displayMainMenu();
  void displayPlayMenu();
  void displayHighscoreMenu();
  void displaySettingsMenu();
  void displaySettingsLevelStart(bool isBlinking);
  void displaySettingsName(bool isBlinking);
  void displayInfoMenu();
  void displayGameOverMenu();

  void displayInfoPageOneMenu();
  void displayInfoPageTwoMenu();

  void handleXMovement(bool isUp);
  void handleCursorOnMain(bool isUp);
  void handleCursorOnHighscore(bool isUp);
  void handleCursorOnSettings(bool isUp);
  void handleCursorOnInfo(bool isUp);

  void handleYMovement(bool isUp);
  void handleYOnSettings(bool isUp);

  void handleOnBtnPressed();
  void handleOnBtnPressedOnMain();
  void handleOnBtnPressedOnSettings();
  void handleOnBtnPressedOnGameOver();

  void displayCursor(bool isCursorOn);
  void displayMainCursor(bool isCursorOn);
  void displaySettingsCursor(bool isCursorOn);
  void clearCursor();

  // Main Screen
  void changeMainScreenCursor(MainScreenCursor mainScreenCursor);
  void changeMainScreenCursorVal(unsigned int newMainCursorVal);

  // Play Screen
  bool checkIfGameShouldStart();
  void setGame(Game *game);

  // Settings Screen
  void changeSettingsScreenCursor(SettingsScreenCursor settingsScreenCursor);
  void changeSettingsScreenCursorVal(unsigned int newScreenCursorVal);
  int getLevelStart();

  // Highscore
  void writeHighscore(int highscore);
  int getHighscore();
  bool checkIfHighscore(int score);

private:
  LiquidCrystal *lcd;
  Game *game;
  Screen screen = MAIN_MENU;

  bool gameOverWasDisplayed = false;

  // Main Screen
  MainScreenCursor mainScreenCursor = PLAY;
  unsigned int mainCursorVal = 0;

  // Settings Screen
  SettingsScreenCursor settingsScreenCursor = NAME;
  unsigned int settingsCursorVal = 0;
  char settingsName[NAME_LEN] = {'P', 'L', 'A', 'Y', 'E', 'R'};
  bool isChangindSettings = false;
  int levelStart = 0;
  unsigned long previousMillisSettingsBlink = 0;
  unsigned long intervalSettingsBlink = 300;
  bool isSettingsBlinkOn = false;
  int currentNameLetterIndex = 0;

  // Info Screen
  InfoPage infoPage = ONE;

  // Blink Cursor
  unsigned long previousMillisCursorBlink = 0;
  unsigned long intervalCursorBlink = 300;
  bool isCursorBlinkOn = false;
};