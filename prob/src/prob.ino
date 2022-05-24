/*
 * Project prob
 * Description:
 * Author:
 * Date:
 */
#include "UART.h"
#include "LightSensor.h"
#include "BarSensor.h"
#include "HumSensor.h"
#include "WindSensor.h"
#include "RainSensor.h"

#define MASTER 1
#define SLAVE 0
#define MODE SLAVE

SYSTEM_THREAD(ENABLED);

Data data;
UART uart(MODE, &data);

#if MODE == SLAVE
  LightSensor lightSensor(A0);
  BarSensor barSensor(0x77, 524288);
  HumSensor humSensor(D4);
  WindSensor windSensor(A2, A3);
  RainSensor rainSensor(A5);
#endif

void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected, 30000);

#if MODE == SLAVE
  Wire.begin();
  barSensor.init();
#endif
}

void loop() {
#if MODE == SLAVE
  delay(3000);
  
  data.lightData = lightSensor.getRaw();
  data.barPresData = barSensor.getCompP(true);
  data.barTempData = barSensor.getCompT(false);
  data.humData = humSensor.getHumidity(true);
  data.tempData = humSensor.getTemperature(false);
  data.windDirData = windSensor.getDir();
  data.windSpeedData = windSensor.getSpeed();
  data.rainData = rainSensor.getRain();

  if(uart.checkRequest())
  {
    Serial.printlnf("Light: %d", data.lightData);
    Serial.printlnf("Pressure: %.2f", data.barPresData);
    Serial.printlnf("Baro Temperature: %.2f", data.barTempData);
    Serial.printlnf("Humidity: %.2f", data.humData);
    Serial.printlnf("Temperature: %.2f", data.tempData);
    Serial.printlnf("Wind Direction: %d", data.windDirData);
    Serial.printlnf("Wind Speed: %.2f", data.windSpeedData);
    Serial.printlnf("Rain: %.2f", data.rainData);
  }
#else
  delay(5000);
  uart.requestData();

  Serial.printlnf("Light: %d", data.lightData);
  Serial.printlnf("Pressure: %.2f", data.barPresData);
  Serial.printlnf("Baro Temperature: %.2d", data.barTempData);
  Serial.printlnf("Humidity: %.2f", data.humData);
  Serial.printlnf("Temperature: %.2f", data.tempData);
  Serial.printlnf("Wind Direction: %d", data.windDirData);
  Serial.printlnf("Wind Speed: %.2f", data.windSpeedData);
  Serial.printlnf("Rain: %.2f", data.rainData);
#endif
}