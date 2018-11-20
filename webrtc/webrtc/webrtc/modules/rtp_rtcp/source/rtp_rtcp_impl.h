/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_RTP_RTCP_SOURCE_RTP_RTCP_IMPL_H_
#define WEBRTC_MODULES_RTP_RTCP_SOURCE_RTP_RTCP_IMPL_H_

#include <list>
#include <vector>

#include "webrtc/modules/rtp_rtcp/interface/rtp_rtcp.h"
#include "webrtc/modules/rtp_rtcp/source/rtcp_receiver.h"
#include "webrtc/modules/rtp_rtcp/source/rtcp_sender.h"
#include "webrtc/modules/rtp_rtcp/source/rtp_sender.h"
#include "webrtc/system_wrappers/interface/scoped_ptr.h"
#include "webrtc/test/testsupport/gtest_prod_util.h"

namespace webrtc {

class ModuleRtpRtcpImpl : public RtpRtcp {
 public:
  explicit ModuleRtpRtcpImpl(const RtpRtcp::Configuration& configuration);

  virtual ~ModuleRtpRtcpImpl();

  // Returns the number of milliseconds until the module want a worker thread to
  // call Process.
  virtual int64_t TimeUntilNextProcess() OVERRIDE;

  // Process any pending tasks such as timeouts.
  virtual int32_t Process() OVERRIDE;

  // Receiver part.

  // Called when we receive an RTCP packet.
  virtual int32_t IncomingRtcpPacket(const uint8_t* incoming_packet,
                                     size_t incoming_packet_length) OVERRIDE;

  virtual void SetRemoteSSRC(uint32_t ssrc) OVERRIDE;

  // Sender part.

  virtual int32_t RegisterSendPayload(const CodecInst& voice_codec) OVERRIDE;

  virtual int32_t RegisterSendPayload(const VideoCodec& video_codec) OVERRIDE;

  virtual int32_t DeRegisterSendPayload(int8_t payload_type) OVERRIDE;

  int8_t SendPayloadType() const;

  // Register RTP header extension.
  virtual int32_t RegisterSendRtpHeaderExtension(RTPExtensionType type,
                                                 uint8_t id) OVERRIDE;

  virtual int32_t DeregisterSendRtpHeaderExtension(
      RTPExtensionType type) OVERRIDE;

  // Get start timestamp.
  virtual uint32_t StartTimestamp() const OVERRIDE;

  // Configure start timestamp, default is a random number.
  virtual void SetStartTimestamp(uint32_t timestamp) OVERRIDE;

  virtual uint16_t SequenceNumber() const OVERRIDE;

  // Set SequenceNumber, default is a random number.
  virtual void SetSequenceNumber(uint16_t seq) OVERRIDE;

  virtual void SetRtpStateForSsrc(uint32_t ssrc,
                                  const RtpState& rtp_state) OVERRIDE;
  virtual bool GetRtpStateForSsrc(uint32_t ssrc, RtpState* rtp_state) OVERRIDE;

  virtual uint32_t SSRC() const OVERRIDE;

  // Configure SSRC, default is a random number.
  virtual void SetSSRC(uint32_t ssrc) OVERRIDE;

  virtual void SetCsrcs(const std::vector<uint32_t>& csrcs) OVERRIDE;

  RTCPSender::FeedbackState GetFeedbackState();

  int CurrentSendFrequencyHz() const;

  virtual void SetRtxSendStatus(int mode) OVERRIDE;
  virtual int RtxSendStatus() const OVERRIDE;

  virtual void SetRtxSsrc(uint32_t ssrc) OVERRIDE;

  virtual void SetRtxSendPayloadType(int payload_type) OVERRIDE;

  // Sends kRtcpByeCode when going from true to false.
  virtual int32_t SetSendingStatus(bool sending) OVERRIDE;

  virtual bool Sending() const OVERRIDE;

  // Drops or relays media packets.
  virtual void SetSendingMediaStatus(bool sending) OVERRIDE;

  virtual bool SendingMedia() const OVERRIDE;

