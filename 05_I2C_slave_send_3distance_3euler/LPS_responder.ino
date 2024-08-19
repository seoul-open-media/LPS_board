
void twrResponder() {
  int32_t curMillis = millis();

  if (!sentAck && !receivedAck) {

    ////////////////////
   // getSerialData();
    ////////////////////

    // check if inactive
    if (curMillis - lastActivity > r_resetPeriod) {
      r_resetInactive();
    }
    return;
  }


  // continue on any success confirmation
  if (sentAck) {
    sentAck = false;
    byte msgId = r_data[0];
    if (msgId == POLL_ACK) {
      timePollAckSent = DW1000Ng::getTransmitTimestamp();
      noteActivity();
    }
    DW1000Ng::startReceive();
  }
  if (receivedAck) {
    receivedAck = false;
    // get message and parse
    DW1000Ng::getReceivedData(r_data, LEN_DATA);
    byte msgId = r_data[0];
    byte mode_from_sender = r_data[16];
    byte to_address = r_data[17];
    byte from_address = r_data[18];
    byte _message = r_data[19];
    //Serial.print("responder Received message from");
    //Serial.println(from_address);
    //Serial.print("msgId"); Serial.print(msgId); Serial.print("mode"); Serial.print(mode_from_sender); Serial.print("to_address"); Serial.print(to_address); Serial.print("from_address"); Serial.print(from_address); Serial.print("_message"); Serial.println(_message);
    ////////////if the master's message matches my address, switch to initiator_mode
    if (from_address == master_address) {
      if (_message == my_address) {
        // be prepared and change to initiator mode
        ok_send_poll = true;
        i_destination_address = 1; // 1~24

        expectedMsgId = POLL_ACK;
        my_mode = initiator_mode;
        // we don't need to get ready to message because we will send message in initiator mode
        // DW1000Ng::startReceive();

        noteActivity();
        time_stamp = millis();
        return;
      } else if (_message != my_address) {
        //  else change to responder_mode
        // Serial.println("Not for me  and I remain in responder mode");
        r_resetInactive();
        noteActivity();
        return;
      }

    }

    ////////
    if (to_address != my_address) {
      r_resetInactive();
      noteActivity();
      return;
    }
    /////////

    ///////////////
  }
}
