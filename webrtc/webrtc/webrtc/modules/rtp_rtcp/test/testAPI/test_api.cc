/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "webrtc/modules/rtp_rtcp/test/testAPI/test_api.h"

#include <algorithm>
#include <vector>

using namespace webrtc;

class RtpRtcpAPITest : public ::testing::Test {
 protected:
  RtpRtcpAPITest() : module(NULL), fake_clock(123456) {
    test_csrcs.push_back(1234);
    test_csrcs.push_back(2345);
    test_id = 123;
    test_ssrc = 3456;
    test_timestamp = 4567;
    test_sequence_number = 2345;
  }
  ~RtpRtcpAPITest() {}

  virtual void SetUp() {
    RtpRtcp::Configuration configuration;
    configuration.id = test_id;
    configuration.audio = true;
    configuration.clock = &fake_clock;
    module = RtpRtcp::CreateRtpRtcp(configuration);
    rtp_payload_registry_.reset(new RTPPayloadRegistry(
            RTPPayloadStrategy::CreateStrategy(true)));
    rtp_receiver_.reset(RtpReceiver::CreateAudioReceiver(
        test_id, &fake_clock, NULL, NULL, NULL, rtp_payload_registry_.get()));
  }

  virtual void TearDown() {
    delete module;
  }

  int test_id;
  scoped_ptr<RTPPayloadRegistry> rtp_payload_registry_;
  scoped_ptr<RtpReceiver> rtp_receiver_;
  RtpRtcp* module;
  uint32_t test_ssrc;
  uint32_t test_timestamp;
  uint16_t test_sequence_number;
  std::vector<uint32_t> test_csrcs;
  SimulatedClock fake_clock;
};

TEST_F(RtpRtcpAPITest, Basic) {
  module->SetSequenceNumber(test_sequence_number);
  EXPECT_EQ(test_sequence_number, module->SequenceNumber());

  module->SetStartTimestamp(test_timestamp);
  EXPECT_EQ(test_timestamp, module->StartTimestamp());

  EXPECT_FALSE(module->Sending());
  EXPECT_EQ(0, module->SetSendingStatus(true));
  EXPECT_TRUE(module->Sending());
}

TEST_F(RtpRtcpAPITest, MTU) {
  EXPECT_EQ(-1, module->SetMaxTransferUnit(10));
  EXPECT_EQ(-1, module->SetMaxTransferUnit(IP_PACKET_SIZE + 1));
  EXPECT_EQ(0, module->SetMaxTransferUnit(1234));
  EXPECT_EQ(1234-20-8, module->MaxPayloadLength());

  EXPECT_EQ(0, module->SetTransportOverhead(true, true, 12));
  EXPECT_EQ(1234 - 20- 20 -20 - 12, module->MaxPayloadLength());

  EXPECT_EQ(0, module->SetTransportOverhead(false, false, 0));
  EXPECT_EQ(1234 - 20 - 8, module->MaxPayloadLength());
}

TEST_F(RtpRtcpAPITest, SSRC) {
  module->SetSSRC(test_ssrc);
  EXPECT_EQ(test_ssrc, module->SSRC());
}

TEST_F(RtpRtcpAPITest, RTCP) {
  EXPECT_EQ(kRtcpOff, module->RTCP());
  module->SetRTCPStatus(kRtcpCompound);
  EXPECT_EQ(kRtcpCompound, module->RTCP());

  EXPECT_EQ(0, module->SetCNAME("john.doe@test.test"));

  EXPECT_FALSE(module->TMMBR());
  module->SetTMMBRStatus(true);
  EXPECT_TRUE(module->TMMBR());
  module->SetTMMBRStatus(false);
  EXPECT_FALSE(module->TMMBR());

  EXPECT_EQ(kNackOff, rtp_receiver_->NACK());
  rtp_receiver_->SetNACKStatus(kNackRtcp);
  EXPECT_EQ(kNackRtcp, rtp_receiver_->NACK());
}

TEST_F(RtpRtcpAPITest, RtxSender) {
  module->SetRtxSendStatus(kRtxRetransmitted);
  int rtx_mode = module->RtxSendStatus();
  EXPECT_EQ(kRtxRetransmitted, rtx_mode);

  module->SetRtxSendStatus(kRtxOff);
  rtx_mode = module->RtxSendStatus();
  EXPECT_EQ(kRtxOff, rtx_mode);

  module->SetRtxSendStatus(kRtxRetransmitted);
  rtx_mode = module->RtxSendStatus();
  EXPECT_EQ(kRtxRetransmitted, rtx_mode);
}

TEST_F(RtpRtcpAPITest, RtxReceiver) {
  const uint32_t kRtxSsrc = 1;
  const int kRtxPayloadType = 119;
  EXPECT_FALSE(rtp_payload_registry_->RtxEnabled());
  rtp_payload_registry_->SetRtxSsrc(kRtxSsrc);
  rtp_payload_registry_->SetRtxPayloadType(kRtxPayloadType);
  EXPECT_TRUE(rtp_payload_registry_->RtxEnabled());
  RTPHeader rtx_header;
  rtx_header.ssrc = kRtxSsrc;
  rtx_header.payloadType = kRtxPayloadType;
  EXPECT_TRUE(rtp_payload_registry_->IsRtx(rtx_header));
  rtx_header.ssrc = 0;
  EXPECT_FALSE(rtp_payload_registry_->IsRtx(rtx_header));
  rtx_header.ssrc = kRtxSsrc;
  rtx_header.payloadType = 0;
  EXPECT_TRUE(rtp_payload_registry_->IsRtx(rtx_header));
}
