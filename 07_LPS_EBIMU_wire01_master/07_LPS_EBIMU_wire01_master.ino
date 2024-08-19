#include <Wire.h>

const int DEVICE = 8;
const int MASTER = 9;

void setup() {
  Wire1.begin(MASTER);
  Wire.setClock(10000);
  Wire1.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {

}

void receiveEvent() {

  const int VEC_MAX = 6;
  float vec[VEC_MAX];
  uint8_t* vp = (uint8_t*) vec;

  // Wire.requestFrom(DEVICE, sizeof(vec));
  while (Wire1.available()) {
    *vp++ = Wire1.read();
  }

  for (int i = 0; i < VEC_MAX; i++) {
    Serial.print(vec[i]); Serial.print(", ");
  }
  Serial.println();
  delay(50);

}
