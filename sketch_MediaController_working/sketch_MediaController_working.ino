#include <SimpleRotary.h>
SimpleRotary rotary(2,3,4);

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

void setup() {
  // put your setup code here, to run once:
  rotary.setDebounceDelay(debounceDelay);
  rotary.setErrorDelay(errorDelay);
  Serial.println("Version 1.01 - May 2, 2020");
}

void loop() {
  byte i = rotary.rotate();
  byte t = rotary.pushType(20);
  
  // CW
  if ( i == 1 ) {
    Serial.println("CW");
    Consumer.write(MEDIA_VOLUME_UP);
    // Do something cool
  }
  
  // CCW
  if ( i == 2 ) {
    Serial.println("CCW");
    Consumer.write(MEDIA_VOLUME_DOWN);
    // Do something cool
  }

  //Long push
  if ( t == 2 ) {
    // Do something cool.
    Serial.println("Long Push");
    Consumer.write(MEDIA_VOLUME_MUTE);
  }
}
