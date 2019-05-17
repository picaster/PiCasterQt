#include "mainwindow.h"
#include <QApplication>

#include "jack/jackclient.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavresample/avresample.h>
}

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  JackClient::getInstance();
  av_register_all();
  MainWindow w;
  w.show();

  return a.exec();
}
