#include "Keyboard.h"
#define SW 10
String s = "";

// void serial1Event();

void setup()
{
  // open the serial port:
  Serial.begin(115200);
  Serial1.begin(115200);
  // initialize control over the keyboard:
  Keyboard.begin();

  pinMode(SW, OUTPUT);
  Serial.println("start");

  // serial1Event();
}

void loop()
{
  while (Serial1.available())
  {
    char inChar = (char)Serial1.read();

    if (inChar == 13)
    {
      Serial.println("got it: " + s);
      s += ",";
      Keyboard.print(s);
      s = "";
      digitalWrite(SW, HIGH);
      delay(100);
      digitalWrite(SW, LOW);
    }
    else
      s += inChar;
  }
}
