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
    bool readBit();
    void readData();

    public:
    HumSensor(int pin);
    float getHumidity(bool readData);
    float getTemperature(bool readData);
};

#endif