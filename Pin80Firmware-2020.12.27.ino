#include <Arduino.h>

#include "pixels.h"
#include "utils.h"
#include "ports.h"
#include "buttons.h"
#include "outputs.h"

/* Variables used */
bool updatingPixels = false;

String cmd;

void setup() {
  Serial.begin(115200);

  setupButtons();

  /* Setup the outputs */
  setupOutputs();

  /* Setup pixel strips */
  initLeds();
}

void loop() {
  checkInput();
  readSerialInput();

}

void readSerialInput(){
  while(Serial.available()) {
    /* Reads one command over serial */
    cmd = Serial.readStringUntil('\n').trim();

    // Direct port
    if (cmd.toUpperCase().startsWith("P")) {
      if (cmd.equals("PXEND")){
        updateAllPixels();
        updatingPixels = false;
        Serial.println("Pixel end");
        continue;
      }
      else if (cmd.equals("PXSTART")){
        updatingPixels = true;
        Serial.println("Pixel start");
        continue;
      }

      /* Extract the port information from the request */
      int firstSpace = cmd.indexOf(' ');
      int portNumber = cmd.substring(1, firstSpace).toInt();
      String action = cmd.substring(firstSpace + 1).toUpperCase();

      int pinNumber = ports[portNumber];
      
      // Check if it's within range?
      Serial.print("Port: ");   Serial.print(portNumber);
      Serial.print("   Pin: "); Serial.print(pinNumber);
      Serial.print("  ");       Serial.println(action);

    /*
      PXSTART
      P4 PX0 001600
      P4 PX1,3,5 160000
      P4 PX0-6 160000
      PXEND

      PXSTART\nP4 PX0 001600\nPXEND\n
      
      P0 ON
      P0 OFF
     */
      if (action.startsWith("PX")){
        // Validate the port for pixels is the same.
        // split action to led number, color.

        if (!updatingPixels) continue;

        // Get the actual pixel start for the strip
        int firstPixel = getFirstPixel(portNumber);
        Serial.print("  firstPixel: "); Serial.println(firstPixel);

        firstSpace = action.indexOf(' ');
        String ledInfo = action.substring(2, firstSpace);
        String colorInfo = action.substring(firstSpace + 1);

        int pixels[64]; // Update max 64 pixels
        int commaLocation = ledInfo.indexOf(',');
        int dashLocation = ledInfo.indexOf('-');
        
        int ledNumber;
        int pixelCount = 0;
        
        // Check if it's a list or range of leds


        if (commaLocation > 0)
        {
          /* List */
          while(commaLocation > 0){
            ledNumber = ledInfo.substring(0, commaLocation).toInt();
            pixels[pixelCount++] = firstPixel + ledNumber;
            ledInfo = ledInfo.substring(commaLocation + 1);
            commaLocation = ledInfo.indexOf(',');
          }
          // grab the last of multiple of the only if it's a single
          ledNumber = ledInfo.substring(0).toInt();
          pixels[pixelCount++] = firstPixel + ledNumber;
        }
        else if (dashLocation > 0)
        {
          /* Range */
          ledNumber = ledInfo.substring(0, dashLocation).toInt();
          ledInfo = ledInfo.substring(dashLocation + 1);
          int endRangeNumber = ledInfo.substring(0).toInt();
          pixelCount = endRangeNumber - ledNumber + 1; // inclusive

          int pixelNum = firstPixel + ledNumber;
          for (int x = 0; x<pixelCount; x++){
            pixels[x] = pixelNum++;
          }
        }
        else 
        { 
          //Single value
          ledNumber = ledInfo.substring(0).toInt();
          // Get the actual pixel start for the strip
        
          pixels[pixelCount++] = firstPixel + ledNumber;
        }

        char buf[7];
        colorInfo.toCharArray(buf, 7);
        int colorValue = x2i(buf);
        for (int x=0;x<pixelCount;x++){
          setPixel(pixels[x], colorValue);
          Serial.print(pixels[x]);
          Serial.print("=");
          Serial.println(colorValue);
        }
      }
      else if (action.equals("ON")){
        digitalWrite(pinNumber, HIGH);
      } else if (action.equals("OFF")){
        digitalWrite(pinNumber, LOW);
      } else {
         Serial.println("Unknown command");
      }
    }
  }
}
