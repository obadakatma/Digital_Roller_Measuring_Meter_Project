#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>


#define clk 2
#define dt 3
#define button 5
#define longButton 7
#define PI 3.14159265358979323846
#define diameter 39.4 //diameter in mm

float oldPosition = -999, newPosition, distance, steps = 80, radius = diameter / 2;
const unsigned long LONG_PRESS_TIME  = 700; // in milliseconds

int lastState = 1;  // the previous state from the button
int currentState;     // the current reading from the button
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;

bool boot, inch;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Encoder myEnc(3, 2);

void setup() {
  Serial.begin(9600);
  pinMode (clk, INPUT);
  pinMode (dt, INPUT);
  pinMode (button, INPUT);
  pinMode (longButton, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Long press");
  lcd.setCursor(3, 1);
  lcd.print("mm -> in");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("dist= 0 mm");
}

void loop() {
  currentState = digitalRead(longButton);

  if (lastState == 0 && currentState)       // button is pressed
    pressedTime = millis();
  else if (lastState && currentState == 0) { // button is released
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if ( pressDuration > LONG_PRESS_TIME && boot)
    {
      inch = !inch;
      lcd.setCursor(0, 0);
      lcd.print("dist= ");
      lcd.print((inch) ? abs(distance / 25.4) : abs(distance));
      lcd.print((inch) ? " in    " : " mm    ");
    }
  }

  lastState = currentState;
  boot = 1;

  if (digitalRead(button))
    myEnc.write(0);
  else
    newPosition = myEnc.read();

  // Calculating the measured distance
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    distance = ((2 * PI * radius) / steps) * newPosition;

    // Printing the result on lcd
    lcd.setCursor(0, 0);
    lcd.print("dist= ");
    lcd.print((inch) ? abs(distance / 25.4) : abs(distance));
    lcd.print((inch) ? " in    " : " mm    ");
  }
}
