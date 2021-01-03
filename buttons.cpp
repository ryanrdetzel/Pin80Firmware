#include <Arduino.h>
#include <MCP23008.h>
#include <MCP23017.h>

#include "ports.h"

uint16_t buttonInputs = 0xFFFF; // All buttons off at startup

unsigned long previous_millis = 0;
const int bounce_millis = 30;

// Input board
MCP23008 mcp8 = MCP23008(0x20);
MCP23017 mcp17 = MCP23017(0x21);

void setupButtons(){
    /* Setup the inputs */
  Wire.begin();
  mcp8.init();
  mcp8.portMode(0b11111111);  // input mode

  mcp17.init();
  mcp17.portMode(MCP23017Port::A, 0b11111111);  // input mode
  mcp17.portMode(MCP23017Port::B, 0b11111111);  // input mode
}

int buttons[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

void checkInput(){
  //uint8_t current8 = mcp8.readPort();
  uint8_t currentA = mcp17.readPort(MCP23017Port::A);
  uint8_t currentB = mcp17.readPort(MCP23017Port::B);

  uint16_t current = currentA | currentB << 8;

  // Button input changed.
  unsigned long current_time = millis();

  // Something has changed.
  if (current != buttonInputs && current_time >= previous_millis + bounce_millis){
    previous_millis = current_time;

    //Serial.print("Something changed: ");
    //Serial.print(current,BIN);
    
    for (int i=0; i<16; i++){
      if ((((current >> i) & 1) != buttons[i])){
        //Serial.print(" changed: ");
        //Serial.print(i);
        
        buttons[i] = (current >> i) & 1;
        if (buttons[i] == 0){
          Joystick.button(i + 1, 1);
        }else{
          Joystick.button(i + 1, 0);
        }
        Joystick.X(512);
        Joystick.Y(512);
        Joystick.Z(512);
      }
    }
    
    buttonInputs = current;
    //Serial.println("");
  }
}
