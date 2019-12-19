#include "Menu.hpp"

Menu::Menu(LiquidCrystal *lcd, Game *game) {
    this->lcd = lcd;
    this->game = game;
}

void Menu::changeScreen(Screen newScreen) {
    lcd->clear();
    this->screen = newScreen;
}

void Menu::display() {
  if (screen == PLAY_MENU && game->checkIfGameOver() && !gameOverWasDisplayed) {
    changeScreen(GAME_OVER_MENU);
    gameOverWasDisplayed = true;
  }
  switch (screen) {
  case MAIN_MENU:
    displayMainMenu();
    break;
  case PLAY_MENU:
    displayPlayMenu();
    break;
  case HIGHSCORE_MENU:
    displayHighscoreMenu();
    break;
  case SETTINGS_MENU:
    displaySettingsMenu();
    break;
  case INFO_MENU:
    displayInfoMenu();
    break;
  case GAME_OVER_MENU:
    displayGameOverMenu();
    break;
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisCursorBlink >= intervalCursorBlink) {
    previousMillisCursorBlink = currentMillis;

    displayCursor(isCursorBlinkOn);

    isCursorBlinkOn = !isCursorBlinkOn;
  }
}

void Menu::displayMainMenu() {
  lcd->setCursor(1, 0);
  lcd->print("Play");

  lcd->setCursor(7, 0);
  lcd->print("Highscore");

  lcd->setCursor(1, 1);
  lcd->print("Settings");

  lcd->setCursor(12, 1);
  lcd->print("Info");
}

void Menu::displayPlayMenu() {
  lcd->setCursor(0, 0);
  lcd->print("Lives: ");

  lcd->setCursor(6, 0);
  lcd->print(game->getLives());

  lcd->setCursor(9, 0);
  lcd->print("Lvl: ");

  lcd->setCursor(14, 0);
  lcd->print(game->getLevel());

  lcd->setCursor(0, 1);
  lcd->print("Score: ");
  
  lcd->setCursor(7, 1);
  lcd->print(game->getScore());
}

void Menu::displayHighscoreMenu() {
  lcd->setCursor(0, 0);
  lcd->print("<<");
  
  lcd->setCursor(5, 0);
  lcd->print("Highscore");

  lcd->setCursor(7, 1);
  lcd->print(getHighscore());
}

void Menu::displaySettingsMenu() {
  lcd->setCursor(0, 0);
  lcd->print("<<");

  lcd->setCursor(3, 0);
  lcd->print("Name:");

  unsigned long currentMillis = millis();
    if (currentMillis - previousMillisSettingsBlink >= intervalSettingsBlink) {
      previousMillisSettingsBlink = currentMillis;

      displaySettingsName(isSettingsBlinkOn);
      displaySettingsLevelStart(isSettingsBlinkOn);
      isSettingsBlinkOn = !isSettingsBlinkOn;
    }
}

void Menu::displaySettingsLevelStart(bool isBlinking) {
  lcd->setCursor(3, 1);
  lcd->print("Lvl start:");

  lcd->setCursor(14, 1);

  if (!isChangindSettings || settingsScreenCursor == NAME) { 
    lcd->print(levelStart);
  } else if (isChangindSettings && settingsScreenCursor == LEVEL) {
    if (isBlinking) {
      lcd->print("  ");
    } else {
      lcd->print(levelStart);
    }  
  }
}

void Menu::displaySettingsName(bool isBlinking) {
  if (!isChangindSettings || settingsScreenCursor == LEVEL) {
    lcd->setCursor(9, 0);
    lcd->print(settingsName);
  } else if (isChangindSettings && settingsScreenCursor == NAME) {
    for (int i = 0; i < NAME_LEN; i++) {
      lcd->setCursor(9+i, 0);
      if (i == currentNameLetterIndex) {
        if (isBlinking) {
          lcd->print(" ");
        } else {
          lcd->print(settingsName[i]);
        }
      } else {
        lcd->print(settingsName[i]);
      }
    }
  }
}

void Menu::displayInfoMenu() {
  switch (infoPage)
  {
  case ONE:
    displayInfoPageOneMenu();
    break;
  
  case TWO:
    displayInfoPageTwoMenu();
    break;
  }
}

void Menu::displayGameOverMenu() {
  lcd->setCursor(0, 0);
  lcd->print("g over");

  lcd->setCursor(7, 0);
  lcd->print("score: ");

  lcd->setCursor(14, 0);
  lcd->print(game->getScore());

  if (checkIfHighscore(game->getScore())) {
    lcd->setCursor(0, 1);
    lcd->print("!!HIGHSCORE!!");
    writeHighscore(game->getScore());
  }

}

