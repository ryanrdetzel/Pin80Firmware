#include <Arduino.h>
#include <MCP23008.h>
#include <MCP23017.h>

#include "ports.h"

uint16_t buttonInputs = 0;

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

void checkInput(){
  //uint8_t current8 = mcp8.readPort();
  uint8_t currentA = mcp17.readPort(MCP23017Port::A);
  uint8_t currentB = mcp17.readPort(MCP23017Port::B);

  uint16_t current = currentA | currentB << 8;

  // Button input changed.
  if (current != buttonInputs){
    buttonInputs = current;

    // 1111111111011111
    // bool on = (((current >> 5) & 1) == 1); // Check 5th bit if it's 0 (on)
    
    if ((current >> 0) & 1){
      digitalWrite(leds2, HIGH);
    }else {
      digitalWrite(leds2, LOW);
    }

    if ((current >> 5) & 1){
      digitalWrite(leds1, HIGH);
    }else {
      digitalWrite(leds1, LOW);
    }
    
    //Serial.print(current, BIN);
    //Serial.println();
  }
}
