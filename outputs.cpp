#include <Arduino.h>
#include "ports.h"

void setupOutputs(){
  pinMode(led, OUTPUT);
  pinMode(leds1, OUTPUT);
  pinMode(leds2, OUTPUT);
  pinMode(leds3, OUTPUT);
  
  digitalWrite(led, HIGH);
  digitalWrite(leds1, LOW);
  digitalWrite(leds2, LOW);
  digitalWrite(leds3, LOW);
}
