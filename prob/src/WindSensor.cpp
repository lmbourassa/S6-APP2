#include "WindSensor.h"
#include "spark_wiring.h"
#include "spark_wiring_usbserial.h"

WindSensor::WindSensor(int dirPin, int speedPin)
{
    this->dirPin = dirPin;
    this->speedPin = speedPin;
    pinMode(speedPin, INPUT);
}

int WindSensor::getDir()
{
    return analogRead(dirPin);
}

float WindSensor::getSpeed()
{
    unsigned long time = pulseIn(speedPin, HIGH) + pulseIn(speedPin, LOW);
    Serial.printlnf("Time: %lu", time);

    if(time < 10000)
    {
        return 0;
    }

    return (-0.0000012*time)+3.6;
}