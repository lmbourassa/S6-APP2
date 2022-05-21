#include "HumSensor.h"
#include "spark_wiring.h"
#include "spark_wiring_usbserial.h"

void HumSensor::writeLow(int holdFor)
{
    // Serial.println("writeLow");
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delayMicroseconds(holdFor);
}

void HumSensor::writeHigh(int holdFor)
{
    // Serial.println("writeHigh");
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delayMicroseconds(holdFor);
}

void HumSensor::awaitResponse()
{
    // Serial.println("awaitResponse");
    pinMode(pin, INPUT);

    delayMicroseconds(20);

    while(!digitalRead(pin))
    {
        delayMicroseconds(30);
    }

    while(digitalRead(pin))
    {
        delayMicroseconds(30);
    }
}

bool HumSensor::readBit()
{
    // Serial.println("readBit");
    unsigned long bitStart, bitEnd, bitTime;

    pinMode(pin, INPUT);

    delayMicroseconds(20);

    while(!digitalRead(pin))
    {
        delayMicroseconds(10);
    }

    bitStart = micros();
    while(digitalRead(pin))
    {
        delayMicroseconds(10);
    }
    bitEnd = micros();

    bitTime = bitEnd - bitStart;

    if((bitTime < 30) && (bitTime > 24))
    {
        return 0;
    }

    else
    {
        return 1;
    }
}

void HumSensor::readData()
{
    Serial.println("readData");
    long int data = 0;

    writeLow(18000);
    writeHigh(40);
    awaitResponse();

    for(int i = 0; i < 40; i++)
    {
        Serial.printlnf("%d", i);
        data <<= 1;
        data |= readBit();
    }

    long int humInt = data | 0xFF00000000;
    long int humDec = data | 0x00FF000000;
    long int tempInt = data | 0x0000FF0000;
    long int tempDec = data | 0x000000FF00;
    long int cs = data | 0x00000000FF;

    long int sum = (humInt + humDec + tempInt + tempDec) | 0xFF;

    if(sum != cs)
    {
        Serial.printlnf("Problem with humidity sensor communication. Received checksum = %lX, calculated checksum = %lX", cs, sum);
        return;
    }

    // Todo: use the integral and decimal parts to form a float
    humData = (float)humInt;
    tempData = (float)tempInt;
}

HumSensor::HumSensor(int pin)
{
    this->pin = pin;
}

float HumSensor::getHumidity(bool readData)
{
    if(readData)
    {
        this->readData();
    }

    return humData;
}

float HumSensor::getTemperature(bool readData)
{
    if(readData)
    {
        this->readData();
    }

    return tempData;
}