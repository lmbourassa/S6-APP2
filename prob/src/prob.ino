/*
 * Project prob
 * Description:
 * Author:
 * Date:
 */
#include "LightSensor.h"
#include "BarSensor.h"
#include "HumSensor.h"
#include "WindSensor.h"
#include "RainSensor.h"

#define MASTER 1
#define SLAVE 0
#define MODE SLAVE

SYSTEM_THREAD(ENABLED);

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
  delay(2000);

#if MODE == SLAVE
  Wire.begin();
  barSensor.init();
#endif
}

void loop() {
#if MODE == SLAVE
  Serial.printlnf("Light: %d", lightSensor.getRaw());
  Serial.printlnf("Pressure: %.2f", barSensor.getCompP(true));
  Serial.printlnf("Temperature: %.2f", barSensor.getCompT(false));
  Serial.printlnf("Humidity: %.2f", humSensor.getHumidity(true));
  Serial.printlnf("Temperature: %.2f", humSensor.getTemperature(false));
  Serial.printlnf("Wind Direction: %d", windSensor.getDir());
  Serial.printlnf("Wind Speed: %f", windSensor.getSpeed());
  Serial.printlnf("Rain: %f", rainSensor.getRain());

  delay(2000);
#else
  
#endif
}