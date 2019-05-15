#include "playlistitemmodel.h"

PlaylistItemModel::PlaylistItemModel(QObject *parent) : QAbstractListModel(parent)
{
}

int
PlaylistItemModel::rowCount(const QModelIndex &/*parent*/) const
{
    return this->lst.count();
}

QVariant
PlaylistItemModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= lst.size())
        return QVariant();
    if (role == Qt::DisplayRole)
    {
        PlaylistItem* item = lst.at(index.row());
        PlaylistItemType itemType = item->getType();
        if (itemType == TRACK)
        {
            return static_cast<TrackItem*>(item)->getPath();
        }
        else
        {
            return QString(">>> stop player <<<");
        }
    }
    return QVariant();
}

bool
PlaylistItemModel::addItem(PlaylistItem* const playlistItem)
{
    int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    lst.append(playlistItem);
    endInsertRows();
    QModelIndex index = this->index(row);
    emit dataChanged(index, index, {Qt::DisplayRole});
    return true;
}

bool
PlaylistItemModel::removeItem(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    lst.removeAt(row);
    endRemoveRows();
    QModelIndex index = this->index(row);
    emit dataChanged(index, index, {Qt::DisplayRole});
    return true;
}
