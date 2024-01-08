#include "stringlistmodel.h"

auto StringListModel::rowCount(const QModelIndex &parent) const -> int
{
    return stringList.count();
}

auto StringListModel::data(const QModelIndex &index, int role) const -> QVariant
{
    if (!index.isValid())
        return {};

    if (index.row() >= stringList.size())
        return {};

    if (role == Qt::DisplayRole)
        return stringList.at(index.row());
    else
        return {};
}

auto StringListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const -> QVariant
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal)
        return QStringLiteral("Column %1").arg(section);
    else
        return QStringLiteral("Row %1").arg(section);
}
void StringListModel::addOpponent(const QString& name){
    stringList.append(name);
}

void StringListModel::removeOpponent(const QString& name){
    stringList.removeOne(name);
}
