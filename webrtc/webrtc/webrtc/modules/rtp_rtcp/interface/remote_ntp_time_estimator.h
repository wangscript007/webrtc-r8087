/*
 *  Copyright (c) 2014 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_RTP_RTCP_INTERFACE_REMOTE_NTP_TIME_ESTIMATOR_H_
#define WEBRTC_MODULES_RTP_RTCP_INTERFACE_REMOTE_NTP_TIME_ESTIMATOR_H_

#include "webrtc/system_wrappers/interface/rtp_to_ntp.h"
#include "webrtc/system_wrappers/interface/scoped_ptr.h"

namespace webrtc {

class Clock;
class TimestampExtrapolator;

// RemoteNtpTimeEstimator can be used to estimate a given RTP timestamp's NTP
// time in local timebase.
// Note that it needs to be trained with at least 2 RTCP SR (by calling
// |UpdateRtcpTimestamp|) before it can be used.
class RemoteNtpTimeEstimator {
 public:
  explicit RemoteNtpTimeEstimator(Clock* clock);

  ~RemoteNtpTimeEstimator();

  // Updates the estimator with round trip time |rtt|, NTP seconds |ntp_secs|,
  // NTP fraction |ntp_frac| and RTP timestamp |rtcp_timestamp|.
  bool UpdateRtcpTimestamp(int64_t rtt, uint32_t ntp_secs, uint32_t ntp_frac,
                           uint32_t rtp_timestamp);

  // Estimates the NTP timestamp in local timebase from |rtp_timestamp|.
  // Returns the NTP timestamp in ms when success. -1 if failed.
  int64_t Estimate(uint32_t rtp_timestamp);

 private:
  Clock* clock_;
  scoped_ptr<TimestampExtrapolator> ts_extrapolator_;
  RtcpList rtcp_list_;
  int64_t last_timing_log_ms_;
  DISALLOW_COPY_AND_ASSIGN(RemoteNtpTimeEstimator);
};

}  // namespace webrtc

#endif  // WEBRTC_MODULES_RTP_RTCP_INTERFACE_REMOTE_NTP_TIME_ESTIMATOR_H_
