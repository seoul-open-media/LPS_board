// Read the EBIMU data and send the float in union
//https://forum.arduino.cc/t/send-and-receive-array-of-floats-with-i2c-wire-between-two-arduino/295915/3
#include <Wire.h>
#include <SoftwareSerial.h>

#define SBUF_SIZE 64

char sbuf[SBUF_SIZE];
signed int sbuf_cnt = 0;

SoftwareSerial softSerial(5, 4); // RX, TX

int EBimuAsciiParser(float *item, int number_of_item)
{
  int n, i;
  int rbytes;
  char *addr;
  int result = 0;

  rbytes = softSerial.available();
  for (n = 0; n < rbytes; n++)
  {
    sbuf[sbuf_cnt] = softSerial.read();
    if (sbuf[sbuf_cnt] == 0x0a)
    {
      addr = strtok(sbuf, ",");
      for (i = 0; i < number_of_item; i++)
      {
        item[i] = atof(addr);
        addr = strtok(NULL, ",");
      }

      result = 1;

      // softSerial.print("\n\r");
      // for(i=0;i<number_of_item;i++)  {  softSerial.print(item[i]);  Serial.print(" "); }
    }
    else if (sbuf[sbuf_cnt] == '*')
    { sbuf_cnt = -1;
    }

    sbuf_cnt++;
    if (sbuf_cnt >= SBUF_SIZE) sbuf_cnt = 0;
  }

  return result;
}

union myData_UNION {

  char buffer[4];
  float eulerReading;

};

myData_UNION myData;
myData_UNION * pMyData;

/////////////////////// EBIMU FUNCTION /////////////////////////////
////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);
  softSerial.begin(57600); // Set the baudrate with GTKTERM <lf> <sb4>
  Serial.println("EBIMU test");
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  float euler[3];

  if (EBimuAsciiParser(euler, 3))
  {
    //     Serial.print("\n\r");
    //     Serial.print(euler[0]);   Serial.print(" ");
    //     Serial.print(euler[1]);   Serial.print(" ");
    //     Serial.print(euler[2]);   Serial.print(" ");
    myData.eulerReading = euler[2];


  }
}

void requestEvent()
{
  Wire.write(myData.buffer, 4);

}
