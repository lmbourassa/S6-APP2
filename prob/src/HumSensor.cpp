#include "HumSensor.h"
#include "spark_wiring.h"
#include "spark_wiring_interrupts.h"
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
    pinMode(pin, INPUT_PULLUP);
    delayMicroseconds(holdFor);
}

void HumSensor::awaitResponse()
{
    // Serial.println("awaitResponse");
    pinMode(pin, INPUT_PULLUP);
    delayMicroseconds(10);

    while(!digitalRead(pin));
    while(digitalRead(pin));
}

unsigned long HumSensor::readBit()
{
    // Serial.println("readBit");
    unsigned long bitStart, bitEnd;

    pinMode(pin, INPUT_PULLUP);

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

    return bitEnd - bitStart;
}

void HumSensor::readData()
{
    Serial.println("readData");
    unsigned long bitTimes[40];

    noInterrupts();

    writeLow(20000);
    writeHigh(40);
    awaitResponse();

    for(int i = 0; i < 40; i++)
    {
        // Serial.printlnf("%d", i);
        bitTimes[i] = readBit();
    }

    interrupts();

    char bits[40];

    for(int i = 0; i < 40; i++)
    {
        if((bitTimes[i] > 24) && (bitTimes[i] < 30))
        {
            bits[i] = 0;
        }

        else
        {
            bits[i] = 1;
        }
    }

    char humInt = (bits[0] << 7) + (bits[1] << 6) + (bits[2] << 5) + (bits[3] << 4) + (bits[4] << 3) + (bits[5] << 2) + (bits[6] << 1) + bits[7];
    char humDec = (bits[8] << 7) + (bits[9] << 6) + (bits[10] << 5) + (bits[11] << 4) + (bits[12] << 3) + (bits[13] << 2) + (bits[14] << 1) + bits[15];
    char tempInt = (bits[16] << 7) + (bits[17] << 6) + (bits[18] << 5) + (bits[19] << 4) + (bits[20] << 3) + (bits[21] << 2) + (bits[22] << 1) + bits[23];
    char tempDec = (bits[24] << 7) + (bits[25] << 6) + (bits[26] << 5) + (bits[27] << 4) + (bits[28] << 3) + (bits[29] << 2) + (bits[30] << 1) + bits[31];
    char cs = (bits[32] << 7) + (bits[33] << 6) + (bits[34] << 5) + (bits[35] << 4) + (bits[36] << 3) + (bits[37] << 2) + (bits[38] << 1) + bits[39];

    char sum = humInt + humDec + tempInt + tempDec;

    // Serial.printlnf("humInt: %X", humInt);
    // Serial.printlnf("humDec: %X", humDec);
    // Serial.printlnf("tempInt: %X", tempInt);
    // Serial.printlnf("tempDec: %X", tempDec);
    // Serial.printlnf("cs: %X", cs);
    // Serial.printlnf("sum: %X", sum);

    if(sum != cs)
    {
        Serial.printlnf("Problem with humidity sensor communication. Received checksum = %X, calculated checksum = %X", cs, sum);
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