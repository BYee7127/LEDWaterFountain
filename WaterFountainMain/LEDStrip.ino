/**************************************************************************************
 *
 * File:      LEDStrip.ino
 * Author:    Alex Gray & Beverly Yee
 * Date:      August 27, 2023
 * 
 * Anything and everything to do with the LED strip.
 *
 **************************************************************************************/
#include <FastLED.h>

// ================ Teensy 4.0 Stuff ================
// check the list of FastLED Parallel Output pins to get the correct one to use
#define DATA_PIN 20
#define NUM_LEDS_PER_STRIP 14
#define NUM_STRIPS 1
#define NUM_LEDS NUM_LEDS_PER_STRIP
#define STEPS        300 

// declare the array of LEDs
CRGB leds[NUM_LEDS_PER_STRIP * NUM_STRIPS];

// array of colors to use on the strip
CRGB clist[10] = { CRGB::Red, CRGB::Lime, CRGB::Magenta, CRGB::Blue, CRGB::OrangeRed,
                   CRGB::ForestGreen, CRGB::Crimson, CRGB::Gold, CRGB::Cyan, CRGB::White };

// length of the color array
int arrLength;
// variables for ease of access on random colors and clearing
CRGB previous, clear, current;

// ================ Delay counters ================
int counter;

int lineUpDelay;
int cycleColorsDelay, cycleColorsLength;
int fadeLength, breatheStaticInEnd, fadeDelay, breatheStaticOutEnd;
int breatheRandomInEnd, breatheRandomOutEnd;
int showRandomSingleCount, showRandSingleEnd;
int fillStaticDelay, fillStaticEnd, fillStaticCount, fillStaticLED;

int patternEnd, patternStart;

int delayCount, colorNum, LEDNum;

bool fadeOutYN, showColor;
// chase variables
int chaseDelay, chaseTotal, chaseCount, chaseCount2, step, initialColor;

// cycle variables
int cycleDelay, cycleCount;


/*
 * Setup the LED strip to be called by WaterFountainMain.setup();
 */
void setupLED() {
  Serial.begin(9600);
  // get the arrLength of the color list
  arrLength = sizeof(clist) / sizeof(clist[0]);
  clear = CRGB::Black;
  previous = clear;

  // add the LED strip
  FastLED.addLeds<NUM_STRIPS, WS2811, DATA_PIN, GRB>(leds, NUM_LEDS_PER_STRIP);

  // initially set it to black
  clearStrip();

  counter = 0;
  setupDelays();

  // connect to an unconnected pin to reset random function
  randomSeed(analogRead(16));

  showColor = true;
}

/*
 * 
 */
void setupDelays() {
  patternStart = 0;
  patternEnd = 0;

  lineUpDelay = 2000;

  cycleColorsDelay = 600;
  cycleColorsLength = cycleColorsDelay * arrLength;

  cycleDelay=100;

  fadeDelay = 6;
  fadeLength = fadeDelay * 255;  // 255 = max brightness

  chaseDelay = 50;
  chaseTotal = 500;
  chaseCount = 0;
  chaseCount2 = 0;
  initialColor = 0;
  step = 0;



  breatheStaticInEnd = cycleColorsLength + fadeLength;
  breatheStaticOutEnd = breatheStaticInEnd + fadeLength;

  breatheRandomInEnd = breatheStaticOutEnd + fadeLength;
  breatheRandomOutEnd = breatheRandomInEnd + fadeLength;

  showRandSingleEnd = (cycleColorsDelay * 4) + breatheRandomOutEnd;
  showRandomSingleCount = 0;

  fillStaticDelay = 60;
  fillStaticCount = 0;
  fillStaticLED = 0;
  fillStaticEnd = showRandSingleEnd + (fillStaticDelay * NUM_LEDS);

  delayCount = 0;
  LEDNum = 0;
  colorNum = 0;
}

/*
 *
 */
