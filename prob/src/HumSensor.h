#ifndef HUMSENSOR_H
#define HUMSENSOR_H

class HumSensor
{
    private:
    int pin;
    float humData, tempData;
    void writeLow(int holdFor);
    void writeHigh(int holdFor);
    void awaitResponse();
    unsigned long readBit();
    void readData();

    public:
    HumSensor(int pin);
    float getHumidity(bool readData);
    float getTemperature(bool readData);
};

#endif