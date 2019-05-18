#ifndef JACKBUTTON_H
#define JACKBUTTON_H

#include <QPushButton>

class JackButton : public QPushButton {
  Q_OBJECT
 public:
  explicit JackButton(QWidget* parent = nullptr);

 private:
  bool playlistStarted = false;
  bool micOpen = false;
};

#endif  // JACKBUTTON_H
