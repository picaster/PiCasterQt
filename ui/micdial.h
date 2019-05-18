#ifndef MICDIAL_H
#define MICDIAL_H

#include <QDial>

class MicDial : public QDial {
  Q_OBJECT
 public:
  MicDial(QWidget* parent = nullptr);
};

#endif  // MICDIAL_H
