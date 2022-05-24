#include "BarSensor.h"
#include "spark_wiring_i2c.h"
#include "spark_wiring_usbserial.h"

void BarSensor::setBackgroundMode()
{
    Wire.beginTransmission(addr);
    Wire.write(0x08);
    Wire.write(0x07); // Background mode
    Wire.endTransmission();
}

void BarSensor::setPressureMode()
{
    Wire.beginTransmission(addr);
    Wire.write(0x06);
    Wire.write(0x00);
    Wire.endTransmission();
}

void BarSensor::setTemperatureMode()
{
    Wire.beginTransmission(addr);
    Wire.write(0x07);
    Wire.write(0x00);
    Wire.endTransmission();
}

void BarSensor::readCoefs()
{
    int coefs[18];

    Wire.beginTransmission(addr);
    Wire.write(0x13); // Address of the first data we want
    Wire.endTransmission(false);
    Wire.requestFrom(addr, 18);

    int i = 0;
    while(Wire.available())
    {
        coefs[i++] = Wire.read();
    }

    Wire.endTransmission();

    c0 = (coefs[0] << 4) | (coefs[1] >> 4);
    c1 = ((coefs[1] | 0x0F) << 8) | coefs[2];
    c00 = (coefs[3] << 12) | (coefs[4] << 4) | (coefs[5] >> 4);
    c10 = ((coefs[5] | 0x0F) << 16) | (coefs[6] << 8) | coefs[7];
    c01 = (coefs[8] << 8) | coefs[9];
    c11 = (coefs[10] << 8) | coefs[11];
    c20 = (coefs[12] << 8) | coefs[13];
    c21 = (coefs[14] << 8) | coefs[15];
    c30 = (coefs[16] << 8) | coefs[17];

    c0 = extend(c0, 0x00000800);
    c1 = extend(c1, 0x00000800);
    c00 = extend(c00, 0x00080000);
    c10 = extend(c10, 0x00080000);
    c01 = extend(c01, 0x00008000);
    c11 = extend(c11, 0x00008000);
    c20 = extend(c20, 0x00008000);
    c21 = extend(c21, 0x00008000);
    c30 = extend(c30, 0x00008000);
}

void BarSensor::readData()
{
    long int data[6];

    Wire.beginTransmission(addr);
    Wire.write(0x00); // Pressure first byte address
    Wire.endTransmission(false);

    Wire.requestFrom(addr, 6);

    int i = 0;
    while(Wire.available())
    {
        data[i++] = Wire.read();
    }

    Wire.endTransmission();

    if(i < 6)
    {
        Serial.println("Problem with barometric sensor communication.");
        return;
    }

    long int pres = (data[0] << 16) | (data[1] << 8) | (data[2]);
    long int temp = (data[3] << 16) | (data[4] << 8) | (data[5]);

    presData = extend(pres, 0x00800000);
    tempData = extend(temp, 0x00800000);
}

long BarSensor::extend(long data, long mask)
{
    if(mask == 0x00)
    {
        return data;
    }

    // Serial.printlnf("Data: 0x%lX, mask: 0x%lX", data, mask);

    if((data & mask) == mask)
    {
        long e = 0x80000000;
        while((e & mask) != mask)
        {
            data |= e;
            e >>= 1;
        }
    }

    // Serial.printlnf("Extended: 0x%lX = %ld", data, data);

    return data;
}

BarSensor::BarSensor(int addr, long int k)
{
    this->addr = addr;
    this->k = k;
}

void BarSensor::init()
{
    setBackgroundMode();
    setPressureMode();
    setTemperatureMode();
    readCoefs();
}

float BarSensor::getCompP(bool readData)
{
    if(readData)
    {
        this->readData();
    }

    float p = presData/(float)k;
    float t = tempData/(float)k;

    return c00 + p*(c10 + p*(c20 + p*c30)) + t*c01 + t*p*(c11 + p*c21);
}

float BarSensor::getCompT(bool readData)
{
    if(readData)
    {
        this->readData();
    }
    
    float t = tempData/(float)k;

    return c0*0.5 + c1*t;
}