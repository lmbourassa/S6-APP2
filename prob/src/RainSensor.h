#ifndef RAINSENSOR_H
#define RAINSENSOR_H

class RainSensor
{
    private:
    int pin;

    public:
    RainSensor(int pin);
    float getRain();
};

#endif