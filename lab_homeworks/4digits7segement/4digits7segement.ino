// Joystick
const int pinSW = A3; // digital pin connected to switch output
const int pinX = A1; // A1 - analog pin connected to X output
const int pinY = A0; // A0 - analog pin connected to Y output
int switchValue;
int previousMillisJoystick = 0;
int intervalJoystick = 200;
int xValue = 0;
int yValue = 0;

void setup() {
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

    Serial.print("X: ");
    Serial.print(xValue);
    Serial.print(" | ");
    Serial.print("Y: ");
    Serial.println(yValue);
    Serial.print("switch value: ");
    Serial.println(switchValue);
  }

}
