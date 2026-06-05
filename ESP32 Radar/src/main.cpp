#include <Arduino.h>
#include <ESP32Servo.h>

const int SERVO_PIN = 18;
const int ECHO_PIN = 4;
const int TRIG_PIN = 2;
const float soundSpeed = 0.0343f; 

Servo servo;

unsigned long prevTime = 0;

int angle = 0;
int direction = 1;

void setup()
{
    Serial.begin(9600);

    servo.attach(SERVO_PIN);
    servo.write(0);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void startPulse()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);
}

float getTimeGoAndReturn()
{
    startPulse();

    return pulseIn(ECHO_PIN, HIGH, 30000);
}

void loop()
{
    unsigned long currTime = millis();

   

    if (currTime - prevTime >= 30)
    {
        
        float timeUltrasonic = getTimeGoAndReturn();
        float distance = timeUltrasonic * soundSpeed / 2.0f;

        prevTime = currTime;

        servo.write(angle);

        angle += direction;

        if (angle >= 180)
        {
            angle = 180;
            direction = -1;
        }
        else if (angle <= 0)
        {
            angle = 0;
            direction = 1;
        }

        
        Serial.print(angle);
        Serial.print(" ");
        Serial.println(distance);
    
    }
}