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

int length, counter;

int lineUpDelay;
int showSingleDelay, showSingleEnd;
int bsLength, breatheStaticInEnd, bswait, breatheStaticOutEnd;
int breatheRandomInEnd, breatheRandomOutEnd;
int showRandomSingleCount, showRandSingleEnd;
int fillStaticDelay, fillStaticEnd, fillStaticCount, fillStaticLED;

int prevInt, curInt;

int nextFuncStart;

int delayCount, LEDNum;
bool fadeOutYN, showColor;

CRGB previous, clear, current;

/*
 * Setup the LED strip to be called by WaterFountainMain.setup();
 */
void setupLED() {
  Serial.begin(9600);
  // get the length of the color list
  length = sizeof(clist) / sizeof(clist[0]);
  clear = CRGB::Black;
  previous = clear;

  // add the LED strip
  FastLED.addLeds<NUM_STRIPS, WS2811, DATA_PIN, GRB>(leds, NUM_LEDS_PER_STRIP);

  // initially set it to black
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  counter = 0;
  setupDelays();
  showColor = true;

  // connect to an unconnected pin to reset random function
  randomSeed(analogRead(16));
}

void setupDelays() {
  lineUpDelay = 2000;

  showSingleDelay = 800;
  showSingleEnd = lineUpDelay + (showSingleDelay * length);

  bswait = 6;
  bsLength = bswait * 255;
  breatheStaticInEnd = showSingleEnd + bsLength;
  breatheStaticOutEnd = breatheStaticInEnd + bsLength;

  breatheRandomInEnd = breatheStaticOutEnd + bsLength;
  breatheRandomOutEnd = breatheRandomInEnd + bsLength;

  showRandSingleEnd = (showSingleDelay * 4) + breatheRandomOutEnd;
  showRandomSingleCount = 0;

  fillStaticDelay = 60;
  fillStaticCount = 0;
  fillStaticLED = 0;
  fillStaticEnd = showRandSingleEnd + (fillStaticDelay * NUM_LEDS);

  delayCount = 0;
  LEDNum = 0;
}

/*
 *
 */
void cycleAllPatterns() {
  if (counter <= lineUpDelay) {
    lineUpTest();
  }

  else if (lineUpDelay < counter && counter <= showSingleEnd) {
    cycleAllColors(counter);
  }

  // fade in/out single color
  else if (showSingleEnd < counter && counter <= breatheStaticInEnd) {
    int curCount = counter - showSingleEnd;
    // fade into color
    fadeInOut(curCount, 4);
  } else if (breatheStaticInEnd < counter && counter < breatheStaticOutEnd) {
    int curCount = breatheStaticOutEnd - counter;
    // fade out to black
    fadeInOut(curCount, 4);
    // reset brightness after hitting 0 in fade
    FastLED.setBrightness(255);
  } else if (counter == breatheStaticOutEnd) {
    // set random color for next function
    getRandomIndex();
  }

  // fade in/out random color
  else if (breatheStaticOutEnd < counter && counter <= breatheRandomInEnd) {
    int curCount = counter - breatheStaticOutEnd;
    // fades in to color
    fadeInOut(curCount, curInt);
  } else if (breatheRandomInEnd < counter && counter <= breatheRandomOutEnd) {
    int curCount = breatheRandomOutEnd - counter;
    // fades out to black
    fadeInOut(curCount, curInt);
    // reset brightness after hitting 0 in fade
    FastLED.setBrightness(255);
  } else if (counter == breatheRandomOutEnd) {
    // set random color for next function
    getRandomIndex();
  }

  // show random color
  else if (breatheRandomOutEnd < counter && counter < showRandSingleEnd) {
    showSingleFill(curInt);
  } else if (counter == showRandSingleEnd) {
    clearStrip();
  }

  // increment fill to color
  else if (showRandSingleEnd < counter && counter <= fillStaticEnd) {
    int curCount = counter - showRandSingleEnd;
    int index = curCount / fillStaticDelay;

    leds[index] = clist[8];
    FastLED.show();
  } else if (fillStaticEnd < counter && counter <= (fillStaticEnd + (fillStaticDelay * NUM_LEDS))) {
    int curCount = counter - fillStaticEnd;
    int index = curCount / fillStaticDelay;

    leds[index] = clear;
    FastLED.show();
  }

  // reset the counter when all patterns are iterated through
  else {
    counter = 0;
  }

  // increase the count with each iteration of the main loop
  counter++;
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
 * Cycles through all the colors of the color array
 *
 * params: 
 */
void cycleAllColors(int wait) {
  if (delayCount == wait) {
    // reset delay count
    delayCount = 0;
    LEDNum++;
  }

  if (LEDNum == length) {
    // reset LEDNum to the beginning of the array
    LEDNum = 0;
  }

  fill_solid(leds, NUM_LEDS, clist[LEDNum]);
  FastLED.show();
  delayCount++;
}

/*
 * 
 */
void fadeInOut(int wait) {
  int bsDelay = wait * 255;
  if (delayCount == bsDelay) {
    fadeOutYN = true;
  } else if (delayCount == 0) {
    fadeOutYN = false;
    getRandomColor();
  }

  if (fadeOutYN) {
    // fading out, so decrement
    delayCount--;
  } else {
    delayCount++;
  }

  fill_solid(leds, NUM_LEDS, current);
  // fill_solid(leds, NUM_LEDS, clist[index]);
  FastLED.setBrightness(delayCount / wait);
  FastLED.show();
}

/*
 * 
 */
void fadeInOut(int wait, int index) {
  int bsDelay = wait * 255;
  if (delayCount == bsDelay) {
    fadeOutYN = true;
  } else if (delayCount == 0) {
    fadeOutYN = false;
  }

  if (fadeOutYN) {
    // fading out, so decrement
    delayCount--;
  } else {
    delayCount++;
  }

  fill_solid(leds, NUM_LEDS, clist[index]);
  FastLED.setBrightness(delayCount / wait);
  FastLED.show();
}

/*
 *
 */
void showSingleFill(int wait) {
  if (delayCount == wait) {
    delayCount = 0;
    getRandomColor();
  }

  fill_solid(leds, NUM_LEDS, current);
  FastLED.show();

  delayCount++;
}

void showSolid(int index) {
  fill_solid(leds, NUM_LEDS, clist[index]);
  FastLED.show();
}

/*
 * Fill the entire strip with black to clear the strip
 */
void clearStrip() {
  fill_solid(leds, NUM_LEDS, clear);
  FastLED.show();
}

/*
 * Incrementally fill the LED strip with a color then black
 * 
 * params:    wait - how long to delay the function before continuing
 */
void incrementFillToBlack(int wait) {
  int fillDelay = wait * NUM_LEDS;

  if (delayCount == fillDelay) {
    showColor = !showColor;
    delayCount = 0;
  }

  if (showColor) {
    leds[delayCount / wait] = clist[2];
    FastLED.show();
  }
  else {
    leds[delayCount / wait] = clear;
    FastLED.show();
  }

  delayCount++;
}

/*************/


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
      leds[j - i] = clist[c];
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
  current = clist[random(length)];

  // change this into a while loop in case more than 2 in a row are the same numbers?
  if (current == previous) {
    // grab another random color if it ends up being the same as the previous color
    current = clist[random(length)];
  }

  // set the previous color to the current color so the next call will have a different color
  // regardless of where it is called from
  previous = current;
}

void getRandomIndex() {
  curInt = random(length);

  if (curInt == prevInt) {
    curInt = random(length);
  }

  prevInt = curInt;
}
