#ifndef BARSENSOR_H
#define BARSENSOR_H

class BarSensor
{
    private:
    int addr;
    long int k;
    long int presData, tempData;
    long int c0, c1, c00, c10, c01, c11, c20, c21, c30;
    void setBackgroundMode();
    void readCoefs();
    void readData();
    long int extend(long int data, long int mask);

    public:
    BarSensor(int addr, long int k);
    void init();
    float getCompP(bool readData);
    float getCompT(bool readData);
};

#endif