#include <Wire.h>

const int DEVICE = 8;

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  const int VEC_MAX = 6;
  float vec[VEC_MAX];
  uint8_t* vp = (uint8_t*) vec;

  Wire.requestFrom(DEVICE, sizeof(vec));
  while (Wire.available()) {
    *vp++ = Wire.read();
  }

  for (int i = 0; i < VEC_MAX; i++) {
    Serial.println(vec[i]);
  }

  delay(1000);
}
