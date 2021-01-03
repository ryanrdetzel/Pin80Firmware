#if !defined(PORT_CONSTANTS_H)
#define PORT_CONSTANTS_H 1


// Pins on teensy for led outputs
const int led = 13;
const int leds1 = 0;
const int leds2 = 1;
const int leds3 = 3;

// Pins on teensy for pixels
const int strip1 = 2;
const int strip2 = 14;

const int ports[5] = {leds1, leds2, leds3, strip1, strip2};

#endif
