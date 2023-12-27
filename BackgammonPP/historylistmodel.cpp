#include "historylistmodel.h"
#include <qnamespace.h>

int HistoryListModel::rowCount(const QModelIndex &parent) const
{
    return turnList.count();
}

QVariant HistoryListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= turnList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return QString("%1").arg(turnList.at(index.row()).toString());

    if(role == Qt::UserRole){
        return turnList.at(index.row()).toVariant();
    }
    if (role == Qt::UserRole + 1)
        return turnList.size() == index.row() + 1;


    return QVariant();
}

QVariant HistoryListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QStringLiteral("Column %1").arg(section);
    else
        return QStringLiteral("Row %1").arg(section);
}
void HistoryListModel::addTurn(const Turn& turn){
    beginInsertRows(QModelIndex(),turnList.size(), turnList.size() + 1);
    turnList.append(turn);
    endInsertRows();
}

