#include "playlistitem.h"

PlaylistItem::~PlaylistItem() {}

TrackItem::TrackItem(const QString label, const QString path) {
  this->label = label;
  this->path = path;
}

ControlItem::ControlItem() {}
