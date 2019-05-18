#include "playbutton.h"
#include "utils/signalbus.h"

PlayButton::PlayButton(QWidget* parent) : QPushButton(parent) {
  /* My signals */

  connect(this, &QPushButton::toggled, [=]() {
    if (this->isChecked()) {
      this->setEnabled(false);
      emit SignalBus::getInstance()->startPlaylist();
    }
  });

  /* Other signals */

  connect(SignalBus::getInstance(), &SignalBus::jackStarted,
          [=]() { this->setEnabled(true); });

  connect(SignalBus::getInstance(), &SignalBus::jackStopped,
          [=]() { this->setEnabled(false); });

  connect(SignalBus::getInstance(), &SignalBus::playlistStopped, [=]() {
    this->setEnabled(true);
    this->toggle();
  });
}
