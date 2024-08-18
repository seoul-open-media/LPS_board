#include <Wire.h>
#include <SoftwareSerial.h>

#define SBUF_SIZE 64

const int DEVICE = 8;
const int VEC_MAX = 4;
float vec[VEC_MAX] = {0.1, 0.2, 0.3, 0.4};



char sbuf[SBUF_SIZE];
signed int sbuf_cnt = 0;

SoftwareSerial softSerial(5, 4); // RX, TX

void setup() {
  Wire.begin(DEVICE);
  Wire.onRequest(requestEvent);
  Serial.begin(115200);
  softSerial.begin(57600); // Set the baudrate with GTKTERM <lf> <sb4>
}

void loop() {
   float euler[3];
  
  if(EBimuAsciiParser(euler, 3))
  {
     Serial.print("\n\r");
     Serial.print(euler[0]);   Serial.print(" ");
     Serial.print(euler[1]);   Serial.print(" ");
     Serial.print(euler[2]);   Serial.print(" ");
     vec[0] = euler[0];
     vec[1] = euler[1];
     vec[2] = euler[2];
  }
}

void requestEvent() {
  Wire.write((uint8_t*) vec, sizeof(vec));
}

int EBimuAsciiParser(float *item, int number_of_item)
{
  int n,i;
  int rbytes;
  char *addr; 
  int result = 0;
  
  rbytes = softSerial.available();
  for(n=0;n<rbytes;n++)
  {
    sbuf[sbuf_cnt] = softSerial.read();
    if(sbuf[sbuf_cnt]==0x0a)
       {
           addr = strtok(sbuf,",");
           for(i=0;i<number_of_item;i++)
           {
              item[i] = atof(addr);
              addr = strtok(NULL,",");
           }

           result = 1;

         // softSerial.print("\n\r");
         // for(i=0;i<number_of_item;i++)  {  softSerial.print(item[i]);  Serial.print(" "); }
       }
     else if(sbuf[sbuf_cnt]=='*')
       {   sbuf_cnt=-1;
       }

     sbuf_cnt++;
     if(sbuf_cnt>=SBUF_SIZE) sbuf_cnt=0;
  }
  
  return result;
}
