#include <Wire.h>

const int DEVICE = 8;
const int VEC_MAX = 6;
float vec[VEC_MAX] = {0.1,0.2,0.3,0.4,0.5,0.6};

void setup() {
  Wire.begin(DEVICE);
  Wire.onRequest(requestEvent);
}

void loop() {
  //delay(1000);
}

void requestEvent() {
  Wire.write((uint8_t*) vec, sizeof(vec));
}
