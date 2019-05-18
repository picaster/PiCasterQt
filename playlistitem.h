#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QVariant>

enum PlaylistItemType { TRACK, CONTROL };

class PlaylistItem {
 public:
  virtual ~PlaylistItem();
  virtual PlaylistItemType getType() = 0;
};

class TrackItem : public PlaylistItem {
 public:
  TrackItem(const QString label, const QString path);
  PlaylistItemType getType() override { return TRACK; }
  const QString getLabel() { return label; }
  const QString getPath() { return path; }

 private:
  QString label;
  QString path;
};

class ControlItem : public PlaylistItem {
 public:
  ControlItem();
  PlaylistItemType getType() override { return CONTROL; }
};

#endif  // PLAYLISTITEM_H
