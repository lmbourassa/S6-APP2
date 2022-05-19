#include "LightSensor.h"
#include "spark_wiring.h"

LightSensor::LightSensor(int pin)
{
    this->pin = pin;
}

int LightSensor::getRaw()
{
    return analogRead(pin);
}

float LightSensor::getVoltage()
{
    // int raw = getRaw();
    // voltage conversion
    float voltage = 0.0;
    return voltage;
}