void Menu::displayInfoPageOneMenu() {
  lcd->setCursor(0, 0);
  lcd->print("<<");

  lcd->setCursor(2, 0);
  lcd->print("Sabin Hantu");

  lcd->setCursor(2, 1);
  lcd->print("git: sabin10");

  lcd->setCursor(15, 0);
  lcd->print(">");

  lcd->setCursor(15, 1);
  lcd->print(">");
}

void Menu::displayInfoPageTwoMenu() {
  lcd->setCursor(2, 0);
  lcd->print("Spazeship");

  lcd->setCursor(1, 1);
  lcd->print("@unibucrobotics");

  lcd->setCursor(0, 0);
  lcd->print("<");

  lcd->setCursor(0, 1);
  lcd->print("<");
}

void Menu::handleXMovement(bool isUp) {
  switch (screen)
  {
  case MAIN_MENU:
    handleCursorOnMain(isUp);
    break;
  case PLAY_MENU:
    break;
  case HIGHSCORE_MENU:
    handleCursorOnHighscore(isUp);
    break;
  case SETTINGS_MENU:
    handleCursorOnSettings(isUp);
    break;
  case INFO_MENU:
    handleCursorOnInfo(isUp);
    break;
  }
}

void Menu::handleCursorOnMain(bool isUp) {
  int currentMainCursorVal = mainCursorVal;
  int newMainCursorVal;
  if (isUp) {
    newMainCursorVal = currentMainCursorVal < 3 ? currentMainCursorVal + 1 : currentMainCursorVal;
  } else {
    newMainCursorVal = currentMainCursorVal > 0 ? currentMainCursorVal - 1 : currentMainCursorVal;
  }
  changeMainScreenCursorVal(newMainCursorVal);
}

void Menu::handleCursorOnHighscore(bool isUp) {
  if (!isUp) {
    changeScreen(MAIN_MENU);
    return;
  }
}

void Menu::handleCursorOnSettings(bool isUp) {
  if (!isChangindSettings) {
    int currentSettingsCursorVal = settingsCursorVal;
    int newSettingsCursorVal;
    if (!isUp && settingsScreenCursor == NAME) {
      changeScreen(MAIN_MENU);
      isChangindSettings = false;
      return;
    } else if (isUp) {
      newSettingsCursorVal = currentSettingsCursorVal < 1 ? currentSettingsCursorVal + 1 : currentSettingsCursorVal;
    } else if (!isUp) {
      newSettingsCursorVal = currentSettingsCursorVal > 0 ? currentSettingsCursorVal - 1 : currentSettingsCursorVal;
    }
    changeSettingsScreenCursorVal(newSettingsCursorVal);
  } else if (isChangindSettings && settingsScreenCursor == NAME) {
    if (isUp) {
      currentNameLetterIndex = currentNameLetterIndex < NAME_LEN - 1 ? currentNameLetterIndex + 1 : currentNameLetterIndex;
    } else {
      currentNameLetterIndex = currentNameLetterIndex > 0 ? currentNameLetterIndex - 1 : currentNameLetterIndex;
    }
  }
}

void Menu::handleCursorOnInfo(bool isUp) {
  if (isUp && infoPage == ONE) {
    infoPage = TWO;
    changeScreen(INFO_MENU);
  } else if (!isUp && infoPage == TWO) {
    infoPage = ONE;
    changeScreen(INFO_MENU);
  } else if (!isUp && infoPage == ONE) {
    changeScreen(MAIN_MENU);
  }
}

void Menu::handleYMovement(bool isUp) {
  switch (screen)
  {
  case SETTINGS_MENU:
    handleYOnSettings(isUp);
    break;
  default:
    break;
  }
}

void Menu::handleYOnSettings(bool isUp) {
  if (!isChangindSettings)
    return;

  switch (settingsScreenCursor)
  {
  case LEVEL:
    if (isUp) {
      levelStart = levelStart < MAX_LEVEL - 2 ? levelStart + 1 : levelStart;
    } else {
      levelStart = levelStart > 0 ? levelStart - 1 : levelStart;
    }
    break;
  case NAME:
    int currentLetterAscii = int(settingsName[currentNameLetterIndex]);
    if (isUp) {
      currentLetterAscii = currentLetterAscii < ASCII_MAX ? currentLetterAscii + 1 : currentLetterAscii;
    } else {
      currentLetterAscii = currentLetterAscii > ASCII_MIN ? currentLetterAscii - 1 : currentLetterAscii;
    }
    settingsName[currentNameLetterIndex] = char(currentLetterAscii);
    break;
  
  default:
    break;
  }

}

void Menu::handleOnBtnPressed() {
  switch (screen)
  {
  case MAIN_MENU:
    handleOnBtnPressedOnMain();
    break;
  case SETTINGS_MENU:
    handleOnBtnPressedOnSettings();
    break;
  case GAME_OVER_MENU:
    handleOnBtnPressedOnGameOver();
    break;
  
  default:
    break;
  }
}

