const int redLedPin = 11;
const int greenLedPin = 10;
const int blueLedPin = 9;

const int redPotPin = A0;
const int greenPotPin = A1;
const int bluePotPin = A2;

int redPotValue = 0;
int greenPotValue = 0;
int bluePotValue = 0;

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  pinMode(redPotPin, INPUT);
  pinMode(greenPotPin, INPUT);
  pinMode(bluePotPin, INPUT);
}

void loop() {
  readPotValues();
  setLedColors(); 
}

void readPotValues() {
  redPotValue = analogRead(redPotPin);
  greenPotValue = analogRead(greenPotPin);
  bluePotValue = analogRead(bluePotPin);
}

void setLedColors() {
  analogWrite(redLedPin, map(redPotValue, 0, 1023, 0, 255));
  analogWrite(greenLedPin, map(greenPotValue, 0, 1023, 0, 255));
  analogWrite(blueLedPin, map(bluePotValue, 0, 1023, 0, 255));
}