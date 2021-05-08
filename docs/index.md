## Hamish's Media Controller

Working on building a small media controller using a rotary encoder and key switches with plans to add rgb indicators. Currently working with a cardboard prototype running a Pro Micro.

#### Using the following packages in Arduino IDE 1.8.13:
 - SimpleRotary [github](https://github.com/mprograms/SimpleRotary)
 - HID-Project [arduino reference](https://www.arduino.cc/reference/en/libraries/hid-project/)
 - NeoPixel [arduino reference](https://www.arduino.cc/reference/en/libraries/adafruit-neopixel/)


# Blog
---
## First test
Using an arduino pro micro with a rotary encoder I started playing around with encoder libraries and the HID-Project for volume control. I settled on SimpleRotary for its ease of use and straightforewards debouncing. Pictured below is the micro connected to the rotary encoder on a breadboard.

![Prototype 1: Pro Micro and rotary encoder on breadboard](https://user-images.githubusercontent.com/67180838/117089971-e39e9b00-ad0b-11eb-9252-1346276d2482.jpg)
_Apr 30 2021_

---
## Prototype begins
With the basic concept started, I made a base and knob for my enocder out of cardboard with room for two pushbuttons for previous/next track (the end goal would be to replace these with keyswitches).

![Prototype 2: Rotary encoder and buttons on cardboard body, cardboard knob](https://user-images.githubusercontent.com/67180838/117092452-1bf5a780-ad13-11eb-8153-e8bd92e295fe.png)

With the controls set up and working to control volume up/down, mute, pause/play, and next/previous track, I worked on the LED lights. I chose some NeoPixel circles with 12 LED's as they were easily accesible and were the right size. The Adafruit_NeoPixel library let me initialize to a standby colour and react to controls being adjusted. I had originaly hoped to use it to indicate my computers volume, however, this would require setting up code/drivers on my computer to send this information to the micro istelf which I do not currently feel interested in doing.

![Figure: Showing Adafruit NeoPixel ring on base with and without knob](https://user-images.githubusercontent.com/67180838/117092735-f4eba580-ad13-11eb-8850-04c1f5bc5d86.png)

![Gif: Showing LED ring changing colour under various actions](https://user-images.githubusercontent.com/67180838/117092503-45163800-ad13-11eb-8a1e-498ab6e051d9.gif)

I also threw in a third button to the Micro's reset pin for debugging. There is currently a bug where after time has passed, the micro's communication to the computer slows down to only update about 5 times per second. Opening the serial monitor fixes this, and so does reseting the board.
_May 04 2021_

---
## Making the knob
My next step was to find a satisfying rotary knob of the size I was thinking of (no luck so far) or make my own. So I tried drawing up an idea in AutoCAD that I based on my cardboard prototype. I made the knob have an exterior radius of 26mm, and made sure to include a 6mm diameter hole in the middle. My goal is ultimately to have something made out of metal but for now 3D printing allows for rapid prototyping.

![Figure: First CAD drawing of rotary encoder knob. Gif: Printing of the first knob.](https://user-images.githubusercontent.com/67180838/117548645-e985cc00-afea-11eb-900d-598175e066d6.gif)

The printed knob's domensions were a little bit off so I used a 15/64 in drill to bring the center hole to the correct size and open the exterior hole wide enough to bring a screwdriver through. This worked very well as a temporary version but turning a screw through the plastic is not a permanent fix. Overall I found this model works very well, the white plastic walls allow more light than I was expecting through. In the meantime I updated some smother colour transitions, added highlights for skipping through tracks, and a green flash for pause and play.

![Figure: Photo of both prototype knobs side by side followed by gif showing the LED colours under various actions.](https://github.com/Ham0osh/MediaController/blob/69595dd127647da17686752bbb0cdb0f236b2aa4/docs/images/ezgif.com-gif-maker%20(6).gif)
_May 08 2021_

---
Hamish Johnson

May 2021
