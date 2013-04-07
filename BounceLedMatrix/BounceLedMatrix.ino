/*
Remy Younes - ryounes@gmail.com
Scrollable text on 8x8 LCD Matrix
*/

//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 */
 
//=======================
// INIT
//=======================
int sensorPin = A0; //pot
int sensorValue = 0;  // variable to store the value coming from the sensor
int btnPin = 6;
boolean lastButton = LOW;
boolean currentButton = LOW;

LedControl lc=LedControl(12,11,10,1);
/* Scroll Delay */
unsigned long delaytime = 40;
unsigned long bouncedelay = 25;
//number of rows needed to represent a spacer on matrix 
unsigned static int const SPACING = 1;
//number of rows needed to represent a char on matrix  (5x8)
unsigned static int const CLEN = 5;
//LCD WIDTH ( 8x8 )
unsigned static int const LCD_WIDTH = 8;
//size of current
unsigned static int bannerSize = 0;
int CURRENT_MODE = 0;

static int const ACCELERATION_VERTICAL = 0;
static int const ACCELERATION_HORIZONTAL = 1;
static int const VELOCITY_VERTICAL = 2;
static int const VELOCITY_HORIZONTAL = 3;
static int[4] = {
  ACCELERATION_VERTICAL,
  ACCELERATION_HORIZONTAL,
  VELOCITY_VERTICAL,
  VELOCITY_HORIZONTAL
}

//=======================
// CHARS
//=======================
// 5 ROWs wide text for letters A through Z
static byte CHARACTERS[][CLEN] = {
  { B00000010,B00010101,B00010101,B00010101,B00011111  }, //a (0)
  { B11111111,B00001001,B00010001,B00010001,B00001110  }, //b (1)
  { B00001110,B00010001,B00010001,B00010001,B00000010  }, //c
  { B00001110,B00010001,B00010001,B00001001,B11111111  }, //d
  { B00001110,B00010101,B00010101,B00010101,B00001100  }, //e
  { B00001000,B00111111,B01001000,B01000000,B00100000  }, //f
  { B00011000,B00100101,B00100101,B00100101,B00111110  }, //g
  { B01111111,B00001000,B00010000,B00010000,B00001111  }, //h
  { B00000000,B00100001,B10111111,B00000001,B00000000  }, //i
  { B00000010,B00000001,B00100001,B10111110,B00000000  }, //j
  { B01111111,B00000100,B00001010,B00010001,B00000000  }, //k
  { B00000000,B01000001,B01111111,B00000001,B00000000  }, //l
  { B00011111,B00010000,B00001100,B00010000,B00001111  }, //m
  { B00011111,B00001000,B00010000,B00010000,B00001111  }, //n
  { B00001110,B00010001,B00010001,B00010001,B00001110  }, //o
  { B00011111,B00010100,B00010100,B00010100,B00001000  }, //p
  { B00001000,B00010100,B00010100,B00001100,B00011111  }, //q
  { B00011111,B00001000,B00010000,B00010000,B00001000  }, //r
  { B00001001,B00010101,B00010101,B00010101,B00000010  }, //s
  { B00010000,B01111110,B00010001,B00000001,B00000010  }, //t
  { B00011110,B00000001,B00000001,B00000010,B00011111  }, //u
  { B00011100,B00000010,B00000001,B00000010,B00011100  }, //v
  { B00011110,B00000001,B00000010,B00000001,B00011110  }, //w
  { B00010001,B00001010,B00000100,B00001010,B00010001  }, //x
  { B00011000,B00000101,B00000101,B00000101,B00011110  }, //y
  { B00010001,B00010011,B00010101,B00011001,B00010001  }, //z (25)
  { B00000000,B00000000,B00000000,B00000000,B00000000  }, //space (26)
  
  //start of japanese katakana (not indexed yet)
  { B00101001,B00101001,B00101001,B00101001,B00111111  }, //yo (27)
  { B00111000,B00100000,B01100001,B00100010,B00111100  }, //u
  { B00100010,B00100010,B00100010,B00100010,B00111111  }, //ko
  { B00110000,B00001001,B00000001,B00000010,B00111100  }, //so 
  { B00100000,B00100100,B00100010,B00100101,B00111000  }, //ma 
  { B01111110,B00010001,B11010001,B00010001,B11010001  }, //bi
  { B00001000,B00001000,B00001000,B00001000,B00001000  }, //- 
  { B00101001,B00101001,B00000001,B00000010,B00001100  }, //shi 
  //ようこそ 
  //ヨウコソ
};