void Menu::handleOnBtnPressedOnMain() {
  switch (mainScreenCursor)
  {
  case PLAY:
      changeScreen(PLAY_MENU);
      break;
  case HIGHSCORE:
      changeScreen(HIGHSCORE_MENU);
      break;
  case SETTINGS:
      changeScreen(SETTINGS_MENU);
      break;
  case INFO:
      changeScreen(INFO_MENU);
      break;
  default:
      break;
  }
}

void Menu::handleOnBtnPressedOnSettings() {
  if (!isChangindSettings) {
    currentNameLetterIndex = 0;
  }
  isChangindSettings = !isChangindSettings;

}

void Menu::handleOnBtnPressedOnGameOver() {
  changeScreen(MAIN_MENU);
  game->resetGame();
  gameOverWasDisplayed = false;
}

void Menu::displayCursor(bool isCursorOn) {
  switch (screen) {
  case MAIN_MENU:
    displayMainCursor(isCursorOn);
    break;
  case SETTINGS_MENU:
    displaySettingsCursor(isCursorOn);
    break;
  }
}

void Menu::clearCursor() {
  switch (screen) {
  case MAIN_MENU:
    displayMainCursor(false);
    break;  
  case SETTINGS_MENU:
    displaySettingsCursor(false);
    break;

  }
}


void Menu::displayMainCursor(bool isCursorOn) {
  switch (mainScreenCursor) {
  case PLAY:
    lcd->setCursor(0, 0);
    break;
  case HIGHSCORE:
    lcd->setCursor(6, 0);
    break;
  case SETTINGS:
    lcd->setCursor(0, 1);
    break;
  case INFO:
    lcd->setCursor(11, 1);
    break;
  }

  if (isCursorOn) {
    lcd->print(">");
  } else {
    lcd->print(" ");
  }
}

void Menu::displaySettingsCursor(bool isCursorOn) {
  switch (settingsScreenCursor)
  {
  case NAME:
    lcd->setCursor(2, 0);
    break;
  case LEVEL:
    lcd->setCursor(2, 1);
    break;
  }

  if (isCursorOn) {
    lcd->print(">");
  } else {
    lcd->print(" ");
  }
}

void Menu::changeMainScreenCursor(MainScreenCursor mainScreenCursor) {
  this->mainScreenCursor = mainScreenCursor;
}

void Menu::changeMainScreenCursorVal(unsigned int newMainCursorVal) {
  mainCursorVal = newMainCursorVal;
  MainScreenCursor newMainScreenCursor;

  // clear the old position
  clearCursor();

  switch (newMainCursorVal) {
  case 0:
    newMainScreenCursor = PLAY;
    break;
  case 1:
    newMainScreenCursor = HIGHSCORE;
    break;
  case 2:
    newMainScreenCursor = SETTINGS;
    break;
  case 3:
    newMainScreenCursor = INFO;
    break;
  default:
    newMainScreenCursor = PLAY;
    break;
  }

  changeMainScreenCursor(newMainScreenCursor);
}

bool Menu::checkIfGameShouldStart() {
  if (screen == MAIN_MENU && mainScreenCursor == PLAY) {
    return true;
  }
  return false;
}

void Menu::setGame(Game *game) {
  this->game = game;
}


void Menu::changeSettingsScreenCursor(SettingsScreenCursor settingsScreenCursor) {
  this->settingsScreenCursor = settingsScreenCursor;
}

void Menu::changeSettingsScreenCursorVal(unsigned int newSettingsCursorVal) {
  settingsCursorVal = newSettingsCursorVal;
  SettingsScreenCursor newSettingsScreenCursor;
  clearCursor();

  switch (newSettingsCursorVal)
  {
  case 0:
    newSettingsScreenCursor = NAME;
    break;
  case 1:
    newSettingsScreenCursor = LEVEL;
    break;
  default:
    newSettingsScreenCursor = NAME;
    break;
  }
  changeSettingsScreenCursor(newSettingsScreenCursor);
}

int Menu::getLevelStart() {
  return levelStart;
}

void Menu::writeHighscore(int highscore) {
  int highscoreMemory = EEPROM.read(HIGHSCORE_ADDRESS);
  if (highscoreMemory == 255) {
    EEPROM.write(HIGHSCORE_ADDRESS, highscore);
  } else {
    EEPROM.put(HIGHSCORE_ADDRESS, highscore);
  }
}

int Menu::getHighscore() {
  int highscoreValue;
  int highscoreMemory = EEPROM.read(HIGHSCORE_ADDRESS);
  if (highscoreMemory == 255) {
    highscoreValue = 0;
  } else {
    highscoreValue = highscoreMemory;
  }
  return highscoreValue;
}

bool Menu::checkIfHighscore(int score) {
  if (score > getHighscore()) {
    return true;
  }
  return false;
}