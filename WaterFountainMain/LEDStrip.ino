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
#define DATA_PIN 20
#define NUM_LEDS_PER_STRIP 14
#define NUM_STRIPS 1
#define NUM_LEDS NUM_LEDS_PER_STRIP

// declare the array of LEDs
CRGB leds[NUM_LEDS_PER_STRIP * NUM_STRIPS];

// array of colors to use on the strip
CRGB clist[10] = { CRGB::Red, CRGB::Lime, CRGB::Magenta, CRGB::Blue, CRGB::OrangeRed, CRGB::ForestGreen, CRGB::Crimson, CRGB::Gold, CRGB::Cyan, CRGB::White };

int length;

CRGB previous, clear, current;

/*
 * Setup the LED strip to be called by WaterFountainMain.setup();
 */
void setupLED() {
  // get the length of the color list
  length = sizeof(clist) / sizeof(clist[0]);
  clear = CRGB::Black;
  previous = clear;

  // add the LED strip
  FastLED.addLeds<NUM_STRIPS, WS2811, DATA_PIN, GRB>(leds, NUM_LEDS_PER_STRIP);

  // initially set it to black
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void cycleAllPatterns() {
  lineUpTest();
  delay(3500);

  cycleAllColors(1000);
  delay(500);

  breatheStatic(1, 5);
  breatheStatic(1, 5);
  breatheStatic(1, 5);
  delay(1000);

  breatheIntoNextColor(5);
  breatheIntoNextColor(5);
  breatheIntoNextColor(5);
  breatheIntoNextColor(5);
  breatheIntoNextColor(5);
  delay(1000);
  FastLED.setBrightness(255);  // need this to show color after fade to black

  showRandomColor(1500);
  showRandomColor(1500);
  showRandomColor(1500);
  showRandomColor(1500);

  fill_solid(leds, NUM_LEDS, clear);
  FastLED.show();
  delay(1000);

  incrementFillToBlackStatic(2, 80);
  incrementFillToBlackStatic(2, 80);
  incrementFillToBlackStatic(2, 80);
  incrementFillToBlackStatic(2, 80);

  incrementFillToBlackRandom(80);
  incrementFillToBlackRandom(80);
  incrementFillToBlackRandom(80);
  incrementFillToBlackRandom(80);
  incrementFillToBlackRandom(80);

  incrementFillRandom(80);
  incrementFillRandom(80);
  incrementFillRandom(80);
  incrementFillRandom(80);
  incrementFillRandom(80);

  singleColorWipeStatic(3, 80);
  singleColorWipeStatic(3, 80);
  singleColorWipeStatic(3, 80);
  singleColorWipeStatic(3, 80);
  singleColorWipeStatic(3, 80);
}

/*
 * Assign each LED with a different color from clist.
 * A test to see how the LEDs are addressable and if they all work.
 * 
 * params:    none
 */
void lineUpTest() {
  int c = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    // since the length of the color list is less than the number of LEDs we have
    if (c >= length) {
      c = 0;
    }

    leds[i] = clist[c];
    c++;
  }

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
 * Each call to this function shows a different color.
 * 
 * params:    wait - how long to delay the function before continuing
 */
void breatheIntoNextColor(int wait) {
  getRandomColor();

  for (uint8_t b = 0; b < 255; b++) {
    fill_solid(leds, NUM_LEDS, current);
    FastLED.setBrightness(b);

    FastLED.show();
    FastLED.delay(wait);
  };

  for (uint8_t b = 255; b > 0; b--) {
    fill_solid(leds, NUM_LEDS, current);
    FastLED.setBrightness(b);

    FastLED.show();
    FastLED.delay(wait);
  };
}

/*
 * Every call to this function lights the strip in a different color.
 * 
 * params:    wait - how long to delay the function before continuing
 */
void showRandomColor(int wait) {
  getRandomColor();

  fill_solid(leds, NUM_LEDS, current);
  FastLED.show();
  FastLED.delay(wait);
}

/*
 * Incrementally fill the LED strip with a color then black
 * 
 * params:    index - number attached to a color in clist
 *            wait - how long to delay the function before continuing
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
 * 
 * params:    wait - how long to delay the function before continuing
 */
void incrementFillToBlackRandom(int wait) {
  getRandomColor();

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
}

/*
 * Each incremental LED is a different, random color.
 * Does not fill to black afterwards.
 * 
 * params:    wait - how long to delay the function before continuing
 */
void incrementFillRandom(int wait) {
  for (int i = 0; i < NUM_LEDS; i++) {
    getRandomColor();
    leds[i] = current;
    FastLED.delay(wait);
  }
}

/*
 * Only one LED is illuminated the entire time and runs across the strip.
 * 
 * params:    index - number attached to a color in clist
 *            wait - how long to delay the function before continuing
 */
void singleColorWipeStatic(int index, int wait) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = clist[index];
    FastLED.delay(wait);
    leds[i] = clear;
    FastLED.show();
  }
}

/*
 * Only two LEDs is illuminated the entire time and runs across the strip.
 * 
 * params:    index1 - number attached to a color in clist
              index2 - number attached to a color in clist, follows index1
 *            wait - how long to delay the function before continuing
 */
void doubleColorWipe(int index1, int index2, int wait) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = clist[index1];
    leds[i - 1] = clist[index2];
    FastLED.delay(wait);
    leds[i - 1] = clear;
    leds[i] = clear;
    FastLED.show();
  }
}

void fullColorWipe(int wait) {
  int c = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    for (int j = NUM_LEDS; j > 0; j--) {
      leds[j-i] = clist[c];
      if (c >= length) {
        c = 9;
      }
      c++;
    }

    FastLED.show();
    FastLED.delay(wait);
  }

  // for (int i = 0; i < NUM_LEDS; i++) {
  //   for (int j = length - 1; j >= 0; j++) {
  //     leds[i - j] = CRGB::White;
  //   }
  //   FastLED.show();
  // }
}

/*
 * Helper function to grab a random color then determine whether it is the same as
 * the previously generated color. While it is still the same, keep randomizing 
 * until the current color is different than the previous.
 * 
 * params:    none
 */
void getRandomColor() {
  current = clist[rand() % length];

  // change this into a while loop in case more than 2 in a row are the same numbers?
  if (current == previous) {
    // grab another random color if it ends up being the same as the previous color
    current = clist[rand() % length];
  }

  // set the previous color to the current color so the next call will have a different color
  // regardless of where it is called from
  previous = current;
}

/*
 * Only do increments/decrements of ODD numbers.
 * Even numbers causes a weird flicker.
 */
void breatheByIncrement(int index, int wait, int steps) {
  for (uint8_t b = 0; b < 255; b += steps) {
    fill_solid(leds, NUM_LEDS, clist[index]);
    FastLED.setBrightness(b);

    FastLED.show();
    FastLED.delay(wait);
  };

  for (uint8_t b = 255; b > 0; b -= steps) {
    fill_solid(leds, NUM_LEDS, clist[index]);
    FastLED.setBrightness(b);

    FastLED.show();
    FastLED.delay(wait);
  };
}
