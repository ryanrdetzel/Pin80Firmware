
#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010

int led = 13;

int ports[4] = {9,10,11,2};
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

void loop() {
  while(Serial.available()) {
    /* Reads one command over serial */
    cmd = Serial.readStringUntil('\n').trim();

    // Direct port
    if (cmd.toUpperCase().startsWith("P")) {
      int firstSpace = cmd.indexOf(' ');
      int portNumber = cmd.substring(1, firstSpace).toInt();
      String action = cmd.substring(firstSpace + 1).toUpperCase();

      // Check if it's within range?
      Serial.print("Received: ");
      Serial.print(portNumber);
      Serial.println(action);

      int pinNumber = ports[portNumber];

    /*
      P4 PXSTART
      P4 PX23 001600
      P4 PX1,2,3,4 160000
      P4 PX6-16 160000
      P4 PXEND
     */
      if (action.startsWith("PX")){
        if (action.equals("PXEND")){
          updateAllPixels();
          updatingPixels = false;
        }
        else if (action.equals("PXSTART")){
          updatingPixels = true;
        }
        else if (updatingPixels){ 
          // Validate the port for pixels is the same.
          // split action to led number, color.

          firstSpace = action.indexOf(' ');
          String ledInfo = action.substring(2, firstSpace);
          String colorInfo = action.substring(firstSpace + 1);

          int pixels[64]; // Update max 64 pixels
          int commaLocation = ledInfo.indexOf(',');
          int dashLocation = ledInfo.indexOf('-');
          
          int ledNumber;
          int pixelCount = 0;
          
          // Check if it's a list or range of leds
          if (commaLocation > 0){
            while(commaLocation > 0){
              ledNumber = ledInfo.substring(0, commaLocation).toInt();
              pixels[pixelCount++] = ledNumber;
              ledInfo = ledInfo.substring(commaLocation + 1);
              commaLocation = ledInfo.indexOf(',');
            }
            // grab the last of multiple of the only if it's a single
            ledNumber = ledInfo.substring(0).toInt();
            pixels[pixelCount++] = ledNumber;
          }else if (dashLocation > 0){
            ledNumber = ledInfo.substring(0, dashLocation).toInt();
            ledInfo = ledInfo.substring(dashLocation + 1);
            int endRangeNumber = ledInfo.substring(0).toInt();
            pixelCount = endRangeNumber - ledNumber + 1; // inclusive
            
            for (int x = 0; x<pixelCount; x++){
              pixels[x] = ledNumber++;
            }
          } else { //Single value
            ledNumber = ledInfo.substring(0).toInt();
            pixels[pixelCount++] = ledNumber;
          }

          char buf[7];
          colorInfo.toCharArray(buf, 7);
          int colorValue = x2i(buf);
          for (int x=0;x<pixelCount;x++){
            setPixel(pixels[x], colorValue);
          }
        }
        return;
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