void cycleAllPatterns() {
  if (counter < lineUpDelay) {
    lineUpTest();
  }
  // lineUpTest -> cycleAllColors setup
  else if (counter == lineUpDelay) {
    patternStart += lineUpDelay;
    patternEnd += lineUpDelay + cycleColorsLength;
  }
  // cycle through all colors
  else if (patternStart < counter && counter < patternEnd) {
    cycleAllColors(cycleColorsDelay);
  }
  // cycleAllColors -> fadeInOut
  else if (counter == patternEnd) {
    patternStart += patternEnd;
    patternEnd += fadeLength;
  }
  // fade in/out random colors
  else if (patternStart < counter && counter < patternEnd) {
    fadeInOut(fadeDelay);
  }



  // // fade in/out single color
  // else if (cycleColorsLength < counter && counter <= breatheStaticInEnd) {
  //   // fades in to color
  //   int curCount = counter - cycleColorsLength;

  //   fill_solid(leds, NUM_LEDS, clist[4]);
  //   FastLED.setBrightness(curCount / fadeDelay);
  //   FastLED.show();
  // } else if (breatheStaticInEnd < counter && counter <= breatheStaticOutEnd) {
  //   int curCount = breatheStaticOutEnd - counter;

  //   fill_solid(leds, NUM_LEDS, clist[4]);
  //   FastLED.setBrightness(curCount / fadeDelay);
  //   FastLED.show();
  //   FastLED.setBrightness(255);
  // }

  // // fade in/out random color
  // else if (breatheStaticOutEnd < counter && counter <= breatheRandomInEnd) {
  //   // fades in to color
  //   int curCount = counter - breatheStaticOutEnd;
  //   if (counter == breatheStaticOutEnd + 1) {
  //     getRandomColor();
  //   }

  //   fill_solid(leds, NUM_LEDS, current);
  //   FastLED.setBrightness(curCount / fadeDelay);
  //   FastLED.show();
  // } else if (breatheRandomInEnd < counter && counter <= breatheRandomOutEnd) {
  //   int curCount = breatheRandomOutEnd - counter;

  //   fill_solid(leds, NUM_LEDS, current);
  //   FastLED.setBrightness(curCount / fadeDelay);
  //   FastLED.show();
  //   FastLED.setBrightness(255);
  // }

  // // show random color
  // else if (breatheRandomOutEnd < counter && counter < showRandSingleEnd) {
  //   if (showRandomSingleCount == 0) {
  //     getRandomColor();
  //   }

  //   showRandomSingleCount++;
  //   fill_solid(leds, NUM_LEDS, current);
  //   FastLED.show();

  //   if (showRandomSingleCount == cycleColorsDelay) {
  //     showRandomSingleCount = 0;
  //   }
  // }

  // else if (counter == showRandSingleEnd) {
  //   fill_solid(leds, NUM_LEDS, clear);
  //   FastLED.show();
  // }

  // // increment fill to black
  // else if (showRandSingleEnd < counter && counter <= fillStaticEnd) {

  //   if (fillStaticCount == fillStaticDelay - 1) {
  //     fillStaticCount = 0;
  //     fillStaticLED++;
  //   }

  //   fillStaticCount++;
  //   leds[fillStaticLED] = clist[8];
  //   FastLED.show();
  // }

  // reset the counter when all patterns are iterated through
  else {
    counter = 0;
    patternEnd = 0;
    patternStart = 0;
    clearStrip();
  }

  // increase the count with each iteration of the main loop
  counter++;
}

/*
 * Assign each LED with a different color from clist.
 * A test to see how the LEDs are addressable and if they all work.
 */
void lineUpTest() {
  int c = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    // since the arrLength of the color list is less than the number of LEDs we have
    if (c >= arrLength) {
      c = 0;
    }

    leds[i] = clist[c];
    c++;
  }

  FastLED.show();
}

/*************/
/*
 * Cycles through all the colors of the color array
 *
 * params:  wait - how long the counter iterates before switching colors
 *                 Simulates a delay
 */
void cycleAllColors(int wait) {
  if (delayCount == wait) {
    // reset delay count when the delay is met
    delayCount = 0;
    // update the index to the color array
    colorNum++;
  }

  if (colorNum == arrLength) {
    // reset colorNum to the beginning of the array
    colorNum = 0;
  }

  fill_solid(leds, NUM_LEDS, clist[colorNum]);
  FastLED.show();
  delayCount++;
}

/*
 * Fade into color and fade out into black. When fading back into color,
 * change to a different color, chosen at random.
 *
 * params:  wait - the time between adjustments of brightness level
 */
void fadeInOut(int wait) {
  // the arrLength of half of the pattern
  int bsDelay = wait * 255;

  // utilize a boolean to determine whether to fade in or out
  if (delayCount == bsDelay) {
    // start fade out when hitting the peak of pattern
    fadeOutYN = true;
  } else if (delayCount == 0) {
    // flip the boolean to fade back into color
    fadeOutYN = false;
    // swap the color
    getRandomColor();
  }

  if (fadeOutYN) {
    // fading out, so decrement
    delayCount--;
  } else {
    delayCount++;
  }

  fill_solid(leds, NUM_LEDS, current);

  // delayCount is going to be higher than 255, so divide by wait
  FastLED.setBrightness(delayCount / wait);
  FastLED.show();
}

