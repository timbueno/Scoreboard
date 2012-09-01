#include <Bounce.h>

#define BLUEBUTTON 7
#define REDBUTTON 6

int latchPin = 8; // ST_CP pin
int clockPin = 9; // SH_CP pin
int dataPin = 10; // DS pin

int n = 1;

//
// Make array of 7 seg digits
// Common anode means digital low (0) turns an led ON!
//
byte numArray[10] = {  0x11, // 0 dec 0001 0001 bin
                       0xD7, // 1 dec 1101 0111 bin
                       0x32, // 2 dec 0011 0010 bin
                       0x92, // 3 dec 1001 0010 bin
                       0xD4, // 4 dec 1101 0100 bin
                       0x98, // 5 dec 1001 1000 bin
                       0x08, // 6 dec 0000 1000 bin
                       0xD3, // 7 dec 1101 0011 bin
                       0x10, // 8 dec 0001 0000 bin
                       0x80  // 9 dec 1000 0000 bin
                     };

// Instantiate player's scores                    
int blueCount = 0;
int redCount = 0;

// Instantiate BLUE BUTTON object
Bounce blueButton = Bounce(BLUEBUTTON, 5);
int bbuttonState = 0;
int blastButtonState = 1;

// // Instantiate RED BUTTON object
// Bounce redButton = Bounce(REDBUTTON, 5);
// int rbuttonState = 0;
// int rlastButtonState = 1;

////////////////////////////////////////////////
// USER FUNCTIONS
//
//
//
// Refreshes each digit of the display
//
void updateScreen(int screen1, int screen2) {
    // Lookup LED in array
    byte blueScreen = numArray[screen1];
    byte redScreen = numArray[screen2];
    
    // Shift data into the two registers
    // digitalWrite(latchPin, 0); // LATCH LOW TO SHIFT
    // shiftOut(dataPin, clockPin, blueScreen);
    // shiftOut(dataPin, clockPin, redScreen);
    // digitalWrite(latchPin, 1); // ENABLE SHIFTED BITS

    Serial.print("Blue Screen:\n");
    Serial.print("\tbits: ");
    Serial.print(blueScreen, BIN);
    Serial.print("\n");
    Serial.print("\tNum: ");
    Serial.print(blueCount, DEC);
    Serial.print("\n");
    Serial.print("Red Screen:\n");
    Serial.print("\tbits: ");
    Serial.print(redScreen, BIN);
    Serial.print("\n");
    Serial.print("\tNum: ");
    Serial.print(redCount, DEC);
    Serial.print("\n");
    Serial.print("************************\n");
}

//
// Move entire byte into the Shift Register
//
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
    // This shifts 8 bits out MSB first, 
    //on the rising edge of the clock,
    //clock idles low

    //internal function setup
    int i=0;
    int pinState;
    pinMode(myClockPin, OUTPUT);
    pinMode(myDataPin, OUTPUT);

    //clear everything out just in case to
    //prepare shift register for bit shifting
    digitalWrite(myDataPin, 0);
    digitalWrite(myClockPin, 0);

    //for each bit in the byte myDataOut�
    //NOTICE THAT WE ARE COUNTING DOWN in our for loop
    //This means that %00000001 or "1" will go through such
    //that it will be pin Q0 that lights. 
    for (i=7; i>=0; i--)  {
      digitalWrite(myClockPin, 0);

      //if the value passed to myDataOut and a bitmask result 
      // true then... so if we are at i=6 and our value is
      // %11010100 it would the code compares it to %01000000 
      // and proceeds to set pinState to 1.
      if ( myDataOut & (1<<i) ) {
        pinState= 1;
      }
      else {  
        pinState= 0;
      }

      //Sets the pin to HIGH or LOW depending on pinState
      digitalWrite(myDataPin, pinState);
      //register shifts bits on upstroke of clock pin  
      digitalWrite(myClockPin, 1);
      //zero the data pin after shift to prevent bleed through
      digitalWrite(myDataPin, 0);
    }

    //stop shifting
    digitalWrite(myClockPin, 0);
}

////////////////////////////////////////////////////////
// MAIN EXECUTION THREAD
//
// This is a basic scoreboard sketch that implements
// two shift registers, two 7-Seg LED screens, and 
// two arcade button switches
//
//
//
// RUN STARTUP
//
void setup(){
    
    pinMode(BLUEBUTTON, INPUT); // Setup BLUE button
    // pinMode(REDBUTTON, INPUT);  // Setup RED button
    
    Serial.begin(9600); // Start serial output
}


//
// MAIN LOOP
//
//
void loop(){
    if(n == 1) {
      Serial.print("###                 ###\n");
      Serial.print("### SCORE-TRON 3000 ###\n");
      Serial.print("###                 ###\n");
      updateScreen(blueCount, redCount);
      n++; 
    }

//
// SERVICE BLUE PLAYER
//
    // Update the debouncer
    blueButton.update();
    // Get the updated value
    int bbuttonState = blueButton.read();
    
    // If button is actually pressed
    if (bbuttonState != blastButtonState) {
      if (bbuttonState == HIGH) {
        // Do work on press
        // Serial.print("LOW\n");
        blueCount++;
        if(blueCount == 10){
          //
          // SERVICE WINNER HERE
          //
          Serial.print("Winner! Winner! Winner!\n");
          blueCount = 0;
        }
        updateScreen(blueCount, redCount);
      }
    }
    // save the current button state
    blastButtonState = bbuttonState;
    
   
  // //SERVICE RED PLAYER
  
  //  // Update the debouncer
  //  rlueButton.update();
  //  // Get the updated value
  //  int rbuttonState = redButton.read();
   
  //  // If button is actually pressed
  //  if (rbuttonState != rlastButtonState) {
  //    if (rbuttonState == HIGH) {
  //      // Do work on press
  //      // Serial.print("LOW\n");
  //      redCount++;
  //      if(redCount == 10){
  //        //
  //        // SERVICE WINNER HERE
  //        //
  //        Serial.print("Winner! Winner! Winner!\n");
  //        redCount = 0;
  //      }
  //      updateScreen(blueCount, redCount);
  //    }
  //  }
  //  // save the current button state
  //  rlastButtonState = rbuttonState;
}
