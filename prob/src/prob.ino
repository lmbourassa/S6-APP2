/*
 * Project prob
 * Description:
 * Author:
 * Date:
 */
#include "LightSensor.h"
#include "BarSensor.h"

SYSTEM_THREAD(ENABLED);

LightSensor lightSensor(A0);
BarSensor barSensor(0x77, 524288);

void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected, 30000);

  Wire.begin();

  barSensor.init();
}

void loop() {
  Serial.printlnf("Light: %d", lightSensor.getRaw());
  Serial.printlnf("Pressure: %.2f", barSensor.getCompP());

  delay(1000);
}