/*
 * Fade into color and fade out into black.
 * Override of the random color version for single color only.
 *
 * params:  wait - the time between adjustments of brightness level
 *          index - the number attached to the color in the array
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
 * Fill the entire strip with a singluar color.
 * After "wait" is met, use a different color.
 *
 * params:  wait - the time between the next color
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

/*
 * Fill the entire strip with a singular color, passed in as an argument.
 *
 * params:  index - the number attached to the color in the array.
 */
void showSolid(int index) {
  fill_solid(leds, NUM_LEDS, clist[index]);
  FastLED.show();
}

/*
 * Incrementally fill the LED strip with a color.
 * The next time the strip fills, it will be a different color.
 * i.e., after the LED is filled with a color, it doesn't clear.
 * 
 * params:    wait - how long to delay the function before continuing
 */
void incrementColorFill(int wait) {
  // the time between lighting the previous LED to lighting the next LED
  int fillDelay = wait * NUM_LEDS;

  if (delayCount == fillDelay) {
    delayCount = 0;
  } else if (delayCount == 0) {
    // change the color at the start of the new pattern
    // this also allows a random color at startup
    getRandomColor();
  }

  // the delayCount works on the index of the strip
  leds[delayCount / wait] = clist[current];
  FastLED.show();

  delayCount++;
}

/*
 * Incrementally fill the LED strip with a color then increment fill to black.
 * When the pattern executes again, it will be a different color.
 * 
 * params:    index - number attached to a color in clist
 *            wait - how long to delay the function before continuing
 */
void incrementFillToBlack(int wait) {
  // the time between lighting the previous LED to lighting the next LED
  int fillDelay = wait * NUM_LEDS;

  if (delayCount == fillDelay) {
    // flip the bool to start the other part of the pattern
    showColor = !showColor;
    getRandomColor();
    delayCount = 0;
  }

  // show the color
  if (showColor) {
    // the delayCount works on the index of the strip
    leds[delayCount / wait] = current;
    FastLED.show();
  } else {
    // when showColor == false, clear it
    leds[delayCount / wait] = clear;
    FastLED.show();
  }

  delayCount++;
}

/*
 * Incrementally fill the LED strip with a color then increment fill to black.
 * 
 * params:    wait - how long to delay the function before continuing
 *            index - number attached to the color in the array
 */
void incrementFillToBlack(int wait, int index) {
  // the time between lighting the previous LED to lighting the next LED
  int fillDelay = wait * NUM_LEDS;

  if (delayCount == fillDelay) {
    // flip the bool to start the other part of the pattern
    showColor = !showColor;
    delayCount = 0;
  }

  // show the color
  if (showColor) {
    // the delayCount works on the index of the strip
    leds[delayCount / wait] = clist[index];
    FastLED.show();
  } else {
    // when showColor == false, clear it
    leds[delayCount / wait] = clear;
    FastLED.show();
  }

  delayCount++;
}

/*
 * Only one LED is illuminated the entire time and runs across the strip.
 * 
 * params:    index - number attached to a color in clist
 *            wait - how long to delay the function before continuing
 */
void singleColorWipe(int index, int wait) {

  if (delayCount == wait) {
    // delay met, turn off the current LED
    leds[LEDNum] = clear;
    FastLED.show();
    // and prepare to light the next one
    LEDNum++;
    // and reset the delayCount
    delayCount = 0;
  }

  if (LEDNum == NUM_LEDS) {
    LEDNum = 0;  // reset the index as to not go out of bounds
  }

  // make this an else statement?
  leds[LEDNum] = clist[index];
  FastLED.show();

  delayCount++;
}

/*
 * Only one LED is illuminated the entire time and runs across the strip.
 * 
 * params:    wait - how long to delay the function before continuing
 */
void singleColorWipe(int wait) {

  if (delayCount == wait) {
    // delay met, turn off the current LED
    leds[LEDNum] = clear;
    FastLED.show();
    // and prepare to light the next one
    LEDNum++;
    // and reset the delayCount
    delayCount = 0;
  }

  if (LEDNum == NUM_LEDS) {
    LEDNum = 0;  // reset the index as to not go out of bounds
    getRandomColor();
  }

  // make this an else statement?
  leds[LEDNum] = current;
  FastLED.show();

  delayCount++;
}

