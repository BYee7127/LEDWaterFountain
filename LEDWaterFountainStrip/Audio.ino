# include  "fix_fft.h"

# define  DEBUG 0
# define  L_IN A1 // Arduino A0 audio input 
# define  R_IN A0 // Arduino A1 audio input

const  int Yres = 8 ;
const  int gain = 3 ;
float peaks [64];
char im [64]; 
char Ldata [64];
char Rdata [64];
char data [64];
char data_avgs [64];
int debugLoop;
int i;
int load;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  for (int i = 0; i <64; i ++) {// 64 bins = 32 bins of usable spectrum data
    Ldata [i] = (( analogRead (L_IN) / 4) - 128); // chose how to interpret the data from analog in
    Rdata [i] = (( analogRead (R_IN) / 4) - 128);
    im [i] = 0; // imaginary component
    data [i] = Ldata [i] + Rdata [i];
  }

  fix_fft (data, im, 6, 0);

}
