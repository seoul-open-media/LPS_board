#include <Wire.h>

const int DEVICE = 8;
const int MASTER = 9;

void setup() {
  Wire1.begin(MASTER);
  Wire1.setClock(100000);
  Wire1.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {

  //  const int VEC_MAX = 6;
  //  float vec[VEC_MAX];
  //  uint8_t* vp = (uint8_t*) vec;
  //
  //  Wire1.requestFrom(DEVICE, sizeof(vec));
  //  while (Wire1.available()) {
  //    *vp++ = Wire1.read();
  //  }
  //
  //  for (int i = 0; i < VEC_MAX; i++) {
  //    Serial.print(vec[i]); Serial.print(", ");
  //  }
  //  Serial.println();
  //  delay(50);

}

void receiveEvent() {
  while (Wire1.available()) {
    if (Wire1.read() == 255) {
     // Serial.println("got 255");
      const int VEC_MAX = 6;
      float vec[VEC_MAX];
      uint8_t* vp = (uint8_t*) vec;
      Wire1.requestFrom(DEVICE, sizeof(vec));
      while (Wire1.available()) {
        *vp++ = Wire1.read();
      }

      for (int i = 0; i < VEC_MAX; i++) {
        Serial.print(vec[i]); Serial.print(", ");
      }
      Serial.println();
    //  delay(50);
    }
  }

  //  const int VEC_MAX = 6;
  //  float vec[VEC_MAX];
  //  uint8_t* vp = (uint8_t*) vec;
  //
  // // Wire1.requestFrom(DEVICE, sizeof(vec));
  //  while (Wire1.available()) {
  //    *vp++ = Wire1.read();
  //  }
  //
  //  for (int i = 0; i < VEC_MAX; i++) {
  //    Serial.print(vec[i]); Serial.print(", ");
  //  }
  //  Serial.println();
  //  delay(50);

}
