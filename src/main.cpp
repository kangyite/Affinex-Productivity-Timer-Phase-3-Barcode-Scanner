#include "Keyboard.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MEMORY_SIZE 20
#define BUF_SIZE 20

char buf[BUF_SIZE];

uint32_t mem[MEMORY_SIZE];
byte memIdx = 0;
byte idx = 0;
String s = "";
int mins = 123;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool send = false;

bool checkMem(char _buf[]);

void setup()
{
  // open the serial port:
  Serial.begin(115200);
  Serial1.begin(115200);

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

  memset(mem, 0, sizeof(mem));
}

void loop()
{
  while (Serial1.available())

  {
    char inChar = Serial1.read();
    if (inChar == 13)
    {
      buf[idx] = 0;
      if (checkMem(buf))
      {
        display.clearDisplay();
        display.setCursor(9, 25);
        display.print(buf);
        display.display();
        Keyboard.print(buf);
        Keyboard.print(',');
        send = true;
      }
      while (Serial.available()) // clear rx buffer
        Serial.read();
      idx = 0;
    }
    else
    {
      if (idx >= BUF_SIZE)
      {
        Serial.println("too long");
        idx = 0;
      }
      buf[idx++] = inChar;
    }
  }
  if (send)
  {
    Serial1.write("@");
    send = false;
  }
}

bool checkMem(char _buf[])
{
  if (strlen(_buf) != 6)
  {
    Serial.println(strlen(_buf));
    return false;
  }
  uint32_t temp = 0;
  uint32_t expo = 100000;
  for (int i = 0; i < 6; i++)
  {
    if (_buf[i] > '9' || buf[i] < '0')
      return false;
    temp += (_buf[i] - '0') * expo;
    expo /= 10;
  }
  for (int i = 0; i < MEMORY_SIZE; i++)
  {
    if (temp == mem[i])
    {
      Serial.println("found same... ignored");
      return false;
    }
  }
  mem[memIdx++] = temp;
  if (memIdx >= MEMORY_SIZE)
    memIdx = 0;
  return true;
}