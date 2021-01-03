#include <Arduino.h>
#include <OctoWS2811.h>

#include "pixels.h"
#include "ports.h"

#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010

const int ledsPerStrip = 24;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;
//WS2811_GRB
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

void initLeds(){
  leds.begin();
  
  for (int i=0; i < leds.numPixels(); i++) {
    leds.setPixel(i, WHITE);
    leds.show();
  }
}


int getFirstPixel(int port){
  int pin = ports[port];
  switch(pin){
    case strip1: return 0;
    case strip2: return ledsPerStrip;
  }

  return 0;
}

int lastPixel(int port){
  int pin = ports[port];
  switch(pin){
    case strip1: return ledsPerStrip - 1;
    case strip2: return (ledsPerStrip * 2) - 1;
  }

  return ledsPerStrip - 1;
}

void setPixel(int num, int value){
  leds.setPixel(num, value);
}

void updateAllPixels(){
  leds.show();
}
