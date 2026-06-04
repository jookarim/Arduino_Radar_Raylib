#include <Arduino.h>
#include <ESP32Servo.h>


Servo servo;
unsigned long prevTime = 0;

void setup() {
  servo.attach(18);
  servo.write(0);
  Serial.begin(9600);
}

int angle = 0;
int direction = 1;

void loop() {
      unsigned long currTime = millis();

      if(currTime - prevTime >= 200)
      {
        prevTime = currTime;
        servo.write(angle);
        angle += direction;

        if(angle >= 180)
        {
          direction = -1;
        }

        else if(angle <= 0)
        {
          direction = 1;
        }

        Serial.println(angle);
      }
    
}
