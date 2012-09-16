#include <Bounce.h>

// Define button pins
#define BLUEBUTTON 7
#define REDBUTTON 6

// Define Transistor control pins
#define BLUETRANS 5
#define REDTRANS 4

// Define shift register control pins
int latchPin = 8; // RCLK pin
int clockPin = 9; // SRCLK pin
int dataPin = 10; // SER pin

int n = 1;

// Active Screen Variable
// Blue = 0
// Red = 1
volatile int active_display = 0;

//
// Make array of 7 seg digits
// Common anode means digital low (0) turns an led ON!
//
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

// Instantiate player's scores                    
int blueCount = 0;
int redCount = 0;

// Instantiate screen bytes
byte blueByte = segArray[blueCount];
byte redByte = segArray[redCount];

// Timer2 reload value, globally available
unsigned int tcnt2;

////////////////////////////////////////////////
// USER FUNCTIONS
//
//
//
// Refreshes each digit of the display
//
void updateScreen(int screen1, int screen2) {
    // Lookup LED in array
    blueByte = segArray[screen1];
    redByte = segArray[screen2];
    
    Serial.print("Blue Screen:\n");
    Serial.print("\tbits: ");
    Serial.print(blueByte, BIN);
    Serial.print("\n");
    Serial.print("\tNum: ");
    Serial.print(screen1, DEC);
    Serial.print("\n");
    Serial.print("Red Screen:\n");
    Serial.print("\tbits: ");
    Serial.print(redByte, BIN);
    Serial.print("\n");
    Serial.print("\tNum: ");
    Serial.print(screen2, DEC);
    Serial.print("\n");
    Serial.print("************************\n");
}

// Shift one digit byte into the register
void displayDigit(byte screen){
    // Shift data into the shift register
    digitalWrite(latchPin, 0); // LATCH LOW TO SHIFT
    shiftOut(dataPin, clockPin, MSBFIRST, screen);  
    digitalWrite(latchPin, 1); // ENABLE SHIFTED BITS
}

// Multiplexing Interupt Service Routine
ISR(TIMER2_OVF_vect){
  
  // Turn off the active display
  if(active_display == 0){
    digitalWrite(BLUETRANS, HIGH);
  }
  else{
    digitalWrite(REDTRANS, HIGH);
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
    digitalWrite(BLUETRANS, LOW);
  }
  else{
    digitalWrite(REDTRANS, LOW);
  }

  // Reload the timer
  TCNT2 = tcnt2;
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

// Instantiate RED BUTTON object
Bounce redButton = Bounce(REDBUTTON, 5);
int rbuttonState = 0;
int rlastButtonState = 1;

void setup(){

    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);

    // Transistor Control
    pinMode(BLUETRANS, OUTPUT);
    pinMode(REDTRANS, OUTPUT);

    // Setup TIMER2 to multiplex the displays
    /* First disable the timer overflow interrupt while we're configuring */
    TIMSK2 &= ~(1<<TOIE2);
    /* Configure timer2 in normal mode (pure counting, no PWM etc.) */
    TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
    TCCR2B &= ~(1<<WGM22);
    /* Select clock source: internal I/O clock */
    ASSR &= ~(1<<AS2);
    /* Now configure the prescaler to CPU clock divided by 1024 */
    TCCR2B |= (1<<CS22)  | (1<<CS21) | (1<<CS20); // Set bits
    /* We need to calculate a proper value to load the timer counter.
     * The following loads the value 151 into the Timer 2 counter register
     * The math behind this is:
     * (CPU frequency) / (prescaler value) = (new timer freq)
     * 16 MHz / 1024 = 15625 Hz = 64 us period
     * We want a 150 Hz refresh rate so...
     * 1 / 150 = 6 ms period
     * (desired period) /  us = 125.
     * 6ms / 64us = 104
     * 256 - 104 = 151;
     */
    /* Save value globally for later reload in ISR */
    tcnt2 = 151; 
    /* Finally load end enable the timer */
    TCNT2 = tcnt2;
    TIMSK2 |= (1<<TOIE2);

  
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
          Serial.print("Winner! Blue! Winner!\n");
          blueCount = 0;
        }
        updateScreen(blueCount, redCount);
      }
    }
    // save the current button state
    blastButtonState = bbuttonState;
    
  //
  //SERVICE RED PLAYER
  //
    
   // Update the debouncer
   redButton.update();
   // Get the updated value
   int rbuttonState = redButton.read();
   
   // If button is actually pressed
   if (rbuttonState != rlastButtonState) {
     if (rbuttonState == HIGH) {
       // Do work on press
       // Serial.print("LOW\n");
       redCount++;
       if(redCount == 10){
         //
         // SERVICE WINNER HERE
         //
         Serial.print("Winner! Red! Winner!\n");
         redCount = 0;
       }
       updateScreen(blueCount, redCount);
     }
   }
   // save the current button state
   rlastButtonState = rbuttonState;
}
