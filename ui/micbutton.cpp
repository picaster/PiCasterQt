#include "micbutton.h"
#include "utils/signalbus.h"

MicButton::MicButton(QWidget* parent) : QPushButton(parent) {
  /* My signals */

  connect(this, &QPushButton::toggled, [=]() {
    QPalette pal = this->palette();
    if (this->isChecked()) {
      emit SignalBus::getInstance()->unMuted();
      this->defautButtonBackgroundColor = pal.color(QPalette::Button);
      pal.setColor(QPalette::Button, QColor(Qt::red));
    } else {
      emit SignalBus::getInstance()->muted();
      pal.setColor(QPalette::Button, this->defautButtonBackgroundColor);
    }
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->update();
  });

  /* Other signals */

  connect(SignalBus::getInstance(), &SignalBus::jackStarted,
          [=]() { this->setEnabled(true); });

  connect(SignalBus::getInstance(), &SignalBus::jackStopped,
          [=]() { this->setEnabled(false); });
}
