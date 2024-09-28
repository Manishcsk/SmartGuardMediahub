#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

SoftwareSerial mySoftwareSerial(2, 3); // RX, TX - //On DFplayerMini connect Rx to Arduino Pin 3 (Tx) and Tx to Arduino Pin 2 (Rx)
DFRobotDFPlayerMini myDFPlayer;

#define fireSensor 6

unsigned long previousMillis = 0;
const long interval = 1300; // 1 second
bool showFireMessage = false;
int displayState = 0;

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));
    while (true);
  }

  Serial.println();
  Serial.println(F("DFPlayer Mini module initialized!"));
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(60);
  myDFPlayer.EQ(0);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Initialize with the I2C addr 0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  pinMode(fireSensor, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  int fire = digitalRead(fireSensor);

  switch (displayState) {
    case 0: // No fire detected
      if (fire == 1) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor((SCREEN_WIDTH - 8 * 2 * 8) / 2, (SCREEN_HEIGHT - 16 * 2) / 2); // Center the text
        display.print("HOME AIN'T BURNING.");
        display.display();
        Serial.println("No fire!");
        showFireMessage = false;
      } else {
        displayState = 1; // Move to fire detected state
        previousMillis = currentMillis;
      }
      break;

    case 1: // Fire detected
      if (fire == 1) {
        displayState = 0; // Move back to no fire state
        display.clearDisplay();
        display.display();
        Serial.println("Fire cleared");
      } else {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          showFireMessage = !showFireMessage;
          display.clearDisplay();
          if (showFireMessage) {
            display.setTextSize(2);
            display.setCursor((SCREEN_WIDTH - 8 * 2 * 14) / 2, (SCREEN_HEIGHT - 16 * 2) / 2); // Center the text
            display.print("    YOUR HOME IS BURNING");
            Serial.println("  FIRE DETEC..");
            myDFPlayer.play(1);
          }
          display.display();
        }
      }
      break;
  }
}
