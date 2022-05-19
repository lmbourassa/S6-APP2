#ifndef BARSENSOR_H
#define BARSENSOR_H

class BarSensor
{
    private:
    int addr;
    int k;
    int presRaw, tempRaw;
    int c00, c10, c01, c11, c20, c21, c30;
    void setBackgroundMode();
    void readCoefs();

    public:
    BarSensor(int addr, int k);
    void init();
    long int getPresRaw();
    long int getTempRaw();
    float getCompP();
    float getCompT();
};

#endif