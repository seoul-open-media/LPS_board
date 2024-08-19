#include <Wire.h>
#include <SoftwareSerial.h>

#include <SPI.h>
#include <DW1000Ng.hpp>
#include <DW1000NgUtils.hpp>
#include <DW1000NgRanging.hpp>
#include <DW1000NgTime.hpp>
#include <DW1000NgConstants.hpp>

#define SBUF_SIZE 64

#define MY_ADDRESS 1
#define NETWORK_ID 10
#define TOTAL_NUM_ANCHORS 3
#define POLL_ACK_CHECK_THRESHOLD 8

// serial message flag
#define SET_DEFAULT_VALUE 0
#define SEND_SONG_INFO 1
#define FINAL_RESULT 2


byte my_address, network_id, total_num_headphones, poll_ack_check_threshold;
//#define my_address 1
//#define poll_ack_check_threshold 4//////////////////////////11ms! total around 270ms.
#define  resetPeriod 8 ///////////////////////////////// 8ms. is the minimum
#define r_resetPeriod 20



byte i_destination_address = 1;
byte r_destination_address = 1;

#define master_address 255




#define LEN_DATA 24
byte d_data[LEN_DATA];
byte i_data[LEN_DATA];
byte r_data[LEN_DATA];
int distance_result[TOTAL_NUM_ANCHORS];  // buffer to store ranging data
byte failure_counter[TOTAL_NUM_ANCHORS];






#define initiator_mode 1
#define responder_mode 2
#define exchange_finish_mode 4

int32_t lasttimesent = 0;


byte my_mode, poll_count;
boolean ok_send_poll, received_poll_ack = false;

// connection pins
const uint8_t PIN_RST = 3; // reset pin
const uint8_t PIN_IRQ = 2; // irq pin
const uint8_t PIN_SS = 10; // spi select pin

// TODO replace by enum
#define POLL 0
#define POLL_ACK 1
#define RANGE 2
#define RANGE_REPORT 3
#define RANGE_FAILED 255
// message flow state
volatile byte expectedMsgId = POLL_ACK;
// message sent/received state
volatile boolean sentAck = false;
volatile boolean receivedAck = false;
volatile boolean receivedMessage = false;
boolean protocolFailed = false;

// timestamps to remember
uint64_t timePollSent;
uint64_t timePollAckReceived;
uint64_t timeRangeSent;

//responder only
uint64_t timePollReceived;
uint64_t timePollAckSent;
uint64_t timeRangeReceived;

// watchdog and reset period
uint32_t lastActivity , time_poll_sent, time_stamp;


// reply times (same on both sides for symm. ranging)
uint16_t replyDelayTimeUS = 3000;
// ranging counter (per second)
uint16_t successRangingCount = 0;
uint32_t rangingCountPeriod = 0;
float samplingRate = 0;

device_configuration_t DEFAULT_CONFIG = {
  false,
  true,
  true,
  true,
  false,
  SFDMode::STANDARD_SFD,
  Channel::CHANNEL_5,
  DataRate::RATE_850KBPS,
  PulseFrequency::FREQ_16MHZ,
  PreambleLength::LEN_256,
  PreambleCode::CODE_3
};

interrupt_configuration_t DEFAULT_INTERRUPT_CONFIG = {
  true,
  true,
  true,
  false,
  true
};

const int DEVICE = 8;
const int VEC_MAX = 6;
//d1 , d2 , d3, euler[0], euler[1],euler[2],
float vec[VEC_MAX] = {0,   0,   0,     0,        0,      0};



char sbuf[SBUF_SIZE];
signed int sbuf_cnt = 0;

SoftwareSerial softSerial(5, 4); // RX, TX

void setup() {
  Wire.begin(DEVICE);
  Wire.onRequest(requestEvent);

  Serial.begin(57600);
  softSerial.begin(57600); // Set the baudrate with GTKTERM <lf> <sb4>

  pinMode(13, OUTPUT);

  // my_address, network_id, total_num_headphones, poll_ack_check_threshold;

  my_address = MY_ADDRESS;
  network_id = NETWORK_ID;
  total_num_headphones = TOTAL_NUM_ANCHORS;
  poll_ack_check_threshold = POLL_ACK_CHECK_THRESHOLD;

  delay(1000);

  // Serial.println(F("### DW1000Ng-arduino-ranging-Initiator ###"));
  // initialize the driver
  DW1000Ng::initialize(PIN_SS, PIN_IRQ, PIN_RST);
  // Serial.println("DW1000Ng initialized ...");
  // general configuration
  DW1000Ng::applyConfiguration(DEFAULT_CONFIG);
  DW1000Ng::applyInterruptConfiguration(DEFAULT_INTERRUPT_CONFIG);

  DW1000Ng::setDeviceAddress(my_address);
  DW1000Ng::setNetworkId(network_id);
  DW1000Ng::setAntennaDelay(16436);

  DW1000Ng::setTXPower(522133279);  // 0x1F1F1F1F
  // Serial.println(F("Committed configuration ..."));
  // DEBUG chip info and registers pretty printed
  /*
    delay(1000);
    char msg[128];
    DW1000Ng::getPrintableDeviceIdentifier(msg);
    Serial.print("Device ID: "); Serial.println(msg);
    DW1000Ng::getPrintableExtendedUniqueIdentifier(msg);
    Serial.print("Unique ID: "); Serial.println(msg);
    DW1000Ng::getPrintableNetworkIdAndShortAddress(msg);
    Serial.print("Network ID & Device Address: "); Serial.println(msg);
    DW1000Ng::getPrintableDeviceMode(msg);
    Serial.print("Device mode: "); Serial.println(msg);
  */

  // attach callback for (successfully) sent and received messages
  DW1000Ng::attachSentHandler(handleSent);
  DW1000Ng::attachReceivedHandler(handleReceived);
  // anchor starts by transmitting a POLL message
  receiver();
  noteActivity();
  initializeDataBuffer();
  //my_mode = default_mode;
  my_mode = responder_mode;
}

void loop() {
//  float euler[3];
//
//  if (EBimuAsciiParser(euler, 3))
//  {
//    Serial.print("\n\r");
//    Serial.print(euler[0]);   Serial.print(" ");
//    Serial.print(euler[1]);   Serial.print(" ");
//    Serial.print(euler[2]);   Serial.print(" ");
//    vec[3] = euler[0];
//    vec[4] = euler[1];
//    vec[5] = euler[2];
//  }

  while (my_mode == initiator_mode){
    twrInitiator();
  }
  while (my_mode == responder_mode){
    twrResponder();
  }
}

void requestEvent() {
  Wire.write((uint8_t*) vec, sizeof(vec));
}

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
