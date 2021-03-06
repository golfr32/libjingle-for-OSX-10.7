/*
 * libjingle
 * Copyright 2012, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// This file contains interfaces for MediaStream and MediaTrack. These
// interfaces are used for implementing MediaStream and MediaTrack as defined
// in http://dev.w3.org/2011/webrtc/editor/webrtc.html#stream-api. These
// interfaces must be used only with PeerConnection. PeerConnectionManager
// interface provides the factory methods to create MediaStream and MediaTracks.

#ifndef TALK_APP_WEBRTC_MEDIASTREAMINTERFACE_H_
#define TALK_APP_WEBRTC_MEDIASTREAMINTERFACE_H_

#include <string>

#include "talk/base/basictypes.h"
#include "talk/base/refcount.h"
#include "talk/base/scoped_ref_ptr.h"

namespace cricket {

class VideoCapturer;
class VideoRenderer;
class VideoFrame;

}  // namespace cricket

namespace webrtc {

class AudioDeviceModule;

// Generic observer interface.
class ObserverInterface {
 public:
  virtual void OnChanged() = 0;

 protected:
  virtual ~ObserverInterface() {}
};

class NotifierInterface {
 public:
  virtual void RegisterObserver(ObserverInterface* observer) = 0;
  virtual void UnregisterObserver(ObserverInterface* observer) = 0;

  virtual ~NotifierInterface() {}
};

// Information about a track.
class MediaStreamTrackInterface : public talk_base::RefCountInterface,
                                  public NotifierInterface {
 public:
  enum TrackState {
    kInitializing,  // Track is beeing negotiated.
    kLive = 1,  // Track alive
    kEnded = 2,  // Track have ended
    kFailed = 3,  // Track negotiation failed.
  };

  virtual std::string kind() const = 0;
  virtual std::string label() const = 0;
  virtual bool enabled() const = 0;
  virtual TrackState state() const = 0;
  virtual bool set_enabled(bool enable) = 0;
  // These methods should be called by implementation only.
  virtual bool set_state(TrackState new_state) = 0;
};

// Interface for rendering VideoFrames from a VideoTrack
class VideoRendererInterface {
 public:
  virtual void SetSize(int width, int height) = 0;
  virtual void RenderFrame(const cricket::VideoFrame* frame) = 0;

 protected:
  // The destructor is protected to prevent deletion via the interface.
  // This is so that we allow reference counted classes, where the destructor
  // should never be public, to implement the interface.
  virtual ~VideoRendererInterface() {}
};

class VideoTrackInterface : public MediaStreamTrackInterface {
 public:
  // Register a renderer that will render all frames received on this track.
  virtual void AddRenderer(VideoRendererInterface* renderer) = 0;
  // Deregister a renderer.
  virtual void RemoveRenderer(VideoRendererInterface* renderer) = 0;

  // Gets a pointer to the frame input of this VideoTrack.
  // The pointer is valid for the lifetime of this VideoTrack.
  // VideoFrames rendered to the cricket::VideoRenderer will be rendered on all
  // registered renderers.
  virtual cricket::VideoRenderer* FrameInput() = 0;

 protected:
  virtual ~VideoTrackInterface() {}
};

class LocalVideoTrackInterface : public VideoTrackInterface {
 public:
  // Get the VideoCapturer associated with the track.
  virtual cricket::VideoCapturer* GetVideoCapture() = 0;

 protected:
  virtual ~LocalVideoTrackInterface() {}
};

class AudioTrackInterface : public MediaStreamTrackInterface {
 public:
 protected:
  virtual ~AudioTrackInterface() {}
};

class LocalAudioTrackInterface : public AudioTrackInterface {
 public:
  // Get the AudioDeviceModule associated with this track.
  virtual AudioDeviceModule* GetAudioDevice() =  0;
 protected:
  virtual ~LocalAudioTrackInterface() {}
};

// List of of tracks.
template <class TrackType>
class MediaStreamTrackListInterface : public talk_base::RefCountInterface {
 public:
  virtual size_t count() const = 0;
  virtual TrackType* at(size_t index) = 0;

 protected:
  virtual ~MediaStreamTrackListInterface() {}
};

typedef MediaStreamTrackListInterface<AudioTrackInterface> AudioTracks;
typedef MediaStreamTrackListInterface<VideoTrackInterface> VideoTracks;

class MediaStreamInterface : public talk_base::RefCountInterface,
                             public NotifierInterface {
 public:
  virtual std::string label() const = 0;
  virtual AudioTracks* audio_tracks() = 0;
  virtual VideoTracks* video_tracks() = 0;

  enum ReadyState {
    kInitializing,
    kLive = 1,  // Stream alive
    kEnded = 2,  // Stream have ended
  };

  virtual ReadyState ready_state() const = 0;

  // These methods should be called by implementation only.
  virtual void set_ready_state(ReadyState state) = 0;

 protected:
  virtual ~MediaStreamInterface() {}
};

class LocalMediaStreamInterface : public MediaStreamInterface {
 public:
  virtual bool AddTrack(AudioTrackInterface* track) = 0;
  virtual bool AddTrack(VideoTrackInterface* track) = 0;
};

}  // namespace webrtc

#endif  // TALK_APP_WEBRTC_MEDIASTREAMINTERFACE_H_
