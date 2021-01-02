
#include <Arduino.h>

#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010

// This matters for port calculation
// P0 is the first 48, P1 is the second, etc.

const int ledsPerStrip = 24;

int led = 13;

const int strip1 = 2;
const int strip2 = 14;

int ports[5] = {9, 10, 11, strip1, strip2};
String cmd;

void setup() {
  Serial.begin(115200);
  
  pinMode(led, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  digitalWrite(led, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);

  initLeds();
}

bool updatingPixels = false;

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

void loop() {
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
        digitalWrite(pinNumber, LOW);
      } else if (action.equals("OFF")){
        digitalWrite(pinNumber, HIGH);
      } else {
         Serial.println("Unknown command");
      }
    }
  }
}

// Convert hex string to int value
int x2i(char *s){
 int x = 0;
 for(;;) {
   char c = *s;
   if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0';
   }
   else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10;
   }
   else break;
   s++;
 }
 return x;
}
