const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;

const int pinDP = 4;

const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;

unsigned int numD1 = 0;
unsigned int numD2 = 5;
unsigned int numD3 = 0;
unsigned int numD4 = 0;

int currentDigitsArr[4] = {0, 0, 0, 0};

unsigned int currentDisplay = 0;

// Joystick
const int pinSW = A3; // digital pin connected to switch output
const int pinX = A1; // A1 - analog pin connected to X output
const int pinY = A0; // A0 - analog pin connected to Y output
int switchValue;
bool switchPressed = false;
int xValue = 0;
int yValue = 0;
int minThreshold = 300;
int maxThreshold = 700;

// Timer
unsigned long previousMillisJoystick = 0;
const long intervalJoystick = 200;

unsigned long previousMillisBlink = 0;
const long intervalBlink = 500;
bool digitBlinkSwitch = false;

// segments array, similar to before
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

// digits array, to switch between them easily
int digits[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
};

byte digitMatrix[noOfDigits][segSize - 1] = {
  // a b c d e f g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1} // 9
};

void displayNumber(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  
  // write the decimalPoint to DP pin
  digitalWrite(segments[segSize - 1], decimalPoint);
}


// activate the display no. received as param
void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }
  
  digitalWrite(digits[num], LOW);
}


void clearLEDs() {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], LOW);
  }
}

void setup() {
  for (int i = 0; i < segSize - 1; i++) {
    pinMode(segments[i], OUTPUT);
  }
  
  for (int i = 0; i < noOfDisplays; i++) {
    pinMode(digits[i], OUTPUT);
  }

  pinMode(pinSW, INPUT_PULLUP); //activate pull-up resistor on the
  
  Serial.begin(9600);
}


void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisJoystick >= intervalJoystick) {
    previousMillisJoystick = currentMillis;
    
    switchValue = digitalRead(pinSW);
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);

    // change on joystick button press
    if (switchValue != 1) {
      switchPressed = !switchPressed;
    }

    // move from one to digit to another only if button wasn't activated
    if (!switchPressed) {
      if (xValue <= minThreshold) {
        currentDisplay = currentDisplay >= 3 ? 3 : currentDisplay+1;
      }
  
      if (xValue >= maxThreshold) {
        currentDisplay = currentDisplay <= 0 ? 0 : currentDisplay-1;
      }
    }

    // change number from current digit only if the button was activated
    if (switchPressed) {
      if (yValue >= maxThreshold) {
        currentDigitsArr[currentDisplay] = currentDigitsArr[currentDisplay] < 9 ? currentDigitsArr[currentDisplay]+1 : 9;
      }

      if (yValue <= minThreshold) {
        currentDigitsArr[currentDisplay] = currentDigitsArr[currentDisplay] > 0 ? currentDigitsArr[currentDisplay]-1 : 0;
      }
    }
  
    Serial.print("Current Display: ");
    Serial.print(currentDisplay);
    Serial.print(" | ");
    Serial.print("switch pressed: ");
    Serial.println(switchPressed);
    Serial.print("Curr number: ");
    Serial.println(currentDigitsArr[currentDisplay]);
  }

  for (int i = 0; i < noOfDisplays; i++) {
    clearLEDs();
    if (currentDisplay == i && switchPressed == false) {
      if (currentMillis - previousMillisBlink >= intervalBlink) {
        previousMillisBlink = currentMillis;
        showDigit(i);
        displayNumber(currentDigitsArr[i], HIGH);
        delay(20);
      }
    } else {
      showDigit(i);
      displayNumber(currentDigitsArr[i], HIGH);
      delay(1);
    }
  }
}
