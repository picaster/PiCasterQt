#include "exitbutton.h"
#include "utils/signalbus.h"

ExitButton::ExitButton(QWidget* parent) : QPushButton(parent) {
  /* My own signals */
  connect(this, &QPushButton::clicked,
          [=]() { emit SignalBus::getInstance()->exit(); });

  /* Other signals */
  connect(SignalBus::getInstance(), &SignalBus::jackStarted,
          [=]() { this->setEnabled(false); });
  connect(SignalBus::getInstance(), &SignalBus::jackStopped,
          [=]() { this->setEnabled(true); });
}
