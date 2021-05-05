// Rotary encoder volume controler
// Author: Hamish Johnson
// Updated May 4, 2021
// Github: https://github.com/Ham0osh/MediaControler

#include <SimpleRotary.h>
#include <Adafruit_NeoPixel.h>
#include <HID-Project.h>

// Setup Rotary encoder
SimpleRotary rotary(2,3,4); //<- CLK,DT,SW pins
// Setup Neopixel Ring
#define NEO_PIN        6 //<- IN pin
#define NUMPIXELS 12 //<- Number of pixels in chain
Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

// Media key definitions, see official USB docs for more
#define MEDIA_FAST_FORWARD  0xB3
#define MEDIA_REWIND  0xB4
#define MEDIA_NEXT  0xB5
#define MEDIA_PREVIOUS  0xB6
#define MEDIA_STOP  0xB7
#define MEDIA_PLAY_PAUSE  0xCD
#define MEDIA_VOLUME_MUTE 0xE2
#define MEDIA_VOLUME_UP 0xE9
#define MEDIA_VOLUME_DOWN 0xEA

// Input settings
const int debounceDelay = 10;
const int errorDelay = 2;
const int buttonPinL = 8;
const int buttonPinR = 9;
// Input state variables
int buttonStateL = 0;
int buttonStateR = 0;
int last_buttonStateL = 0;
int last_buttonStateR = 0;
// time variables
unsigned long timeSince;
unsigned long timeLast;
unsigned long timeTrail;
unsigned long timeFade;
// colour state variables
bool colIsDefault;
bool colChanged;
bool muted;

// Define colours used below
static const uint32_t BLACK = pixels.Color(1, 1, 1);
static const uint32_t WHITE = pixels.Color(255, 255, 255);
static const uint32_t BLU = pixels.Color(0, 255, 255);
static const uint32_t ORNG = pixels.Color(255, 128, 0);
static const uint32_t PURP = pixels.Color(127, 0, 155);
static const uint32_t light_PURP = pixels.Color(158, 72, 235);
static const uint32_t dark_PURP = pixels.Color(30, 0, 80);
static const uint32_t RED = pixels.Color(255, 5, 5);
static const uint32_t GRN = pixels.Color(102, 204, 1);

void setup() {
  timeTrail = 2000;     // Duration led's will stay changed before going back to standby
  timeFade = 20;        // Fade duration for mute and standby transitions
  // to keep track of time between presses
  timeSince = 0;
  timeLast = millis();
  // to keep track of colour states
  pixels.begin();
  colIsDefault = true;
  colChanged = false;
  muted = false;

  // Startup Animation
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, PURP);
    pixels.show();
    delay(timeFade*3);
  }
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, ORNG);
    pixels.show();
    delay(timeFade*3);
  }
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, BLU);
    pixels.show();
    delay(timeFade*3);
  }
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, PURP);
    pixels.show();
    delay(timeFade*3);
  }

  // Config rotary debounce settings
  rotary.setDebounceDelay(debounceDelay);
  rotary.setErrorDelay(errorDelay);
  // Setup Button pins
  pinMode(buttonPinL,INPUT_PULLUP);
  pinMode(buttonPinR,INPUT_PULLUP);
  
  // Start Serial Monitor
  Serial.begin(9600);
  Serial.println("Version 1.03 - May 4, 2021");
  Serial.println("Hamish Johnson");
}

void loop() {
  // Check button states
  byte i = rotary.rotate();
  byte t = rotary.pushType(50);
  buttonStateL = digitalRead(buttonPinL);
  buttonStateR = digitalRead(buttonPinR);

  // LeftButton
  if ( buttonStateL == 0 && last_buttonStateL == 1 ) {
    // Previous track
    Serial.println("LEFT");
    Consumer.write(MEDIA_PREVIOUS);
    for(int i=0; i<NUMPIXELS; i++) {
      if (i >= 6){
        pixels.setPixelColor(i, light_PURP);
      } else {
        pixels.setPixelColor(i, dark_PURP);
      }
    }
    colChanged = true;
    timeLast = millis();
    colIsDefault = false;
  }
  
  // RightButton
  if ( buttonStateR == 0 && last_buttonStateR == 1 ) {
    // Next track
    Serial.println("RIGHT");
    Consumer.write(MEDIA_NEXT);
    for(int i=0; i<NUMPIXELS; i++) {
      if (i < 6){
        pixels.setPixelColor(i, light_PURP);
      } else {
        pixels.setPixelColor(i, dark_PURP);
      }
    }
    colChanged = true;
    timeLast = millis();
    colIsDefault = false;
  }
 
  // CW
  if ( i == 1 ) {
    // Volume up
    Serial.println("CW");
    Consumer.write(MEDIA_VOLUME_UP);
    // Do something cool
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, ORNG);
    }
    colChanged = true;
    timeLast = millis();
    colIsDefault = false;
    muted = false;
  }  
  
  // CCW
  if ( i == 2 ) {
    // Volume down
    Serial.println("CCW");
    Consumer.write(MEDIA_VOLUME_DOWN);
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, BLU);
    }
    colChanged = true;
    timeLast = millis();
    colIsDefault = false;
    muted = false;
  }
  
  //Long push
  if ( t == 2 ) {
    Serial.println("Long Push");
    Consumer.write(MEDIA_VOLUME_MUTE);
    timeLast = millis();
    colIsDefault = false;
    // toggle muted
    muted = not(muted);
    if (not(muted)){
      for(int i=0; i<NUMPIXELS; i++) {
        pixels.setPixelColor(i, PURP);
      }
      colIsDefault = true;
    }
     colChanged = true;
  }

  //Short push
  if ( t == 1 ) {
    // Pause
    Serial.println("Short Push");
    Consumer.write(MEDIA_PLAY_PAUSE);
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, GRN);
    }
    colChanged = true;
    colIsDefault = false;
    timeLast = millis() + 1000;
  }
  
  // Check if its time to switch back to standby
  if (  not(colIsDefault) && not(muted) && millis()-timeLast >= timeTrail ){
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, PURP);
      pixels.show();
      delay(timeFade);
    }
    colChanged = false;
    colIsDefault = true;
  }
  // Make red if muted
  if (muted && colChanged){
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, RED);
      pixels.show();
      delay(timeFade);
    }
    colChanged = false;
  }
  // Update colour
  if (colChanged){
    pixels.show();
  }

  // Send button states and reset if colours need to be updated
  delay(10);
  last_buttonStateL = buttonStateL;
  last_buttonStateR = buttonStateR;
  colChanged = false;
}
