#include "stopbutton.h"
#include "utils/signalbus.h"

StopButton::StopButton(QWidget* parent) : QPushButton(parent) {
  /* My signal */

  connect(this, &QPushButton::clicked, [=] {
    this->setEnabled(false);
    emit SignalBus::getInstance()->stopPlaylist();
  });

  /* Other signals */

  connect(SignalBus::getInstance(), &SignalBus::jackStopped,
          [=]() { this->setEnabled(false); });

  connect(SignalBus::getInstance(), &SignalBus::playlistStarted,
          [=]() { this->setEnabled(true); });
}
