#include <OctoWS2811.h>

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
  //leds.show();
  
  for (int i=0; i < leds.numPixels(); i++) {
    leds.setPixel(i, WHITE);
    leds.show();
  }
}

void startLedTest(){
  leds.setPixel(0, RED);
}

void setPixel(int num, int value){
  leds.setPixel(num, value);
}

void updateAllPixels(){
  leds.show();
}

//void colorWipeRed()
//{
//  for (int i=0; i < leds.numPixels(); i++) {
//    leds.setPixel(i, RED);
//  }
//  leds.show();
//}
//
//void colorWipeWhite()
//{
//  for (int i=0; i < leds.numPixels(); i++) {
//    leds.setPixel(i, WHITE);
//  }
//  leds.show();
//}
