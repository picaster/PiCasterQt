#ifndef JACKSLIDERMODULE_H
#define JACKSLIDERMODULE_H

#include <QString>

#include "jackmodule.h"

class JackPort;

class JackFaderModule : public JackModule
{
  Q_OBJECT

private:
  JackPort* micInLeft;
  JackPort* micInRight;
  JackPort* micOutLeft;
  JackPort* micOutRight;
  bool muted = false;
  long double amplification = 1.0l;

public:
  JackFaderModule(QString prefix, JackClient* jackClient);
  void process(const jack_nframes_t nframes) const;
  void mute();
  void unmute();
  void setAmplification(long double amplification);
};

#endif // JACKSLIDERMODULE_H
