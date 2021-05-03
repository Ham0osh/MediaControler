#include <SimpleRotary.h>
SimpleRotary rotary(2,3,4);

#include <Adafruit_NeoPixel.h>
#define PIN        6
#define NUMPIXELS 12
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#include <HID-Project.h>
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

#define CONSUMER_EMAIL_READER 0x18A
#define CONSUMER_CALCULATOR 0x192
#define CONSUMER_EXPLORER 0x194

const int debounceDelay = 10;
const int errorDelay = 2;
const int buttonPinL = 8;
const int buttonPinR = 9;
int buttonStateL = 0;
int buttonStateR = 0;
int last_buttonStateL = 0;
int last_buttonStateR = 0;

void setup() {
  // put your setup code here, to run once:
  pixels.begin();

 for(int i=0; i<NUMPIXELS; i++) {
   pixels.setPixelColor(i, pixels.Color(127, 0, 155));
   pixels.show();
 }
  
  rotary.setDebounceDelay(debounceDelay);
  rotary.setErrorDelay(errorDelay);
  Serial.println("Version 1.02 - May 2, 2020");

  pinMode(buttonPinL,INPUT_PULLUP);
  pinMode(buttonPinR,INPUT_PULLUP);
  
  // Start Serial Monitor
  Serial.begin(9600);
}

void loop() {
  pixels.clear();
  byte i = rotary.rotate();
  byte t = rotary.pushType(50);
  buttonStateL = digitalRead(buttonPinL);
  buttonStateR = digitalRead(buttonPinR);

  // LeftButton
  if ( buttonStateL == 0 && last_buttonStateL == 1 ) {
    Serial.println("LEFT");
    Consumer.write(MEDIA_PREVIOUS);
  }
  // RightButton
  if ( buttonStateR == 0 && last_buttonStateR == 1 ) {
    Serial.println("RIGHT");
    Consumer.write(MEDIA_NEXT);
  }
 
  // CW
  if ( i == 1 ) {
    Serial.println("CW");
    Consumer.write(MEDIA_VOLUME_UP);
    // Do something cool
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 128, 0));
      pixels.show();
    }
  }  
  
  // CCW
  if ( i == 2 ) {
    Serial.println("CCW");
    Consumer.write(MEDIA_VOLUME_DOWN);
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 255, 255));
      pixels.show();
    // Do something cool
    }
  }
  

  //Short push
  if ( t == 1 ) {
    // Do something cool.
    Serial.println("Short Push");
    Consumer.write(MEDIA_PLAY_PAUSE);
  }
  
  //Long push
  if ( t == 2 ) {
    // Do something cool.
    Serial.println("Long Push");
    Consumer.write(MEDIA_VOLUME_MUTE);
  }
  delay(10);
  last_buttonStateL = buttonStateL;
  last_buttonStateR = buttonStateR;
}
