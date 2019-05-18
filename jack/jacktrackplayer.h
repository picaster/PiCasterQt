#ifndef JACKTRACKPLAYER_H
#define JACKTRACKPLAYER_H

#include "jack/jackclient.h"
#include "jack/jackmodule.h"
#include "jack/jackport.h"

class JackTrackPlayer : public JackModule {
 public:
  JackTrackPlayer(QString prefix, JackClient* jackClient);
  virtual void process(const jack_nframes_t nframes) const;

 private:
  JackPort* outLeft;
  JackPort* outRight;
  bool playing;
};

#endif  // JACKTRACKPLAYER_H
