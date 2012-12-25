Arduino Scoreboard
==================
 
## Parts List: ##

#### Microcontroller ####
- [Arduino Pro Mini 328 5V/16MHz](https://www.sparkfun.com/products/11113) x 1

#### Buttons/Switches ####
- [Red Concave Button](https://www.sparkfun.com/products/9336) x 1
- [Blue Concave Button](https://www.sparkfun.com/products/9337) x 1
- [Rocker Power Switch](https://www.sparkfun.com/products/11138) x 1

#### Displays ####
- [Red 7-Segment Display](https://www.sparkfun.com/products/8546) x 1
- [Blue 7-Segment Display](https://www.sparkfun.com/products/9191) x 1

#### ICs ####
- [74HC595 Shift Register](https://www.sparkfun.com/products/733) x 1

## Notes: ##

- This scoreboard sketch requires the use of seven (7) digital I/O pins.

- The sketch also expects the arduino to run at 16 MHz. 
    - If another clock speed is required, the timer must be adjusted in order to facilitate proper multiplexing of the screens.
