#ifndef EXITBUTTON_H
#define EXITBUTTON_H

#include <QPushButton>

class ExitButton : public QPushButton {
  Q_OBJECT
 public:
  explicit ExitButton(QWidget* parent = nullptr);
};

#endif  // EXITBUTTON_H
