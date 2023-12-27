#include "historylistmodel.h"
#include <iostream>
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

    if (role == Qt::DisplayRole)        // Ovde treba nesto smisleno? index.row() + turnList.at(index.row()).toString()...
        return QString("%1 :Turn %2").arg(turnList.at(index.row()).m_player == PlayerColor::BLACK ? "black" : "white").arg(index.row() + 1);

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