//=======================
// SETUP 
//=======================
void setup() {
   //The MAX72XX is in power-saving mode on startup - > Wakeup call
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,5);
  /* and clear the display */
  lc.clearDisplay(0);
  Serial.begin(9600);
}

//=======================
// PRINT ON LED MATRIX & SCROLL (R to L).
//=======================
void writeOnMatrix(String message) {

  //add padding(optional): helps with scroll rotation
  message = "  " + message + "  ";
  
  //size of the banner
  byte* banner = constructBanner(message);
  printBanner(banner);
  //deallocate, once we're done printing
  delete [] banner;
}


//=======================
//HELPERS METHODS
//=======================

//construct a printable banner out of each message character
byte* constructBanner(String s){
  //calculate bannerSize for memory alloc and loop
  bannerSize =  s.length() * (CLEN + SPACING);  
  // banner is a side-by-side collection of the letters that spell out message
  // m allocation for banner
  byte* banner = new byte[bannerSize];
  int index, offset;
  char c;
  
  //insert letters in banner
  for(int i=0; i<s.length(); i++){
    offset = (CLEN + SPACING) * i;
    c = tolower(s[i]);
    
    index = getCharIndex(c);
   
    // print each row of each char on banner
    for(int j=0; j<CLEN; j++){
      if(j<CLEN){
        banner[offset+j] = CHARACTERS[index][j];
      }
    }
    for(int k=0; k<SPACING; k++){
      banner[offset+CLEN+k] = B00000000; 
    }
  }
  
  return banner;
}

boolean debounce(boolean last){
  boolean current = digitalRead(btnPin);
  if(last != current){
    delay(5);
    current = digitalRead(btnPin);
  }
  return current;
}
  

//CONVERT CURRENT MESSAGE CHARACTER TO INDEX IN CHARACTER SET
int getCharIndex(char c){
  //index 26 represents a space (shown by default if unknown char)
  int index = 26;
  
  //if char is a-z
  if ( 'a' <= c && c <= 'z') {
    index = c - 'a';
  }
  return index;
}

//ITERATE THROUGH BANNER AND PRINT EACH ROW
void printBanner(byte* banner){
  for( int i=0; i < bannerSize-(LCD_WIDTH); i++){
    //Update rows from i to i+j (8 rows)
    for(int j=0; j<LCD_WIDTH; j++){
      lc.setRow(0, j, banner[i+j]); 
    }
    //scroll delay
    delay(delaytime);
  }
}

void changeMethod(){
  if(
  attr++ 
}

//=======================
// MAIN LOOP
//=======================
void loop() {
//  writeOnMatrix("MakineBounce");
  double x = 0, 
      y = 6;
  double  vx = 0.1,
          vy = 0.0,
          ax = 0,
          ay = -0.1;
      
  while(1){
    lc.setLed(0,floor(x),floor(y),false);
    
     
    if(x+vx>=LCD_WIDTH || x+vx < 0){
      vx*= -1;
    }
     
    if( y+vy>LCD_WIDTH || y+vy < 0){
      vy*= -1;
      y -= y*2;
    }
    
    x+=vx;
    y+=vy;    
    vx+=ax;
    vy+=ay;
    lc.setLed(0,floor(x),floor(y),true);
//    Serial.println("====");    
    sensorValue = analogRead(sensorPin);
    sensorValue = map(sensorValue, 0,1023, 0, 300);
    
    //btn
    lastButton = currentButton;
    currentButton = debounce(lastButton) ;
    
    //btn pushed
    if (lastButton == LOW && currentButton == HIGH ){
      changeMode();
    }  
    Serial.println(v);
    bouncedelay = sensorValue;
    delay(bouncedelay);
//    Serial.println(y);
//    Serial.println(vy);
//    Serial.println(ay);
  }
}