  // Used by the codec module to deliver a video or audio frame for
  // packetization.
  virtual int32_t SendOutgoingData(
      FrameType frame_type,
      int8_t payload_type,
      uint32_t time_stamp,
      int64_t capture_time_ms,
      const uint8_t* payload_data,
      size_t payload_size,
      const RTPFragmentationHeader* fragmentation = NULL,
      const RTPVideoHeader* rtp_video_hdr = NULL) OVERRIDE;

  virtual bool TimeToSendPacket(uint32_t ssrc,
                                uint16_t sequence_number,
                                int64_t capture_time_ms,
                                bool retransmission) OVERRIDE;
  // Returns the number of padding bytes actually sent, which can be more or
  // less than |bytes|.
  virtual size_t TimeToSendPadding(size_t bytes) OVERRIDE;

  virtual bool GetSendSideDelay(int* avg_send_delay_ms,
                                int* max_send_delay_ms) const OVERRIDE;

  // RTCP part.

  // Get RTCP status.
  virtual RTCPMethod RTCP() const OVERRIDE;

  // Configure RTCP status i.e on/off.
  virtual void SetRTCPStatus(RTCPMethod method) OVERRIDE;

  // Set RTCP CName.
  virtual int32_t SetCNAME(const char c_name[RTCP_CNAME_SIZE]) OVERRIDE;

  // Get remote CName.
  virtual int32_t RemoteCNAME(uint32_t remote_ssrc,
                              char c_name[RTCP_CNAME_SIZE]) const OVERRIDE;

  // Get remote NTP.
  virtual int32_t RemoteNTP(uint32_t* received_ntp_secs,
                            uint32_t* received_ntp_frac,
                            uint32_t* rtcp_arrival_time_secs,
                            uint32_t* rtcp_arrival_time_frac,
                            uint32_t* rtcp_timestamp) const OVERRIDE;

  virtual int32_t AddMixedCNAME(uint32_t ssrc,
                                const char c_name[RTCP_CNAME_SIZE]) OVERRIDE;

  virtual int32_t RemoveMixedCNAME(uint32_t ssrc) OVERRIDE;

  // Get RoundTripTime.
  virtual int32_t RTT(uint32_t remote_ssrc,
                      int64_t* rtt,
                      int64_t* avg_rtt,
                      int64_t* min_rtt,
                      int64_t* max_rtt) const OVERRIDE;

  // Force a send of an RTCP packet.
  // Normal SR and RR are triggered via the process function.
  virtual int32_t SendRTCP(uint32_t rtcp_packet_type = kRtcpReport) OVERRIDE;

  virtual int32_t ResetSendDataCountersRTP() OVERRIDE;

  // Statistics of the amount of data sent and received.
  virtual int32_t DataCountersRTP(size_t* bytes_sent,
                                  uint32_t* packets_sent) const OVERRIDE;

  virtual void GetSendStreamDataCounters(
      StreamDataCounters* rtp_counters,
      StreamDataCounters* rtx_counters) const OVERRIDE;

  // Get received RTCP report, sender info.
  virtual int32_t RemoteRTCPStat(RTCPSenderInfo* sender_info) OVERRIDE;

  // Get received RTCP report, report block.
  virtual int32_t RemoteRTCPStat(
      std::vector<RTCPReportBlock>* receive_blocks) const OVERRIDE;

  // Set received RTCP report block.
  virtual int32_t AddRTCPReportBlock(
      uint32_t ssrc,
      const RTCPReportBlock* receive_block) OVERRIDE;

  virtual int32_t RemoveRTCPReportBlock(uint32_t ssrc) OVERRIDE;

  virtual void GetRtcpPacketTypeCounters(
      RtcpPacketTypeCounter* packets_sent,
      RtcpPacketTypeCounter* packets_received) const OVERRIDE;

  // (REMB) Receiver Estimated Max Bitrate.
  virtual bool REMB() const OVERRIDE;

  virtual void SetREMBStatus(bool enable) OVERRIDE;

  virtual void SetREMBData(uint32_t bitrate,
                           const std::vector<uint32_t>& ssrcs) OVERRIDE;

  // (IJ) Extended jitter report.
  virtual bool IJ() const OVERRIDE;

  virtual void SetIJStatus(bool enable) OVERRIDE;

