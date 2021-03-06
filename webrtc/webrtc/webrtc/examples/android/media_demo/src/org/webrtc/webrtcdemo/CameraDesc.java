/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

package org.webrtc.webrtcdemo;

public class CameraDesc {
  private final long nativeCameraDesc;

  // CameraDesc can only be created from the native layer.
  private CameraDesc(long nativeCameraDesc) {
    this.nativeCameraDesc = nativeCameraDesc;
  }

  // Dispose must be called before all references to CameraDesc are lost as it
  // will free memory allocated in the native layer.
  public native void dispose();
}