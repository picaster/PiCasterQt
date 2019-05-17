#include "jackfadermodule.h"
#include "jackclient.h"
#include "jackport.h"

#include <math.h>

JackFaderModule::JackFaderModule(QString prefix, JackClient* jackClient)
  : JackModule()
{
  connect(SignalBus::getInstance(),
          &SignalBus::djVolumeChanged,
          [=](long double db_value) {
            this->setAmplification(pow(10, db_value / 20.0l));
          });

  micInLeft = jackClient->registerPort(prefix + "_in_left", JackPortIsInput);
  micInRight = jackClient->registerPort(prefix + "_in_right", JackPortIsInput);
  micOutLeft = jackClient->registerPort(prefix + "_out_left", JackPortIsOutput);
  micOutRight =
    jackClient->registerPort(prefix + "_out_right", JackPortIsOutput);

  auto playback_1 = jackClient->getSystemPort("playback_1");
  auto playback_2 = jackClient->getSystemPort("playback_2");
  auto capture_1 = jackClient->getSystemPort("capture_1");
  auto capture_2 = jackClient->getSystemPort("capture_2");

  jackClient->connectPorts(capture_1, micInLeft);
  jackClient->connectPorts(capture_2, micInRight);
  jackClient->connectPorts(micOutLeft, playback_1);
  jackClient->connectPorts(micOutRight, playback_2);

  jackClient->registerModule(this);
}

void
JackFaderModule::process(const jack_nframes_t nframes) const
{
  auto micAmplification = muted ? 0.0l : this->amplification;

  auto micInLeftBuffer = micInLeft->getBuffer(nframes);
  auto micInRightBuffer = micInRight->getBuffer(nframes);
  auto micOutLeftBuffer = micOutLeft->getBuffer(nframes);
  auto micOutRightBuffer = micOutRight->getBuffer(nframes);

  for (jack_nframes_t frame = 0; frame < nframes; frame++) {
    micOutLeftBuffer[frame] =
      static_cast<float>(micAmplification) * micInLeftBuffer[frame];
    micOutRightBuffer[frame] =
      static_cast<float>(micAmplification) * micInRightBuffer[frame];
  }
}

void
JackFaderModule::mute()
{
  muted = true;
}

void
JackFaderModule::unmute()
{
  muted = false;
}

void
JackFaderModule::setAmplification(long double amplification)
{
  this->amplification = amplification;
}
