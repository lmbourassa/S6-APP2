#include "UART.h"
#include "spark_wiring.h"
#include "spark_wiring_usbserial.h"

void UART::sendData()
{
    Serial1.write((byte*)&data, sizeof(Data));
}

UART::UART(bool mode)
{
    this->mode = mode;
    Serial1.begin(9600);
}

void UART::requestData()
{
    Serial1.write(0x42);

    while(!Serial1.available());

    byte buffer[sizeof(Data)];

    long i = 0;
    while(Serial1.available())
    {
        buffer[i++] = Serial1.read();
    }

    Data* tempData = (Data*)buffer;

    data->lightData = tempData->lightData;
    data->barPresData = tempData->barPresData;
    data->barTempData = tempData->barTempData;
    data->humData = tempData->humData;
    data->tempData = tempData->tempData;
    data->windDirData = tempData->windDirData;
    data->windSpeedData = tempData->windSpeedData;
    data->rainData = tempData->rainData;
}

void UART::checkRequest()
{
    if (Serial1.available())
    {
        int inByte = Serial1.read();
        
        if(inByte == 0x42)
        {
            sendData();
        }
    }
}