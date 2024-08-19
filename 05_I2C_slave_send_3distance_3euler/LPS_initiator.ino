void twrInitiator() {
  //


  if (!sentAck && !receivedAck) { // 패킷을 보내고 받는 동안이 아니라면



    //////////////////////////
    if (ok_send_poll == false) { // poll하고나서
      if (millis() - time_poll_sent > poll_ack_check_threshold ) { // 일정시간후에도
        if (received_poll_ack == false) { // poll_ack을 받지 못했다면
          // 그다음 주소로 이동 하여 폴
          distance_result[(i_destination_address - 1)] = 2520;
          i_destination_address++;
          expectedMsgId = POLL_ACK;
          ok_send_poll = true;
          sentAck = false;
          receivedAck = false;
        }
      }
    }

    //////////////////////////

    if (ok_send_poll == true) {
      if (i_destination_address < (TOTAL_NUM_ANCHORS + 1)) { // i_destination_address 1~20
        received_poll_ack = false;
        expectedMsgId = POLL_ACK;
        ok_send_poll = false;
        noteActivity();
        time_poll_sent = millis();
        DW1000Ng::forceTRxOff();
        transmitPoll(i_destination_address + 100);

        return;

      } else {///////////////////////////////////////// we checked all the range


        // be prepared and go out to responder_mode
        ok_send_poll = false;
        expectedMsgId = POLL;
        my_mode = responder_mode;
        DW1000Ng::forceTRxOff();
        DW1000Ng::startReceive();


        //    sendResult();
        vec[0] = distance_result[0];
        vec[1] = distance_result[1];
        vec[2] = distance_result[2];
        
        printResult();


        return;
        ///////////////////////////////////////////////

      }// if the destination headphones is off
    } else if (ok_send_poll == false && (millis() - lastActivity > resetPeriod)) {
      distance_result[(i_destination_address - 1)] = 2530;


      // add the counter
      failure_counter[i_destination_address - 1]++;
      // if it fails 3times then reset song info
      if (failure_counter[i_destination_address - 1] >= 3) {
        failure_counter[i_destination_address - 1] = 0;
      }

      i_destination_address++;
      ok_send_poll = true;
      /*
        Serial.print("i_destination_address");
        Serial.print((i_destination_address - 1)); // since we added 1 before
        Serial.println(" time out");
      */
    }
  }
  // continue on any success confirmation
  if (sentAck) {

    sentAck = false;
    DW1000Ng::startReceive();
  }
  if (receivedAck) {
    receivedAck = false;
    // get message and parse
    DW1000Ng::getReceivedData(i_data, LEN_DATA);
    byte msgId = i_data[0];
    byte mode_from_sender = i_data[16];
    byte to_address = i_data[17];
    byte from_address = i_data[18];
    byte _message = i_data[19];
    //Serial.print("Initiator Received message from");
    //Serial.println(from_address);
    ////////////if the master's message matches my address, switch to initiator_mode
    if (from_address == master_address) {
      if (_message == my_address) {
        // already initiator mode
      } else if (_message != my_address) {
        // else change to responder_mode
        // Serial.write (255);
        // Serial.write (2);

        expectedMsgId = POLL;
        my_mode = responder_mode;
      }
      DW1000Ng::startReceive();
      return;
    }
    /////////////
    ////////
    if (to_address != my_address) {
      r_resetInactive();
      noteActivity();
      return;
    }
    /////////

    ////////////
    if ( mode_from_sender == responder_mode) {
      if (msgId != expectedMsgId) {
        // unexpected message, start over again
        // Serial.print("Received wrong message # "); Serial.println(msgId);
        expectedMsgId = POLL_ACK;
        distance_result[(i_destination_address - 1)] = 2540;
        i_destination_address++;
        ok_send_poll = true;

        return;
      }
      if (msgId == POLL_ACK) {
        received_poll_ack = true;
        timePollSent = DW1000Ng::getTransmitTimestamp();
        timePollAckReceived = DW1000Ng::getReceiveTimestamp();
        expectedMsgId = RANGE_REPORT;
        transmitRange();
        noteActivity();
      } else if (msgId == RANGE_REPORT) {

        expectedMsgId = POLL_ACK;
        float curRange;
        memcpy(&curRange, i_data + 1, 4);

        ////////////////////////////
        int distanceCm = int((curRange / DISTANCE_OF_RADIO_INV) * 100); // distance in cm.

        //////// get distance
        distance_result[(i_destination_address - 1)] = distanceCm;
        i_destination_address++;
        ok_send_poll = true;
        noteActivity();

        /////////////////////////
      } else if (msgId == RANGE_FAILED) {
        expectedMsgId = POLL_ACK;

        distance_result[(i_destination_address - 1)] = 2520;
        i_destination_address++;
        ok_send_poll = true;
        noteActivity();

      }
      /////////////////////////////////////
    }
    ////////////////////////////////////
  }

}
