#include "RainSensor.h"
#include "spark_wiring.h"
#include "spark_wiring_usbserial.h"

RainSensor::RainSensor(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
}

float RainSensor::getRain()
{
    unsigned long time = pulseIn(pin, HIGH) + pulseIn(pin, LOW);
    Serial.printlnf("Time: %lu", time);

    return (1000000.0/time)*0.2794;
}