  // (TMMBR) Temporary Max Media Bit Rate.
  virtual bool TMMBR() const OVERRIDE;

  virtual void SetTMMBRStatus(bool enable) OVERRIDE;

  int32_t SetTMMBN(const TMMBRSet* bounding_set);

  virtual uint16_t MaxPayloadLength() const OVERRIDE;

  virtual uint16_t MaxDataPayloadLength() const OVERRIDE;

  virtual int32_t SetMaxTransferUnit(uint16_t size) OVERRIDE;

  virtual int32_t SetTransportOverhead(
      bool tcp,
      bool ipv6,
      uint8_t authentication_overhead = 0) OVERRIDE;

  // (NACK) Negative acknowledgment part.

  virtual int SelectiveRetransmissions() const OVERRIDE;

  virtual int SetSelectiveRetransmissions(uint8_t settings) OVERRIDE;

  // Send a Negative acknowledgment packet.
  virtual int32_t SendNACK(const uint16_t* nack_list, uint16_t size) OVERRIDE;

  // Store the sent packets, needed to answer to a negative acknowledgment
  // requests.
  virtual void SetStorePacketsStatus(bool enable,
                                     uint16_t number_to_store) OVERRIDE;

  virtual bool StorePackets() const OVERRIDE;

  // Called on receipt of RTCP report block from remote side.
  virtual void RegisterRtcpStatisticsCallback(
      RtcpStatisticsCallback* callback) OVERRIDE;
  virtual RtcpStatisticsCallback* GetRtcpStatisticsCallback() OVERRIDE;

  // (APP) Application specific data.
  virtual int32_t SetRTCPApplicationSpecificData(uint8_t sub_type,
                                                 uint32_t name,
                                                 const uint8_t* data,
                                                 uint16_t length) OVERRIDE;

  // (XR) VOIP metric.
  virtual int32_t SetRTCPVoIPMetrics(const RTCPVoIPMetric* VoIPMetric) OVERRIDE;

  // (XR) Receiver reference time report.
  virtual void SetRtcpXrRrtrStatus(bool enable) OVERRIDE;

  virtual bool RtcpXrRrtrStatus() const OVERRIDE;

  // Audio part.

  // Set audio packet size, used to determine when it's time to send a DTMF
  // packet in silence (CNG).
  virtual int32_t SetAudioPacketSize(uint16_t packet_size_samples) OVERRIDE;

  virtual bool SendTelephoneEventActive(int8_t& telephone_event) const OVERRIDE;

  // Send a TelephoneEvent tone using RFC 2833 (4733).
  virtual int32_t SendTelephoneEventOutband(uint8_t key,
                                            uint16_t time_ms,
                                            uint8_t level) OVERRIDE;

  // Set payload type for Redundant Audio Data RFC 2198.
  virtual int32_t SetSendREDPayloadType(int8_t payload_type) OVERRIDE;

  // Get payload type for Redundant Audio Data RFC 2198.
  virtual int32_t SendREDPayloadType(int8_t& payload_type) const OVERRIDE;

  // Store the audio level in d_bov for header-extension-for-audio-level-
  // indication.
  virtual int32_t SetAudioLevel(uint8_t level_d_bov) OVERRIDE;

  // Video part.

  virtual int32_t SendRTCPSliceLossIndication(uint8_t picture_id) OVERRIDE;

  // Set method for requestion a new key frame.
  virtual int32_t SetKeyFrameRequestMethod(
      KeyFrameRequestMethod method) OVERRIDE;

  // Send a request for a keyframe.
  virtual int32_t RequestKeyFrame() OVERRIDE;

  virtual int32_t SetCameraDelay(int32_t delay_ms) OVERRIDE;

  virtual void SetTargetSendBitrate(
      const std::vector<uint32_t>& stream_bitrates) OVERRIDE;

  virtual int32_t SetGenericFECStatus(bool enable,
                                      uint8_t payload_type_red,
                                      uint8_t payload_type_fec) OVERRIDE;

  virtual int32_t GenericFECStatus(
      bool& enable,
      uint8_t& payload_type_red,
      uint8_t& payload_type_fec) OVERRIDE;

  virtual int32_t SetFecParameters(
      const FecProtectionParams* delta_params,
      const FecProtectionParams* key_params) OVERRIDE;

