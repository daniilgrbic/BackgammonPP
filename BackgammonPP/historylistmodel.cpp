#include "historylistmodel.h"
#include <qnamespace.h>

auto HistoryListModel::rowCount(const QModelIndex &parent) const -> int
{
    return turnList.count();
}

auto HistoryListModel::data(const QModelIndex &index, int role) const -> QVariant
{
    if (!index.isValid())
        return {};

    if (index.row() >= turnList.size())
        return {};

    if (role == Qt::DisplayRole) {
        auto item = turnList.at(index.row());
        if(std::holds_alternative<Turn>(item))
            return std::get<Turn>(item).toString();
        else
            return std::get<QString>(item);
    }

    if (role == Qt::UserRole) {
        auto item = turnList.at(index.row());
        if(std::holds_alternative<Turn>(item))
            return std::get<Turn>(item).toVariant();
    }

    if (role == Qt::UserRole + 1)
        return turnList.size() == index.row() + 1;

    if (role == Qt::UserRole + 2)
        return std::holds_alternative<Turn>(turnList.at(index.row()));

    return {};
}

auto HistoryListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const -> QVariant
{
    if (role != Qt::DisplayRole)
        return {};

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

void HistoryListModel::addString(const QString &string)
{
    beginInsertRows(QModelIndex(),turnList.size(), turnList.size() + 1);
    turnList.append(string);
    endInsertRows();
}
