#include "AiEsp32RotaryEncoder.h"

#if defined(ESP8266)
#define ROTARY_ENCODER_A_PIN D6
#define ROTARY_ENCODER_B_PIN D5
#define ROTARY_ENCODER_BUTTON_PIN D7
#else
#define ROTARY_ENCODER_A_PIN 33
#define ROTARY_ENCODER_B_PIN 32
#define ROTARY_ENCODER_BUTTON_PIN 25
#endif
#define ROTARY_ENCODER_STEPS 4
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS);
byte value;


void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
    value = rotaryEncoder.readEncoder();
}

void encoderTurned(){
      display.clearDisplay();
    display.display();
    display.setCursor(0,0);
    display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.println("Value: ");
  display.println(value);
  display.display();
}

void buttonPressed(){
  display.display();
  display.clearDisplay();
  Serial.print("ButtonPressed");
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("MIDI randomiser ");
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.println("CH1: ");
  display.println("CH2: ");
  display.println("CH3: ");
  display.display();
  display.clearDisplay();
}
void setup()
{
  display.setRotation(2);
    Serial.begin(115200);
      delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
 
  display.display();
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setBoundaries(0, 127, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
    rotaryEncoder.setAcceleration(100);
}

void loop()
{
    if (rotaryEncoder.encoderChanged())
    {
        encoderTurned();
    }
    if (rotaryEncoder.isEncoderButtonClicked())
    {
        buttonPressed();
    }
}
