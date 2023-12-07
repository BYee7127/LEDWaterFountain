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
int chaseDelay, chaseTotal, chaseCount, chaseCount2, step, initialColor;

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

  // connect to an unconnected pin to reset random function
  randomSeed(analogRead(16));
}

void setupDelays() {
  lineUpDelay = 2000;

  chaseDelay = 50;
  chaseTotal = 1500;
  chaseCount = 0;
  chaseCount = 0;
  initialColor=0;
  step = 0;
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
}

/*
 *
 */
void cycleAllPatterns() {
  if (counter <= lineUpDelay) {
    lineUpTest();
  }

  // cycle all colors
  else if (lineUpDelay < counter && counter <= showSingleEnd) {
    int currentTime = showSingleEnd - counter;

    if (0 < currentTime && currentTime < showSingleDelay * 1) {
      fill_solid(leds, NUM_LEDS, clist[0]);
    } else if (showSingleDelay * 1 < currentTime && currentTime <= showSingleDelay * 2) {
      fill_solid(leds, NUM_LEDS, clist[1]);
    } else if (showSingleDelay * 2 < currentTime && currentTime <= showSingleDelay * 3) {
      fill_solid(leds, NUM_LEDS, clist[2]);
    } else if (showSingleDelay * 3 < currentTime && currentTime <= showSingleDelay * 4) {
      fill_solid(leds, NUM_LEDS, clist[3]);
    } else if (showSingleDelay * 4 < currentTime && currentTime <= showSingleDelay * 5) {
      fill_solid(leds, NUM_LEDS, clist[4]);
    } else if (showSingleDelay * 5 < currentTime && currentTime <= showSingleDelay * 6) {
      fill_solid(leds, NUM_LEDS, clist[5]);
    } else if (showSingleDelay * 6 < currentTime && currentTime <= showSingleDelay * 7) {
      fill_solid(leds, NUM_LEDS, clist[6]);
    } else if (showSingleDelay * 7 < currentTime && currentTime <= showSingleDelay * 8) {
      fill_solid(leds, NUM_LEDS, clist[7]);
    } else if (showSingleDelay * 8 < currentTime && currentTime <= showSingleDelay * 9) {
      fill_solid(leds, NUM_LEDS, clist[8]);
    } else if (showSingleDelay * 9 < currentTime && currentTime <= showSingleEnd) {
      fill_solid(leds, NUM_LEDS, clist[9]);
    }
    FastLED.show();
  }

  // fade in/out single color
  else if (showSingleEnd < counter && counter <= breatheStaticInEnd) {
    // fades in to color
    int curCount = counter - showSingleEnd;

    fill_solid(leds, NUM_LEDS, clist[4]);
    FastLED.setBrightness(curCount / bswait);
    FastLED.show();
  } else if (breatheStaticInEnd < counter && counter <= breatheStaticOutEnd) {
    int curCount = breatheStaticOutEnd - counter;

    fill_solid(leds, NUM_LEDS, clist[4]);
    FastLED.setBrightness(curCount / bswait);
    FastLED.show();
    FastLED.setBrightness(255);
  }

  // fade in/out random color
  else if (breatheStaticOutEnd < counter && counter <= breatheRandomInEnd) {
    // fades in to color
    int curCount = counter - breatheStaticOutEnd;
    if (counter == breatheStaticOutEnd + 1) {
      getRandomColor();
    }

    fill_solid(leds, NUM_LEDS, current);
    FastLED.setBrightness(curCount / bswait);
    FastLED.show();
  } else if (breatheRandomInEnd < counter && counter <= breatheRandomOutEnd) {
    int curCount = breatheRandomOutEnd - counter;

    fill_solid(leds, NUM_LEDS, current);
    FastLED.setBrightness(curCount / bswait);
    FastLED.show();
    FastLED.setBrightness(255);
  }

  // show random color
  else if (breatheRandomOutEnd < counter && counter < showRandSingleEnd) {
    if (showRandomSingleCount == 0) {
      getRandomColor();
    }

    showRandomSingleCount++;
    fill_solid(leds, NUM_LEDS, current);
    FastLED.show();

    if (showRandomSingleCount == showSingleDelay) {
      showRandomSingleCount = 0;
    }
  }

  else if(counter == showRandSingleEnd){
    fill_solid(leds, NUM_LEDS, clear);
    FastLED.show();
  }

  // increment fill to black
  else if (showRandSingleEnd < counter && counter <= fillStaticEnd) {
    
    if(fillStaticCount == fillStaticDelay - 1){
      fillStaticCount = 0;
      fillStaticLED++;
    }

    fillStaticCount++;
    leds[fillStaticLED] = clist[8];
    FastLED.show();
  }

  // reset the counter when all patterns are iterated through
  else {
    counter = 0;
    fill_solid(leds, NUM_LEDS, clear);
    FastLED.show();
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

/*************/
/*
 * Each call to this function shows a different color.
 * 
 * params:    wait - how long to delay the function before continuing
 */


/*
 * Every call to this function lights the strip in a different color.
 * 
 * params:    wait - how long to delay the function before continuing
 */

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

void chase(int index)
{
  if(chaseCount<=chaseTotal)
  {
    current = clist[index];

    if(step==0)
    {
      leds[0]=current;
      leds[1]=CRGB::Black;
      leds[2]=current;
      leds[3]=CRGB::Black;
      leds[4]=current;
      leds[5]=CRGB::Black;
      leds[6]=current;
      leds[7]=CRGB::Black;
      leds[8]=current;
      leds[9]=CRGB::Black;
      leds[10]=current;
      leds[11]=CRGB::Black;
      leds[12]=current;
      leds[13]=CRGB::Black;
    }
    else if(step==1)
    {
      leds[0]=CRGB::Black;
      leds[1]=current;
      leds[2]=CRGB::Black;
      leds[3]=current;
      leds[4]=CRGB::Black;
      leds[5]=current;
      leds[6]=CRGB::Black;
      leds[7]=current;
      leds[8]=CRGB::Black;
      leds[9]=current;
      leds[10]=CRGB::Black;
      leds[11]=current;
      leds[12]=CRGB::Black;
      leds[13]=current;
    }
    else
    {
      step=0;
    }

    if(chaseCount2 == chaseDelay)
    {
      chaseCount2=0;
      step++;
    }
    FastLED.show();
    FastLED.setBrightness(255);
  }
  else
  {
    chaseCount=0;
  }
  chaseCount++;
  chaseCount2++;
}

void chaseRandom()
{
  if(initialColor==0)
  {
    getRandomColor();
    initialColor++;
  }
  if(chaseCount<=chaseTotal)
  {
    if(step==0)
    {
      leds[0]=current;
      leds[1]=CRGB::Black;
      leds[2]=current;
      leds[3]=CRGB::Black;
      leds[4]=current;
      leds[5]=CRGB::Black;
      leds[6]=current;
      leds[7]=CRGB::Black;
      leds[8]=current;
      leds[9]=CRGB::Black;
      leds[10]=current;
      leds[11]=CRGB::Black;
      leds[12]=current;
      leds[13]=CRGB::Black;
    }
    else if(step==1)
    {
      leds[0]=CRGB::Black;
      leds[1]=current;
      leds[2]=CRGB::Black;
      leds[3]=current;
      leds[4]=CRGB::Black;
      leds[5]=current;
      leds[6]=CRGB::Black;
      leds[7]=current;
      leds[8]=CRGB::Black;
      leds[9]=current;
      leds[10]=CRGB::Black;
      leds[11]=current;
      leds[12]=CRGB::Black;
      leds[13]=current;
    }
    else
    {
      step=0;
    }

    if(chaseCount2 == chaseDelay)
    {
      chaseCount2=0;
      step++;
    }
    FastLED.show();
    FastLED.setBrightness(255);
  }
  else
  {
    chaseCount=0;
    getRandomColor();
  }
  chaseCount++;
  chaseCount2++;
}
