/*
Remy Younes
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
 
LedControl lc=LedControl(12,11,10,1);

/* Scroll Delay */
unsigned long delaytime = 80;
//number of rows needed to represent a spacer on matrix 
unsigned static int const SPACING = 1;
//number of rows needed to represent a char on matrix  (5x8)
unsigned static int const CLEN = 5;
unsigned static int const LCD_WIDTH = 8;
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


void setup() {
   //The MAX72XX is in power-saving mode on startup - > Wakeup call
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,5);
  /* and clear the display */
  lc.clearDisplay(0);
}

// Write a message on LedMatrix and Scroll it Right to left.
void writeOnMatrix(String message) {

  //add padding(optional): helps with scroll rotation
  message = "  " + message + "  ";
  
  //size of the banner
  int aSize = message.length() * (CLEN + SPACING);
  byte* banner = constructBanner(message);
  
  //print banner
  for( int i=0; i < aSize-(LCD_WIDTH); i++){
    //Update rows from i to i+j (8 rows)
    for(int j=0; j<LCD_WIDTH; j++){
      lc.setRow(0, j, banner[i+j]); 
    }
    //scroll delay
    delay(delaytime);
  }
  //deallocate, once we're done printing
  delete [] banner;
}

 byte* constructBanner(String s){
  int aSize =  s.length() * (CLEN + SPACING);  
  // banner is a side-by-side collection of the letters that spell out message
  // m allocation for banner
  byte* banner= new byte[aSize];
  int index, offset;
  char l;
  
  //insert letters in banner
  for( int i=0; i<s.length(); i++){
    offset = ( CLEN + SPACING) * i;
    l = tolower(s[i]);
    
    //if char is a-z, print on banner + SPACING
    if ( 'a' <= l && l <= 'z') {
      index = l - 'a';
    }else{
      //index 26 represents a space
      //show a space if unknown char
      index = 26;
    }
   
    // print each row of each char on banner
    for(int j=0; j<=CLEN; j++){
      if(j<CLEN){
        banner[offset+j] = CHARACTERS[index][j];
      }else{
        banner[offset+CLEN] = B00000000;
      }
    }
  }
  
  return banner;
}

void loop() { 
  writeOnMatrix("sbhackerspace");
  writeOnMatrix("fishbon");
}
