void initializeDataBuffer() {
  for (int i = 0; i < LEN_DATA; i++) {
    d_data[i] = 0;
    i_data[i] = 0;
    r_data[i] = 0;
  }

  for (int i = 0; i < 20; i++) {
    failure_counter[i] = 0;
  }
}
void noteActivity() {
  // update activity timestamp, so that we do not reach "resetPeriod"
  lastActivity = millis();
}


void r_resetInactive() {
  // anchor listens for POLL
  expectedMsgId = POLL;
  receiver();
  noteActivity();
  //Serial.println("resetInactive");
}


void handleSent() {
  // status change on sent success
  sentAck = true;
}

void handleReceived() {
  // status change on received success
  receivedAck = true;
  //receivedMessage = true;
}
void receiver() {
  DW1000Ng::forceTRxOff();
  // so we don't need to restart the receiver manually
  DW1000Ng::startReceive();
  //  Serial.println("startReceive");
}
void transmitPoll(byte address) {
  i_data[0] = POLL;
  i_data[16] = my_mode; // initiator_mode
  i_data[17] = address;
  i_data[18] = my_address;
  i_data[19] = 0; // _message 0, in case
  DW1000Ng::setTransmitData(i_data, LEN_DATA);
  DW1000Ng::startTransmit();
  // Serial.print("TransmitPoll to"); Serial.println(address);
}
void transmitRangeReport(float curRange) {
  r_data[0] = RANGE_REPORT;
  r_data[16] = my_mode; // responder_mode
  r_data[17] = r_destination_address;
  r_data[18] = my_address;
  r_data[19] = 0; // _message 0, in case

  // add info

  /////////////////////////////////

  // write final ranging result
  memcpy(r_data + 1, &curRange, 4);
  DW1000Ng::setTransmitData(r_data, LEN_DATA);
  DW1000Ng::startTransmit();
  // Serial.println("transmitRangeReport");
}

void transmitRangeFailed() {
  r_data[0] = RANGE_FAILED;
  r_data[16] = my_mode; // responder_mode
  r_data[17] = r_destination_address;
  r_data[18] = my_address;
  r_data[19] = 0; // _message 0, in case
  DW1000Ng::setTransmitData(r_data, LEN_DATA);
  DW1000Ng::startTransmit();
  //Serial.println("transmitRangeFailed");
}


void transmitPollAck() {
  r_data[0] = POLL_ACK;
  r_data[16] = my_mode; // responder_mode
  r_data[17] = r_destination_address;
  r_data[18] = my_address;
  r_data[19] = 0; // _message 0, in case
  DW1000Ng::setTransmitData(r_data, LEN_DATA);
  DW1000Ng::startTransmit();
  // Serial.println("transmitPollAck");
}


void transmitRange() {
  i_data[0] = RANGE;
  i_data[16] = my_mode; // initiator_mode
  i_data[17] = i_destination_address + 100 ;
  i_data[18] = my_address;
  i_data[19] = 0; // _message 0, in case


  /* Calculation of future time */
  byte futureTimeBytes[LENGTH_TIMESTAMP];

  timeRangeSent = DW1000Ng::getSystemTimestamp();
  timeRangeSent += DW1000NgTime::microsecondsToUWBTime(replyDelayTimeUS);
  DW1000NgUtils::writeValueToBytes(futureTimeBytes, timeRangeSent, LENGTH_TIMESTAMP);
  DW1000Ng::setDelayedTRX(futureTimeBytes);
  timeRangeSent += DW1000Ng::getTxAntennaDelay();

  DW1000NgUtils::writeValueToBytes(i_data + 1, timePollSent, LENGTH_TIMESTAMP);
  DW1000NgUtils::writeValueToBytes(i_data + 6, timePollAckReceived, LENGTH_TIMESTAMP);
  DW1000NgUtils::writeValueToBytes(i_data + 11, timeRangeSent, LENGTH_TIMESTAMP);
  DW1000Ng::setTransmitData(i_data, LEN_DATA);
  DW1000Ng::startTransmit(TransmitMode::DELAYED);
  //  Serial.print("Expect RANGE to be sent @ "); Serial.println(timeRangeSent.getAsFloat());
  // Serial.println("TransmitRange");

}

void printResult() {
  Serial.print("elapsed time: ");
  Serial.println((millis() - time_stamp));
  for (int i = 0; i < total_num_headphones; i++) {
    Serial.print(distance_result[i]);
    Serial.print(", ");
  }
  Serial.println();
}
/*
void sendDistanceData() {

  byte elapsed_time = (millis() - time_stamp);

  byte s_data[23] = {255, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0};
  s_data[1] = FINAL_RESULT;
  memcpy(&s_data[2], distance_result, 20);
  s_data[22] = elapsed_time;
  Serial.write(s_data, 23);

}

void sendResult() {
  byte elapsed_time = (millis() - time_stamp);


  byte lenData = 23;
  byte s_data[lenData];
  s_data[0] = 255;

  s_data[1] = FINAL_RESULT;

  memcpy(&s_data[2], distance_result, 20);

  s_data[22] = elapsed_time;

  Serial.write(s_data, lenData);

}
*/
