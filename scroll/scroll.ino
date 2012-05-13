#include <TM1638.h>
const int TM168_STR = 9; /* brown  */
const int TM168_DAT = 8; /* orange */
const int TM168_CLK = 7; /* green  */
const int IR_DAT = 2;    /* brown  */
const int RLY_DAT = 4;   /* yellow */

//TM1638(byte dataPin, byte clockPin, byte strobePin, boolean activateDisplay, byte intensity);
// 9=brown, 8=orange, 7=green 
TM1638 module(TM168_DAT, TM168_STR , TM168_CLK, true, 4);
// counters to make things slower...
int count, wait, pos, color, blinkTimer=0;
String str="LENkA";
boolean start=false;
//LED colors
int colors[4]={
  0xFF, 0xFF00, 0xFFFF, 0x0000};
boolean turn=false;
int lumi = 5;

// function to clear the line before pos
// default=8 clears the whole line
void clearLine(unsigned int pos=8)
{
  for(int i=0; i<pos; i++) {
    module.setDisplayToString( " ", 0, i);
  }
}

int setIntensity(int lvl)
{
  if(lvl >= 7) lvl = 7;
  if(lvl <= 0) lvl = 0;
  module.setupDisplay(true, lvl);
  module.setDisplayToDecNumber(lvl,0,true); 
  delay(500);
  module.setDisplayToDecNumber(0,0,true);
  return lvl;
}

void setup() {
  // HARDWARE
  module.setDisplayToDecNumber(0,0,true);
  pinMode(IR_DAT, INPUT);
  pinMode(RLY_DAT, OUTPUT);
}

void loop() {
  // read the button value
  byte keys = module.getButtons();
  if (keys == 0x1 || digitalRead(IR_DAT) == LOW){
    start = !start; // start scrolling or pause
    delay(500);
  }
  if (keys == 0x2) { // stop scrolling
    start = false;
    clearLine();  
  }  
  // REGULATE light intensity
  if (keys == 0x8){ // increase intensity
    lumi++;
    lumi =  setIntensity( lumi );
  }
  if (keys == 0x4){ // decrease intensity
    lumi--;
    lumi = setIntensity( lumi );
  }

  if (start) {
    module.setLEDs((0x1 << pos) << (turn ? 8:0));
    wait++; 
    module.setDisplayToString( str, 0, pos);
    clearLine(pos);

    if ( wait==200 ) {// wait some time until update the position
      wait=0;
      pos++;
    }

    if (pos > 7) {// arrived to the end of the line
      pos = 0;
      clearLine();
      turn=!turn;
    }
  } 
  else { // blink LEDS until start button is pressed
    blinkTimer++;
    if (blinkTimer == 500){
      blinkTimer = 0;
      color++;
      if ( color > 2) color =0;
      module.setLEDs(0);
      module.setLEDs(colors[color]);
    }
  }
}





















