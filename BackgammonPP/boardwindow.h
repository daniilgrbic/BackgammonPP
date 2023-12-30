#pragma once

#include "boardscene.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QVector>
#include <QPaintEvent>
#include "historylistmodel.h"
namespace Ui {
class BoardWindow;
}

class BoardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BoardWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void setExitPoints(GameType);
    void setOpponentName(QString opponentName);
    void setScore(int white, int black);
    void scrollLogToBottom();
    ~BoardWindow();
public slots:
    void enableEndTurn();
    void setUndoEnabled(bool enabled);
    void requestTurn(const std::vector<Turn> *legalTurns, const Roll *roll);
    void showRoll(const Roll& roll);
signals:
    void setBoardState(const BoardState &state);
    void diceRolled(Roll);
    void forwardTurnFinish(Turn);
    void requestCloseGame();

private slots:
    void on_pbRollDice_clicked();

    void on_pbEndTurn_clicked();

    void on_pbUndo_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_pbExitCurrentGame_clicked();

private:
    Ui::BoardWindow *ui;
    BoardScene *m_boardScene;
    BoardScene *m_historyboardScene;


    QString m_opponentName;
public:
    HistoryListModel *m_historyModel;
};
