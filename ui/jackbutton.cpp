#include "jackbutton.h"

#include "utils/signalbus.h"

JackButton::JackButton(QWidget* parent) : QPushButton(parent) {
  connect(this, &QPushButton::toggled, [=]() {
    if (this->isChecked())
      emit SignalBus::getInstance()->startJack();
    else
      emit SignalBus::getInstance()->stopJack();
  });

  connect(SignalBus::getInstance(), &SignalBus::playlistStarted, [=]() {
    this->playlistStarted = true;
    this->setEnabled(false);
  });

  connect(SignalBus::getInstance(), &SignalBus::playlistStopped, [=]() {
    this->playlistStarted = false;
    this->setEnabled(!this->micOpen);
  });

  connect(SignalBus::getInstance(), &SignalBus::unMuted, [=]() {
    this->micOpen = true;
    this->setEnabled(false);
  });

  connect(SignalBus::getInstance(), &SignalBus::muted, [=]() {
    this->micOpen = false;
    this->setEnabled(!this->playlistStarted);
  });
}
