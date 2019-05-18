#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QListView>
#include <QObject>

class MediaPlayer : public QObject {
  Q_OBJECT
 public:
  MediaPlayer(QListView* listView);

 private:
  QListView* listView;
  void startPlaylist();
};

#endif  // MEDIAPLAYER_H
