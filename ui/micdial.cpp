#include "math.h"

#include "micdial.h"
#include "utils/signalbus.h"

MicDial::MicDial(QWidget* parent) : QDial(parent) {
  /* My signals */

  connect(this, &QDial::valueChanged, [=](int value) {
    long double f_value = value / static_cast<long double>(10.0);
    long double db_value = 65 * log10(f_value);
    emit SignalBus::getInstance()->micDialChanged(db_value);
  });

  /* Othe signals */

  connect(SignalBus::getInstance(), &SignalBus::jackStarted,
          [=]() { this->setEnabled(true); });

  connect(SignalBus::getInstance(), &SignalBus::jackStopped,
          [=]() { this->setEnabled(false); });
}
