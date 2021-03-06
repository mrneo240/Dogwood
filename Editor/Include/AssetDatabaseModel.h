#pragma once

#include <QAbstractTableModel>
#include <vector>

class ResourceInfo;

using std::vector;

class AssetDatabaseModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    AssetDatabaseModel(QObject *parent);

    int             rowCount(const QModelIndex &parent = QModelIndex()) const;
    int             columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant        data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    ResourceInfo*   getItem(const QModelIndex &index) const;

    Qt::ItemFlags   flags(const QModelIndex& index) const;
    Qt::DropActions supportedDragActions() const;
    Qt::DropActions supportedDropActions() const;
    QStringList     mimeTypes() const;
    QMimeData*      mimeData(const QModelIndexList &indexes) const;

    static const int FilterRole = Qt::UserRole + 2;

private:
    vector<ResourceInfo*>   m_infos;
};