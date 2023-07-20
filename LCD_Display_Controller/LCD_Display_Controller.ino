#include <LiquidCrystal.h>

/*
 * LCD RS pin to digital pin 5
 * LCD Enable pin to digital pin 6
 * LCD D4 pin to digital pin 7
 * LCD D5 pin to digital pin 8
 * LCD D6 pin to digital pin 9
 * LCD D7 pin to digital pin 10
 * Input String coming signals pin 13
*/

const int rs = 5, en = 6, d4 = 7, d5 = 8, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define the input pins
// int inputPins[] = { A0, A1, A2, A3, A4, A5, A6, A7 };
int inputPins[] = { A7, A6, A5, A4, A3, A2, A1, A0 };

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(57600);
  pinMode(13, INPUT);

  // Set all the input pins as INPUT
  for (int i = 0; i < 8; i++) {
    pinMode(inputPins[i], INPUT);
  }
}

bool previousState = LOW;

void loop() {
  // Check if there is a new character to read

  bool currentState = digitalRead(13);

  if (currentState == HIGH && previousState == LOW) {
    int asciiValue = 0;
    String binaryString = "";  // Define a String to hold the binary values

    // Read the 8 bits from the input pins
    for (int i = 0; i < 8; i++) {
      int binaryValue = 0;
      if (analogRead(inputPins[i]) >= 850) {
        binaryValue = 1;
      }
      binaryString += String(binaryValue);  // Add the binary value to the string
    }

    byte binary_val = (byte) strtol(binaryString.c_str(), NULL, 2);

    // Convert the binary value to an ASCII character
    char character = (char) binary_val;

    // Print the character to the LCD screen
    lcd.print(character);
  }

  previousState = currentState;
}
