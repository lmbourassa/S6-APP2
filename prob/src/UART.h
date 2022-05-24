#ifndef UART_H
#define UART_H

struct Data
{
    int lightData;
    float barPresData;
    float barTempData;
    float humData;
    float tempData;
    int windDirData;
    float windSpeedData;
    float rainData;
};

class UART
{
    private:
    bool mode; // 0 = Slave  1 = Master
    Data* data;
    void sendData();
    
    public:
    UART(bool mode, Data* data);
    void requestData();
    bool checkRequest();
};

#endif