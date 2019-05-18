#ifndef JACKSLIDERMODULE_H
#define JACKSLIDERMODULE_H

#include <QString>

#include "jackmodule.h"

class JackPort;

class JackFaderModule : public JackModule {
  Q_OBJECT

 private:
  JackPort* inLeft;
  JackPort* inRight;
  JackPort* outLeft;
  JackPort* outRight;
  bool muted = true;
  long double amplification = 1.0l;

 public:
  JackFaderModule(QString prefix, JackClient* jackClient);
  void process(const jack_nframes_t nframes) const override;
  void mute();
  void unmute();
  void setAmplification(long double amplification);
};

#endif  // JACKSLIDERMODULE_H
