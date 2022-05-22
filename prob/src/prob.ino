/*
 * Project prob
 * Description:
 * Author:
 * Date:
 */
#include "LightSensor.h"
#include "BarSensor.h"
#include "HumSensor.h"

SYSTEM_THREAD(ENABLED);

  LightSensor lightSensor(A0);
  BarSensor barSensor(0x77, 524288);
  HumSensor humSensor(D4);

void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected, 30000);
  delay(2000);

  Wire.begin();

  barSensor.init();
}

void loop() {
  Serial.printlnf("Light: %d", lightSensor.getRaw());
  Serial.printlnf("Pressure: %.2f", barSensor.getCompP(true));
  Serial.printlnf("Temperature: %.2f", barSensor.getCompT(false));
//   Serial.printlnf("Humidity: %.2f", humSensor.getHumidity(true));
//   Serial.printlnf("Temperature: %.2f", humSensor.getTemperature(false));

  delay(2000);
}