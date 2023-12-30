#ifndef HISTORYLISTMODEL_H
#define HISTORYLISTMODEL_H

#include "engine/core/turn.h"
#include <QAbstractListModel>
#include <QObject>
#include <QList>

class HistoryListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    HistoryListModel(const QVector<Turn> &turns, QObject *parent = nullptr)
        : QAbstractListModel(parent), turnList(turns.cbegin(), turns.cend()) {}

    HistoryListModel(QObject *parent = nullptr)
        : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
public slots:
    void addTurn(const Turn& turn);
    void addString(const QString& string);

private:
    QList<std::variant<Turn, QString>> turnList;
};

#endif // HISTORYLISTMODEL_H
