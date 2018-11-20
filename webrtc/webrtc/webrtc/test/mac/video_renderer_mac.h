/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_VIDEO_ENGINE_TEST_COMMON_MAC_VIDEO_RENDERER_MAC_H_
#define WEBRTC_VIDEO_ENGINE_TEST_COMMON_MAC_VIDEO_RENDERER_MAC_H_

#include "webrtc/base/constructormagic.h"
#include "webrtc/test/gl/gl_renderer.h"

@class CocoaWindow;

namespace webrtc {
namespace test {

class MacRenderer : public GlRenderer {
 public:
  MacRenderer();
  virtual ~MacRenderer();

  bool Init(const char* window_title, int width, int height);

  // Implements GlRenderer.
  virtual void RenderFrame(const I420VideoFrame& frame, int delta) OVERRIDE;

 private:
  CocoaWindow* window_;

  DISALLOW_COPY_AND_ASSIGN(MacRenderer);
};
}  // test
}  // webrtc

#endif  // WEBRTC_VIDEO_ENGINE_TEST_COMMON_MAC_VIDEO_RENDERER_MAC_H_