/*
 * Fill the entire strip with black to clear the strip.
 */
void clearStrip() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

/*************/

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
      if (c >= arrLength) {
        c = 9;
      }
      c++;
    }

    FastLED.show();
    FastLED.delay(wait);
  }

  // for (int i = 0; i < NUM_LEDS; i++) {
  //   for (int j = arrLength - 1; j >= 0; j++) {
  //     leds[i - j] = CRGB::White;
  //   }
  //   FastLED.show();
  // }
}

/*
 * Helper function to grab a random color then determine whether it is the same as
 * the previously generated color. While it is still the same, keep randomizing 
 * until the current color is different than the previous.
 */
void getRandomColor() {
  current = clist[random(arrLength)];

  // change this into a while loop in case more than 2 in a row are the same numbers?
  if (current == previous) {
    // grab another random color if it ends up being the same as the previous color
    current = clist[random(arrLength)];
  }

  // set the previous color to the current color so the next call will have a different color
  // regardless of where it is called from
  previous = current;
}

void chase(int index) {
  if (chaseCount <= chaseTotal) {
    current = clist[index];

    if (step == 0) {
      leds[0] = current;
      leds[1] = CRGB::Black;
      leds[2] = current;
      leds[3] = CRGB::Black;
      leds[4] = current;
      leds[5] = CRGB::Black;
      leds[6] = current;
      leds[7] = CRGB::Black;
      leds[8] = current;
      leds[9] = CRGB::Black;
      leds[10] = current;
      leds[11] = CRGB::Black;
      leds[12] = current;
      leds[13] = CRGB::Black;
    } else if (step == 1) {
      leds[0] = CRGB::Black;
      leds[1] = current;
      leds[2] = CRGB::Black;
      leds[3] = current;
      leds[4] = CRGB::Black;
      leds[5] = current;
      leds[6] = CRGB::Black;
      leds[7] = current;
      leds[8] = CRGB::Black;
      leds[9] = current;
      leds[10] = CRGB::Black;
      leds[11] = current;
      leds[12] = CRGB::Black;
      leds[13] = current;
    }

    if (chaseCount2 == chaseDelay) {
      chaseCount2 = 0;
      if (step == 0) step = 1;
      else step = 0;
    }

    FastLED.show();
    FastLED.setBrightness(255);
  } else {
    chaseCount = 0;
    chaseCount2 = 0;
  }
  chaseCount++;
  chaseCount2++;
}

void chaseRandom() {
  if (initialColor == 0) {
    getRandomColor();
    initialColor++;
  }
  if (chaseCount <= chaseTotal) {
    if (step == 0) {
      leds[0] = current;
      leds[1] = CRGB::Black;
      leds[2] = current;
      leds[3] = CRGB::Black;
      leds[4] = current;
      leds[5] = CRGB::Black;
      leds[6] = current;
      leds[7] = CRGB::Black;
      leds[8] = current;
      leds[9] = CRGB::Black;
      leds[10] = current;
      leds[11] = CRGB::Black;
      leds[12] = current;
      leds[13] = CRGB::Black;
    } else if (step == 1) {
      leds[0] = CRGB::Black;
      leds[1] = current;
      leds[2] = CRGB::Black;
      leds[3] = current;
      leds[4] = CRGB::Black;
      leds[5] = current;
      leds[6] = CRGB::Black;
      leds[7] = current;
      leds[8] = CRGB::Black;
      leds[9] = current;
      leds[10] = CRGB::Black;
      leds[11] = current;
      leds[12] = CRGB::Black;
      leds[13] = current;
    }

    if (chaseCount2 == chaseDelay) {
      chaseCount2 = 0;
      if (step == 0) step = 1;
      else step = 0;
    }
    FastLED.show();
    FastLED.setBrightness(255);
  } else {
    chaseCount = 0;
    chaseCount2 = 0;
    getRandomColor();
  }
  chaseCount++;
  chaseCount2++;
}

void fillRainbowCycle( uint8_t colorIndex) {
  for( int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    leds[i] = ColorFromPalette(RainbowColors_p, colorIndex, 255, LINEARBLEND);
    colorIndex += STEPS;              
  }
  FastLED.show();
}

void fillCycle( uint8_t colorIndex) {
  for( int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    leds[i] = ColorFromPalette(RainbowColors_p, colorIndex, 255, LINEARBLEND);              
  }
  FastLED.show();
}