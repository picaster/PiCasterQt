#ifndef MICBUTTON_H
#define MICBUTTON_H

#include <QPushButton>

class MicButton : public QPushButton {
  Q_OBJECT
 public:
  MicButton(QWidget* parent = nullptr);

 private:
  bool open = false;
  QColor defautButtonBackgroundColor;
};

#endif  // MICBUTTON_H
