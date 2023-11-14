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
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioInputAnalog adcL(A2);
AudioInputAnalog adcR(A3);
AudioMixer4 mixer1;
AudioAmplifier amp1;
AudioRecordQueue queue1;
AudioAnalyzeFFT256 fft256;
AudioConnection patchCord1(adcL, 0, mixer1, 0);
AudioConnection patchCord2(adcR, 0, mixer1, 1);
AudioConnection patchCord3(mixer1, amp1);
AudioConnection patchCord4(amp1, queue1);
AudioConnection patchCord5(amp1, fft256);

static int FFTarray[12];

void setup() {
  setupLED();
}

void loop() {

  if(fft256.available())
  {
    averageFFT();
  }

  // lineUpTest();
  // cycleAllColors(1000);
  // breatheStatic(3, 5);
  // showRandomColor(1000);
  // incrementFillToBlackStatic(2, 60);
  // incrementFillToBlackRandom(60);
  // incrementFillRandom(60);
  // singleColorWipeStatic(5, 60);
  // doubleColorWipe(1,3,60);
  fullColorWipe(60);

  // analogWrite(0, 255);   // pump 12
  // analogWrite(1, 255);   // pump 11
  // analogWrite(2, 255);   // pump 10
  // analogWrite(3, 255);   // pump 09
  // analogWrite(4, 255);   // pump 08
  // analogWrite(5, 255);   // pump 07
  // analogWrite(6, 255);   // pump 06
  // analogWrite(7, 255);   // pump 05
  // analogWrite(8, 255);   // pump 04
  // analogWrite(9, 255);   // pump 03
  // analogWrite(10, 255);  // pump 02
  // analogWrite(11, 255);  // pump 01
}

void averageFFT() //averages the 128 bins of fft down to 12 and places the values in FFTarray[]
{
  for(int i=0; i<12; i++)
  {
    if(i==0) //first bin averages over 14
    {
      FFTarray[i]=(fft256.read(0)+fft256.read(1)+fft256.read(2)+fft256.read(3)+
               fft256.read(4)+fft256.read(5)+fft256.read(6)+fft256.read(7)+
               fft256.read(8)+fft256.read(9)+fft256.read(10)+fft256.read(11)+
               fft256.read(12)+fft256.read(13))/14;
    }
    else if(i==11) //last bin averages over 14
    {
      FFTarray[i]=(fft256.read(114)+fft256.read(115)+fft256.read(116)+fft256.read(117)+
                fft256.read(118)+fft256.read(119)+fft256.read(120)+fft256.read(121)+
                fft256.read(122)+fft256.read(123)+fft256.read(124)+fft256.read(125)+
                fft256.read(126)+fft256.read(127))/14;
    }
    else //all other bins average over 10
    {
      FFTarray[i]=(fft256.read(1+((i-1)*10)+13)+fft256.read(2+((i-1)*10)+13)+
                fft256.read(3+((i-1)*10)+13)+fft256.read(4+((i-1)*10)+13)+
                fft256.read(5+((i-1)*10)+13)+fft256.read(6+((i-1)*10)+13)+
                fft256.read(7+((i-1)*10)+13)+fft256.read(8+((i-1)*10)+13)+
                fft256.read(9+((i-1)*10)+13)+fft256.read(10+((i-1)*10)+13))/10;
    }
  }
}