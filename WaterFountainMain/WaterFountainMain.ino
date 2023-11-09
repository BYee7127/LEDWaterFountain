/**************************************************************************************
 *
 * File:      WaterFountainMain.ino
 * Author:    Alex Gray & Beverly Yee
 * Date:      November 08, 2023
 * 
 * Calls to the different parts of the Water Fountain are made here. 
 * To add a sketch to be a part of the project, make sure the sketch is saved in the
 * Sketches folder of WaterFountainMain. Go to Sketch -> Add File..., and navigate to 
 * the sketch to add.
 *
 * DO NOT call setup() or loop() in any of the other sketches. If it is necessary to
 * do some setup, call it setup[object]() where [object] is the thing to setup.
 *        i.e: setupLEDStrip() or setupFFT()
 * 
 **************************************************************************************/

void setup() {
  setupLED();
}

void loop() {
  // lineUpTest();
  // cycleAllColors(1000);
  breatheStatic(4, 5);
}
