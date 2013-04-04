//We always have to include the library
#include "LedControl.h"



/*
Remy Younes
Nick Winters
This program is based off the LedMatrixDemo example. 
It has been modified to allow for variable message.

 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=500;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}

// 5 column wide text for letters A through Z
static byte CHARACTERS[][5] = {
  { B01111111,B10001000,B10001000,B10001000,B01111111  },
  { B01111111,B01001001,B01001001,B00110110,B00000000  },
  { B01111110,B10000001,B10000001,B10000001,B01000010  },
  { B01111111,B01000001,B01000001,B00111110,B00000000  },
  { B11111110,B10010010,B10010010,B10010010,B00000000  },
  { B01111111,B01001000,B01001000,B01001000,B00000000  },
  { B01111110,B10000001,B10001001,B10001001,B01001110  },
  { B01111111,B00001000,B00001000,B00001000,B01111111  },
  { B00100001,B00100001,B00111111,B00100001,B00100001  },
  { B00000110,B00000001,B00000001,B00000001,B11111110  },
  { B11111111,B00011000,B00100100,B01000010,B10000001  },
  { B11111111,B00000001,B00000001,B00000001,B00000001  },
  { B11111100,B01000000,B00100000,B01000000,B11111100  },
  { B00111111,B00010000,B00001000,B00000010,B00111111  },
  { B01111110,B10000001,B10000001,B10000001,B01111110  },
  { B11111111,B10001000,B10001000,B10001000,B01110000  },
  { B01111100,B10000010,B10000010,B10000011,B01111101  },
  { B11111111,B10001000,B10001100,B10001010,B01110001  },
  { B00110001,B01001001,B01001001,B01001001,B01000110  },
  { B00100000,B00100000,B00111111,B00100000,B00100000  },
  { B00111110,B00000001,B00000001,B00000001,B00111110  },
  { B00111100,B00000010,B00000001,B00000010,B00111100  },
  { B00111111,B00000010,B00000100,B00000010,B00111111  },
  { B00010001,B00001010,B00000100,B00001010,B00010001  },
  { B11100000,B00010000,B00001111,B00010000,B11100000  },
  { B00010001,B00010011,B00010101,B00011001,B00010001  },
};

/*
 Displays the variable "message" on a LedMatrix
 */
void writeArduinoOnMatrix() {

  String message = "Fishbon";
  
  for( int i=0; i<message.length(); i++){
    char l = message[i];
    // replace lowercase with uppercase
    if ('a' <= l && l <= 'z') {
      l = l - ('a' - 'A');
    }
    if ('A' <= l && l <= 'Z') {
      int index = l - 'A';
      Serial.println(index);
      lc.setRow(0,0,CHARACTERS[index][0]);
      lc.setRow(0,1,CHARACTERS[index][1]);
      lc.setRow(0,2,CHARACTERS[index][2]);
      lc.setRow(0,3,CHARACTERS[index][3]);
      lc.setRow(0,4,CHARACTERS[index][4]);
    }
    delay(delaytime);
  }
}

void loop() { 
  writeArduinoOnMatrix();
}
