#ifndef WINDSENSOR_H
#define WINDSENSOR_H

class WindSensor
{
    private:
    int dirPin;
    int speedPin;

    public:
    WindSensor(int dirPin, int speedPin);
    int getDir();
    float getSpeed();
};

#endif