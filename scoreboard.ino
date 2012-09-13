#include <Bounce.h>

#define BLUEBUTTON 7
#define REDBUTTON 6

#define BLUETRANS 5
#define REDTRANS 4

int latchPin = 8; // ST_CP pin
int clockPin = 9; // SH_CP pin
int dataPin = 10; // DS pin

int n = 1;

// Active Screen Variable
// Blue = 0
// Red = 1
int active_display = 0;

// Instantiate player's scores                    
int blueCount = 0;
int redCount = 0;

// Instantiate screen bytes
byte blueByte = B00011000;
byte redByte = B00011000;

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

byte segArray[10] = {  B00011000, // 0 dec
                       B01111011, // 1 dec
                       B00101100, // 2 dec
                       B00101001, // 3 dec
                       B01001011, // 4 dec
                       B10001001, // 5 dec
                       B11001000, // 6 dec
                       B00111011, // 7 dec
                       B00001000, // 8 dec
                       B00001011  // 9 dec
                      };

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
    byte blueScreen = segArray[screen1];
    byte redScreen = segArray[screen2];
    
    // Shift data into the two registers
    digitalWrite(latchPin, 0); // LATCH LOW TO SHIFT
    // shiftOut(dataPin, clockPin, blueScreen);
    shiftOut(dataPin, clockPin, MSBFIRST, blueScreen);  
    // shiftOut(dataPin, clockPin, redScreen);
    digitalWrite(latchPin, 1); // ENABLE SHIFTED BITS

    Serial.print("Blue Screen:\n");
    Serial.print("\tbits: ");
    Serial.print(blueScreen, BIN);
    Serial.print("\n");
    Serial.print("\tNum: ");
    Serial.print(screen1, DEC);
    Serial.print("\n");
    Serial.print("Red Screen:\n");
    Serial.print("\tbits: ");
    Serial.print(redScreen, BIN);
    Serial.print("\n");
    Serial.print("\tNum: ");
    Serial.print(screen2, DEC);
    Serial.print("\n");
    Serial.print("************************\n");
}

void displayDigit(byte screen){
    // Shift data into the two registers
    digitalWrite(latchPin, 0); // LATCH LOW TO SHIFT
    // shiftOut(dataPin, clockPin, blueScreen);
    shiftOut(dataPin, clockPin, MSBFIRST, screen);  
    // shiftOut(dataPin, clockPin, redScreen);
    digitalWrite(latchPin, 1); // ENABLE SHIFTED BITS
}

void multiplexScreens(){
  // Turn off the active display
  if(active_display == 0){
    digitalWrite(BLUETRANS, LOW);
  }
  else{
    digitalWrite(REDTRANS, LOW);
  }

  // Toggle Display
  active_display ^= 1;

  // Shift out screen bits
  if(active_display == 0){
    displayDigit(blueByte);
  }
  else{
    displayDigit(redByte);
  }

  // Turn display back on
  if(active_display == 0){
    digitalWrite(BLUETRANS, HIGH);
  }
  else{
    digitalWrite(REDTRANS, HIGH);
  }
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

// Instantiate BLUE BUTTON object
Bounce blueButton = Bounce(BLUEBUTTON, 5);
int bbuttonState = 0;
int blastButtonState = 1;

// // Instantiate RED BUTTON object
// Bounce redButton = Bounce(REDBUTTON, 5);
// int rbuttonState = 0;
// int rlastButtonState = 1;

void setup(){
    
    pinMode(BLUEBUTTON, INPUT); // Setup BLUE button
    // pinMode(REDBUTTON, INPUT);  // Setup RED button

    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);

    // Transistor Control
    pinMode(BLUETRANS, OUTPUT);
    pinMode(REDTRANS, OUTPUT);

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
    
    Serial.begin(9600); // Start serial output
}


//
// MAIN LOOP
//
//
void loop(){
    if(n == 1) {
      digitalWrite(BLUETRANS, LOW);
      digitalWrite(REDTRANS, HIGH);
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
  //  redButton.update();
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
