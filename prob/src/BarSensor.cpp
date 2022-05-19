#include "BarSensor.h"
#include "spark_wiring_i2c.h"

void BarSensor::setBackgroundMode()
{
    Wire.beginTransmission(addr);
    Wire.write(0x08);
    Wire.write(0x07); // Background mode
    Wire.endTransmission();
}

void BarSensor::readCoefs()
{
    int coefs[15];

    Wire.beginTransmission(addr);
    Wire.write(0x13); // Address of the first data we want
    Wire.endTransmission(false);
    Wire.requestFrom(addr, 15);

    int i = 0;
    while(Wire.available())
    {
        coefs[i++] = Wire.read();
    }

    Wire.endTransmission();

    c00 = (coefs[0]<<12) | (coefs[1]<<4) | (coefs[2]>>4);
    c10 = ((coefs[2] | 0x0F)<<16) | (coefs[3]<<8) | coefs[4];
    c01 = (coefs[5]<<8) | coefs[6];
    c11 = (coefs[7]<<8) | coefs[8];
    c20 = (coefs[9]<<8) | coefs[10];
    c21 = (coefs[11]<<8) | coefs[12];
    c30 = (coefs[13]<<8) | coefs[14];
}

BarSensor::BarSensor(int addr, int k)
{
    this->addr = addr;
    this->k = k;
}

void BarSensor::init()
{
    setBackgroundMode();
    readCoefs();
}

long int BarSensor::getPresRaw()
{
    long int pres = 0;

    Wire.beginTransmission(addr);
    Wire.write(0x00); // Pressure first byte address
    Wire.endTransmission(false);

    Wire.requestFrom(addr, 3);

    while(Wire.available())
    {
        char c = Wire.read();

        pres<<=8;
        pres = pres|(int)c;
    }

    if((pres & 0x0800) == 0x0800)
    {
        pres |= 0xFF000000;
    }

    return pres;
}

long int BarSensor::getTempRaw()
{
    long int temp = 0;

    Wire.beginTransmission(addr);
    Wire.write(0x03); // Temperature first byte address
    Wire.endTransmission(false);

    Wire.requestFrom(addr, 3);

    while(Wire.available())
    {
        char c = Wire.read();

        temp<<=8;
        temp = temp|(int)c;
    }

    if((temp & 0x0800) == 0x0800)
    {
        temp |= 0xFF000000;
    }

    return temp;
}

float BarSensor::getCompP()
{
    float p = getPresRaw()/(float)k;
    float t = getTempRaw()/(float)k;

    return c00+p*(c10+p*(c20+p*c30))+t*c01+t*p*(c11+p*c21);
}

// Todo
float BarSensor::getCompT()
{
    return 0.0;
}