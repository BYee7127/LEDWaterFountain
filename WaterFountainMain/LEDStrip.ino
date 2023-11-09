/**************************************************************************************
 *
 * File:      LEDStrip.ino
 * Author:    Alex Gray & Beverly Yee
 * Date:      August 27, 2023
 * 
 **************************************************************************************/
#include <FastLED.h>

// ================ Teensy 4.0 Stuff ================
// check the list of FastLED Parallel Output pins to get the correct one to use
#define DATA_PIN 7
#define NUM_LEDS_PER_STRIP 14
#define NUM_STRIPS 1
#define NUM_LEDS NUM_LEDS_PER_STRIP

// declare the array of LEDs
CRGB leds[NUM_LEDS_PER_STRIP * NUM_STRIPS];

// array of colors to use on the strip
CRGB clist[10] = { CRGB::Red, CRGB::Lime, CRGB::Magenta, CRGB::Blue, CRGB::OrangeRed, CRGB::ForestGreen, CRGB::Crimson, CRGB::Gold, CRGB::Cyan, CRGB::White };

// get the length of the color list
int length = sizeof(clist) / sizeof(clist[0]);

CRGB previousColor;
CRGB clear = CRGB::Black;

/*
 * Setup the LED strip to be called by WaterFountainMain.setup();
 */
void setupLED() {
  // set previousColor to black
  previousColor = CRGB::Black;

  // add the LED strip
  FastLED.addLeds<NUM_STRIPS, WS2811, DATA_PIN, GRB>(leds, NUM_LEDS_PER_STRIP);

  // initially set it to black
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

/*
 * Assign each LED with a different color from clist.
 * A test to see how the LEDs are addressable and if they all work.
 * 
 * params:    none
 */
void lineUpTest() {
  // int c = 0;
  // for (int i = 0; i < NUM_LEDS; i++) {
  //   // since the length of the color list is less than the number of LEDs we have
  //   if (c >= length) {
  //     c = 0;
  //   }

  //   leds[i] = clist[c];
  //   c++;
  // }

  leds[0] = CRGB::Red;
  leds[1] = CRGB::Lime;
  leds[2] = CRGB::Magenta;
  leds[3] = CRGB::Blue;
  leds[4] = CRGB::OrangeRed;
  leds[5] = CRGB::ForestGreen;
  leds[6] = CRGB::Crimson;
  leds[7] = CRGB::Gold;
  leds[8] = CRGB::Cyan;
  leds[9] = CRGB::White;
  leds[10] = CRGB::Red;
  leds[11] = CRGB::Lime;
  leds[12] = CRGB::Magenta;
  leds[13] = CRGB::Blue;

  FastLED.show();
}

/*
 * Iterate through clist and fill the entire LED strip with that color.
 * 
 * params:    wait - how long to delay function before continuing
 */
void cycleAllColors(uint16_t wait) {
  for (int l = 0; l < length; l++) {
    fill_solid(leds, NUM_LEDS, clist[l]);

    FastLED.show();
    FastLED.delay(wait);
  }
}

/*
 * Fade in/out the entire strip with a single, hardcoded color.
 * Stretch goal - take this as an input from the user.
 * 
 * params:    index - number attached to a color in clist
 *            wait - how long to delay the function before continuing
 */
void breatheStatic(int index, int wait) {
  for (uint8_t b = 0; b < 255; b++) {
    fill_solid(leds, NUM_LEDS, clist[index]);
    FastLED.setBrightness(b);

    FastLED.show();
    FastLED.delay(wait);
  };

  for (uint8_t b = 255; b > 0; b--) {
    fill_solid(leds, NUM_LEDS, clist[index]);
    FastLED.setBrightness(b);

    FastLED.show();
    FastLED.delay(wait);
  };
}

/*
 * Only do increments/decrements of ODD numbers.
 * Even numbers causes a weird flicker.
 */
void breatheByIncrement(int index, int wait){
  for (uint8_t b = 0; b < 255; b++) {
    fill_solid(leds, NUM_LEDS, clist[index]);
    FastLED.setBrightness(b);

    FastLED.show();
    FastLED.delay(wait);
  };

  for (uint8_t b = 255; b > 0; b--) {
    fill_solid(leds, NUM_LEDS, clist[index]);
    FastLED.setBrightness(b);

    FastLED.show();
    FastLED.delay(wait);
  };
}

/*
 * Every call to this function lights the strip in a different color.
 * But the next color is different from the previous color, if it has
 * been called previously.
 * 
 */
void showRandomColor(int wait) {
  // grab a random color
  CRGB current = clist[rand() % 10];

  // check if the current color is the same as the previous color
  if (current == previousColor) {
    current = clist[rand() % 10];
  }

  fill_solid(leds, NUM_LEDS, current);
  FastLED.show();
  FastLED.delay(wait);

  // now set previous to current
  previousColor = current;
}

/*
 * Incrementally fill the LED strip with a color then black
 */
void incrementFillToBlackStatic(int index, int wait) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = clist[index];
    FastLED.show();
    FastLED.delay(wait);
  }

  // once the strip is filled with color, go to black
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = clear;
    FastLED.show();
    FastLED.delay(wait);
  }
}

/*
 * Incrementally fill the LED strip with a color then black
 * Every call to this function has a different color
 */
void incrementFillToBlackRandom(int wait) {
  // grab a random color
  CRGB current = clist[rand() % 10];

  // check if the current color is the same as the previous color
  if (current == previousColor) {
    current = clist[rand() % 10];
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = current;
    FastLED.show();
    FastLED.delay(wait);
  }

  // once the strip is filled with color, go to black
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = clear;
    FastLED.show();
    FastLED.delay(wait);
  }

  // now set previous to current
  previousColor = current;
}

/*
 * Each incremental LED is a different, random color.
 * Does not fill to black afterwards.
 */
void incrementFillRandom(int wait) {
  // initialize just in case
  CRGB current = clear;
  CRGB prev = clear;

  for (int i = 0; i < NUM_LEDS; i++) {
    current = clist[rand() % 10];
    // make sure the next color is indeed a different color
    if (current == prev) {
      current = clist[rand() % 10];
    }

    leds[i] = current;
    FastLED.delay(wait);

    prev = current;
  }
}

void singleColorWipeStatic(int index, int wait) {
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = clist[index];
    FastLED.delay(wait);
    leds[i] = clear;
    FastLED.show();
  }
}

