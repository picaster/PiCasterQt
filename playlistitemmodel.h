#ifndef PLAYLISTITEMMODEL_H
#define PLAYLISTITEMMODEL_H

#include <QAbstractListModel>

#include "playlistitem.h"

class PlaylistItemModel : public QAbstractListModel
{
  Q_OBJECT
public:
  explicit PlaylistItemModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  bool addItem(PlaylistItem* const playlistItem);
  bool removeItem(int row);
  const QList<PlaylistItem*> list() { return lst; }

private:
  QList<PlaylistItem*> lst;
};

#endif // PLAYLISTITEMMODEL_H
