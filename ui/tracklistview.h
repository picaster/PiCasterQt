#ifndef TRACKLISTVIEW_H
#define TRACKLISTVIEW_H

#include <QListView>

class TrackListView : public QListView {
  Q_OBJECT
 public:
  TrackListView(QWidget* parent = nullptr);
};

#endif  // TRACKLISTVIEW_H
