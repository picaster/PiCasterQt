#include "jackfadermodule.h"
#include "jackclient.h"
#include "jackport.h"

#include <math.h>

JackFaderModule::JackFaderModule(QString prefix, JackClient* jackClient)
    : JackModule() {
  connect(SignalBus::getInstance(), &SignalBus::micDialChanged,
          [=](long double db_value) {
            this->setAmplification(pow(10, db_value / 20.0l));
          });

  connect(SignalBus::getInstance(), &SignalBus::muted,
          [=]() { this->muted = true; });

  connect(SignalBus::getInstance(), &SignalBus::unMuted,
          [=]() { this->muted = false; });

  inLeft = jackClient->registerPort(prefix + "_in_left", JackPortIsInput);
  inRight = jackClient->registerPort(prefix + "_in_right", JackPortIsInput);
  outLeft = jackClient->registerPort(prefix + "_out_left", JackPortIsOutput);
  outRight = jackClient->registerPort(prefix + "_out_right", JackPortIsOutput);

  auto playback_1 = jackClient->getSystemPort("playback_1");
  auto playback_2 = jackClient->getSystemPort("playback_2");
  auto capture_1 = jackClient->getSystemPort("capture_1");
  auto capture_2 = jackClient->getSystemPort("capture_2");

  jackClient->connectPorts(capture_1, inLeft);
  jackClient->connectPorts(capture_2, inRight);
  jackClient->connectPorts(outLeft, playback_1);
  jackClient->connectPorts(outRight, playback_2);

  jackClient->registerModule(this);
}

void JackFaderModule::process(const jack_nframes_t nframes) const {
  auto micAmplification = muted ? 0.0l : this->amplification;

  auto micInLeftBuffer = inLeft->getBuffer(nframes);
  auto micInRightBuffer = inRight->getBuffer(nframes);
  auto micOutLeftBuffer = outLeft->getBuffer(nframes);
  auto micOutRightBuffer = outRight->getBuffer(nframes);

  for (jack_nframes_t frame = 0; frame < nframes; frame++) {
    micOutLeftBuffer[frame] =
        static_cast<float>(micAmplification) * micInLeftBuffer[frame];
    micOutRightBuffer[frame] =
        static_cast<float>(micAmplification) * micInRightBuffer[frame];
  }
}

void JackFaderModule::mute() { muted = true; }

void JackFaderModule::unmute() { muted = false; }

void JackFaderModule::setAmplification(long double amplification) {
  this->amplification = amplification;
}
