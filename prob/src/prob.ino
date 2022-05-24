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

const size_t SCAN_RESULT_COUNT = 20;
const unsigned long SCAN_PERIOD_MS = 2000;
BleScanResult scanResult[SCAN_RESULT_COUNT];
BleUuid weatherStationUUID("6895874e-1fd2-4d28-90a8-df56297e33c8");
BleUuid lightCharUUID("e750e756-9c92-4bdb-bf59-0a2946ef5000");
BleUuid baroPresCharUUID("96fb198d-a3cf-451f-9100-dcb69cbee828");
BleUuid baroTempCharUUID("c31127fb-dbff-45d1-a23d-d0cb3b680238");
BleUuid humCharUUID("55769cfd-4d3d-4b48-89d1-f5d258f566a2");
BleUuid tempCharUUID("e2cd64e2-9ab3-4dd5-b40c-7dd6b62c16d5");
BleUuid windDirCharUUID("3ee0aa80-8d59-4532-bdf1-7311f668de3a");
BleUuid windSpeedCharUUID("4d4f0acc-47cd-4eb6-b1c7-83be045d377e");
BleUuid rainCharUUID("0f381db9-9f9e-4190-b865-73f71ae01cb5");
BleCharacteristic lightCharacteristic("Light", BleCharacteristicProperty::NOTIFY, lightCharUUID, weatherStationUUID);
BleCharacteristic baroPresCharacteristic("Baro Pres", BleCharacteristicProperty::NOTIFY, baroPresCharUUID, weatherStationUUID);
BleCharacteristic baroTempCharacteristic("Baro Temp", BleCharacteristicProperty::NOTIFY, baroTempCharUUID, weatherStationUUID);
BleCharacteristic humCharacteristic("Hum", BleCharacteristicProperty::NOTIFY, humCharUUID, weatherStationUUID);
BleCharacteristic tempCharacteristic("Temp", BleCharacteristicProperty::NOTIFY, tempCharUUID, weatherStationUUID);
BleCharacteristic windDirCharacteristic("Wind Dir", BleCharacteristicProperty::NOTIFY, windDirCharUUID, weatherStationUUID);
BleCharacteristic windSpeedCharacteristic("Wind Speed", BleCharacteristicProperty::NOTIFY, windSpeedCharUUID, weatherStationUUID);
BleCharacteristic rainCharacteristic("Rain", BleCharacteristicProperty::NOTIFY, rainCharUUID, weatherStationUUID);
// BleCharacteristic baseCharacteristic;
BlePeerDevice peer;

#if MODE == SLAVE
  LightSensor lightSensor(A0);
  BarSensor barSensor(0x77, 524288);
  HumSensor humSensor(D4);
  WindSensor windSensor(A2, A3);
  RainSensor rainSensor(A5);
#endif

// void baseCharHandler(const uint8_t* data, size_t len, const BlePeerDevice &peer, void* context)
// {

// }

void lightCallback(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context)
{
  Serial.println("Light value changed!");
}

void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected, 30000);

  BLE.on();

#if MODE == SLAVE
  Wire.begin();
  barSensor.init();

  BLE.addCharacteristic(lightCharacteristic);
  BLE.addCharacteristic(baroPresCharacteristic);
  BLE.addCharacteristic(baroTempCharacteristic);
  BLE.addCharacteristic(humCharacteristic);
  BLE.addCharacteristic(tempCharacteristic);
  BLE.addCharacteristic(windDirCharacteristic);
  BLE.addCharacteristic(windSpeedCharacteristic);
  BLE.addCharacteristic(rainCharacteristic);
  BleAdvertisingData advData;
  advData.appendServiceUUID(weatherStationUUID);
  BLE.advertise(&advData);

#else

  // baseCharacteristic.onDataReceived(baseCharHandler, &baseCharacteristic);

  while(!BLE.connected())
  {
    static unsigned long lastScan = 0;

    if (millis() - lastScan >= SCAN_PERIOD_MS)
    {
      lastScan = millis();
      size_t count = BLE.scan(scanResult, SCAN_RESULT_COUNT);

      if(count > 0)
      {
        for(unsigned int i = 0; i < count; i++)
        {
          BleUuid foundServiceUUID;
          size_t svcCount = scanResult[i].advertisingData().serviceUUID(&foundServiceUUID, 1);

          if (svcCount > 0 && foundServiceUUID == weatherStationUUID)
          {
            Serial.write("Found you!");
            peer = BLE.connect(scanResult[i].address());

            lightCharacteristic.onDataReceived(lightCallback, &lightCharacteristic);

            break;
          }
        }
      }
    }
  }
#endif
}

void loop() {
#if MODE == SLAVE
  delay(3000);
  
  data.lightData = lightSensor.getRaw();
  lightCharacteristic.setValue(data.lightData);

  data.barPresData = barSensor.getCompP(true);
  data.barTempData = barSensor.getCompT(false);
  data.humData = humSensor.getHumidity(true);
  data.tempData = humSensor.getTemperature(false);
  data.windDirData = windSensor.getDir();
  data.windSpeedData = windSensor.getSpeed();
  data.rainData = rainSensor.getRain();

  // if(uart.checkRequest())
  // {
  //   Serial.printlnf("Light: %d", data.lightData);
  //   Serial.printlnf("Pressure: %.2f", data.barPresData);
  //   Serial.printlnf("Baro Temperature: %.2f", data.barTempData);
  //   Serial.printlnf("Humidity: %.2f", data.humData);
  //   Serial.printlnf("Temperature: %.2f", data.tempData);
  //   Serial.printlnf("Wind Direction: %d", data.windDirData);
  //   Serial.printlnf("Wind Speed: %.2f", data.windSpeedData);
  //   Serial.printlnf("Rain: %.2f", data.rainData);
  // }
#else
  delay(5000);
  // uart.requestData();

  Serial.printlnf("Light: %d", data.lightData);
  Serial.printlnf("Pressure: %.2f", data.barPresData);
  Serial.printlnf("Baro Temperature: %.2f", data.barTempData);
  Serial.printlnf("Humidity: %.2f", data.humData);
  Serial.printlnf("Temperature: %.2f", data.tempData);
  Serial.printlnf("Wind Direction: %d", data.windDirData);
  Serial.printlnf("Wind Speed: %.2f", data.windSpeedData);
  Serial.printlnf("Rain: %.2f", data.rainData);
#endif
}