  bool LastReceivedNTP(uint32_t* NTPsecs,
                       uint32_t* NTPfrac,
                       uint32_t* remote_sr) const;

  bool LastReceivedXrReferenceTimeInfo(RtcpReceiveTimeInfo* info) const;

  virtual int32_t BoundingSet(bool& tmmbr_owner, TMMBRSet*& bounding_set_rec);

  virtual void BitrateSent(uint32_t* total_rate,
                           uint32_t* video_rate,
                           uint32_t* fec_rate,
                           uint32_t* nackRate) const OVERRIDE;

  int64_t SendTimeOfSendReport(uint32_t send_report);

  bool SendTimeOfXrRrReport(uint32_t mid_ntp, int64_t* time_ms) const;

  // Good state of RTP receiver inform sender.
  virtual int32_t SendRTCPReferencePictureSelection(
      uint64_t picture_id) OVERRIDE;

  virtual void RegisterSendChannelRtpStatisticsCallback(
      StreamDataCountersCallback* callback) OVERRIDE;
  virtual StreamDataCountersCallback*
      GetSendChannelRtpStatisticsCallback() const OVERRIDE;

  void OnReceivedTMMBR();

  // Bad state of RTP receiver request a keyframe.
  void OnRequestIntraFrame();

  // Received a request for a new SLI.
  void OnReceivedSliceLossIndication(uint8_t picture_id);

  // Received a new reference frame.
  void OnReceivedReferencePictureSelectionIndication(uint64_t picture_id);

  void OnReceivedNACK(const std::list<uint16_t>& nack_sequence_numbers);

  void OnRequestSendReport();

 protected:
  void RegisterChildModule(RtpRtcp* module);

  void DeRegisterChildModule(RtpRtcp* module);

  bool UpdateRTCPReceiveInformationTimers();

  uint32_t BitrateReceivedNow() const;

  // Get remote SequenceNumber.
  uint16_t RemoteSequenceNumber() const;

  // Only for internal testing.
  uint32_t LastSendReport(int64_t& last_rtcptime);

  RTPSender                 rtp_sender_;

  RTCPSender                rtcp_sender_;
  RTCPReceiver              rtcp_receiver_;

  Clock*                    clock_;

 private:
  FRIEND_TEST_ALL_PREFIXES(RtpRtcpImplTest, Rtt);
  FRIEND_TEST_ALL_PREFIXES(RtpRtcpImplTest, RttForReceiverOnly);
  int64_t RtcpReportInterval();
  void SetRtcpReceiverSsrcs(uint32_t main_ssrc);

  void set_rtt_ms(int64_t rtt_ms);
  int64_t rtt_ms() const;

  bool TimeToSendFullNackList(int64_t now) const;

  bool IsDefaultModule() const;

  int32_t             id_;
  const bool                audio_;
  bool                      collision_detected_;
  int64_t             last_process_time_;
  int64_t             last_bitrate_process_time_;
  int64_t             last_rtt_process_time_;
  uint16_t            packet_overhead_;

  scoped_ptr<CriticalSectionWrapper> critical_section_module_ptrs_;
  scoped_ptr<CriticalSectionWrapper> critical_section_module_ptrs_feedback_;
  ModuleRtpRtcpImpl*            default_module_;
  std::vector<ModuleRtpRtcpImpl*> child_modules_;
  size_t padding_index_;

  // Send side
  NACKMethod            nack_method_;
  int64_t nack_last_time_sent_full_;
  uint32_t nack_last_time_sent_full_prev_;
  uint16_t nack_last_seq_number_sent_;

  bool                  simulcast_;
  VideoCodec            send_video_codec_;
  KeyFrameRequestMethod key_frame_req_method_;

  RemoteBitrateEstimator* remote_bitrate_;

  RtcpRttStats* rtt_stats_;

  // The processed RTT from RtcpRttStats.
  scoped_ptr<CriticalSectionWrapper> critical_section_rtt_;
  int64_t rtt_ms_;
};

}  // namespace webrtc

#endif  // WEBRTC_MODULES_RTP_RTCP_SOURCE_RTP_RTCP_IMPL_H_
