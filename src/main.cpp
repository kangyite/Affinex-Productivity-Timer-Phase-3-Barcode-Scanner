#include "Keyboard.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
const byte rxPin = 9;
const byte txPin = 10;
SoftwareSerial mySerial(rxPin, txPin);

String s = "";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
  // open the serial port:
  Serial.begin(115200);
  Serial1.begin(115200);
  mySerial.begin(115200);
  pinMode(txPin, OUTPUT);
  // initialize control over the keyboard:
  Keyboard.begin();

  Serial.println("start");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();

  display.setTextSize(3);
  display.setTextColor(WHITE);

  // Display static text
}

void loop()
{
  while (Serial1.available())
  {
    char inChar = (char)Serial1.read();

    if (inChar == 13)
    {
      Serial.println("Data: " + s);
      display.clearDisplay();
      display.setCursor(9, 25);
      display.print(s);
      display.display();
      s += ",";
      Keyboard.print(s);
      s = "";
      mySerial.println("@@");
    }
    else
      s += inChar;
  }
}
