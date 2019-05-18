#include <iostream>

#include "jacktrackplayer.h"

JackTrackPlayer::JackTrackPlayer(QString prefix, JackClient* jackClient)
    : JackModule() {
  outLeft = jackClient->registerPort(prefix + "_out_left", JackPortIsOutput);
  outRight = jackClient->registerPort(prefix + "_out_right", JackPortIsOutput);

  auto playback_1 = jackClient->getSystemPort("playback_1");
  auto playback_2 = jackClient->getSystemPort("playback_2");

  jackClient->connectPorts(outLeft, playback_1);
  jackClient->connectPorts(outRight, playback_2);

  jackClient->registerModule(this);

  this->playing = false;
}

void JackTrackPlayer::process(const jack_nframes_t nframes) const {
  std::cerr << "JackTrackPlayer::process" << std::endl;
  if (!this->playing) return;
}
