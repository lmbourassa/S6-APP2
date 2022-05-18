/*
 * Project prob
 * Description:
 * Author:
 * Date:
 */

SYSTEM_THREAD(ENABLED);

int lightSensor = A0;
int barSensorAddr = 0x77;
int presData, tempData;
int c00, c10, c20, c30, c01, c11, c21;
int k = 524288; // default oversampling

void getCoefs()
{
  char coefs[15];

  Wire.beginTransmission(barSensorAddr);
  Wire.write(0x13);
  Wire.endTransmission(false);
  Wire.requestFrom(barSensorAddr, 15);

  int i = 0;
  while(Wire.available())
  {
    coefs[i++] = Wire.read();
  }

  Wire.endTransmission();

  c00 = (coefs[0]<<12) | (coefs[1]<<4) | (coefs[2]>>4);
  c10 = ((coefs[2] | 0x0F)<<16) | (coefs[3]<<8) | coefs[4];
  c20 = (coefs[9]<<8) | coefs[10];
  c30 = (coefs[13]<<8) | coefs[14];
  c01 = (coefs[5]<<8) | coefs[6];
  c11 = (coefs[7]<<8) | coefs[8];
  c21 = (coefs[11]<<8) | coefs[12];

  Serial.printlnf("c00: %d", c00);
  Serial.printlnf("c10: %d", c10);
  Serial.printlnf("c20: %d", c20);
  Serial.printlnf("c30: %d", c30);
  Serial.printlnf("c01: %d", c01);
  Serial.printlnf("c11: %d", c11);
  Serial.printlnf("c21: %d", c21);
}

float calculatePComp()
{
  float t = (float)tempData/k;
  float p = (float)presData/k;

  return c00+p*(c10+p*(c20+p*c30))+t*c01+t*p*(c11+p*c21);
}

// float calculateTComp()
// {
//   float t = (float)tempData/k;

//   return
// }

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  waitFor(Serial.isConnected, 30000);

  Wire.begin();

  getCoefs();

  Wire.beginTransmission(barSensorAddr);
  Wire.write(0x08);
  Wire.write(0x07);
  Wire.endTransmission();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  int lightData = analogRead(lightSensor);

  Serial.printlnf("Light: %d", lightData);

  Wire.beginTransmission(barSensorAddr);
  Wire.write(0x00);
  Wire.endTransmission(false);

  Wire.requestFrom(barSensorAddr, 6);

  int i = 0;
  presData = 0;
  tempData = 0;

  while(Wire.available())
  {
    char c = Wire.read();
    
    if(i < 3)
    {
      presData<<=8;
      presData = presData|c;
    }
    else if((i >= 3) && (i < 6))
    {
      tempData<<=8;
      tempData = tempData|c;
    }

    i++;
  }

  Wire.endTransmission();

  Serial.printlnf("Pres: %d", presData);
  Serial.printlnf("Temp: %d", tempData);

  Serial.printlnf("P Comp: %f", calculatePComp());

  delay(1000);
}