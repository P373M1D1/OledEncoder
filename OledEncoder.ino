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
#define switch 27
#define gnd 14
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS);
byte value;
const bool displayCleared = false;
bool stateSwitch = true;
bool lastStateSwitch = true;
//int midiChannel[17];

struct midiChannelSettings{
  struct channelEntry{
    uint8_t maxPresetNumber;
    uint8_t midiCCactive;
    uint8_t activeValue;
    uint8_t bypassValue;
  }
  channel[17];  
  };


void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
    value = rotaryEncoder.readEncoder();
}

void encoderTurned(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.println("Value: ");
  display.println(value);
}


void encoderPressed(){
  display.clearDisplay();
  Serial.print("ButtonPressed");
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Encoder pressed   ");
}
void buttonPressed(){
  display.clearDisplay(); 
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Button pressed");
}

void setup()
{
  display.setRotation(2);
  Serial.begin(115200);
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
 display.display(); // show splashscreen
 delay(2000);
 pinMode(switch, INPUT_PULLUP);
 pinMode(gnd, OUTPUT);
 digitalWrite(gnd, LOW);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(SH110X_WHITE);
display.setCursor(0, 0);
startupScreen();
display.display();
delay(1000);
rotaryEncoder.begin();
rotaryEncoder.setup(readEncoderISR);
rotaryEncoder.setBoundaries(0, 127, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
rotaryEncoder.setAcceleration(100);
//screenWash();
startupMessage();
}

void loop()
{ 
  midiChannelSettings settings;

  for (int i = 1; i < 17; i++){
    settings.channel[i].maxPresetNumber = 127;
    settings.channel[i].midiCCactive = 1;
    settings.channel[i].activeValue = 127;
    settings.channel[i].bypassValue = 0;
  }

   stateSwitch = digitalRead(switch);
   display.display();
    if (rotaryEncoder.encoderChanged())
    {
        encoderTurned();
    }
    if (rotaryEncoder.isEncoderButtonClicked())
    {
        encoderPressed();
    }

    if (!stateSwitch && lastStateSwitch == true){
      if (stateSwitch == LOW);
      buttonPressed();
    }
 lastStateSwitch = stateSwitch; 
for (int i = 1; i <= 16; i++){
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.print("Channel: ");
  display.println(i);
  display.print("Max Preset: ");
  display.println(settings.channel[i].maxPresetNumber);
  display.print("ToggleActiveCC: ");
  display.println(settings.channel[i].midiCCactive);
  display.print("ActiveValue: ");
  display.println(settings.channel[i].activeValue);
  display.print("BypassValue: ");
  display.println(settings.channel[i].bypassValue);
  display.display();
  delay(1000);

}
}