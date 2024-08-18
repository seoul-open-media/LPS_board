// Request sensor data in float every 2sec. and receive&decode it

#include <Wire.h>
#include <Metro.h>

Metro sendDataMetro = Metro(2000);

union myData_UNION {
  char buffer[4];
  float eulerReading;
};

myData_UNION myData;
myData_UNION * pMyData;

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop()
{
  if (sendDataMetro.check() == 1) {
    Wire.requestFrom(8, 4);    // request 4 bytes from slave device #8
    Serial.println(myData.eulerReading);
  }
  if (Wire.available() > 3) // slave may send less than requested
  {
    Wire.readBytes( (byte *) &myData, sizeof(myData));
  }


}
