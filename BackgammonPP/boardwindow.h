#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include "boardscene.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QVector>
#include <QPaintEvent>
#include "engine/core/game.h"
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
    ~BoardWindow();
public slots:
    void enableEndTurn();
    void setUndoEnabled(bool enabled);
    void requestTurn(const std::vector<Turn> *legalTurns, const Roll *roll);
    void showRoll(const Roll& roll);
signals:
    void setBoardState(const BoardState &state);
    void forwardTurnFinish(Turn);
    void requestCloseGame();
private slots:
    void on_pbExitGame_clicked();

    void on_pbRollDice_clicked();

    void on_pbEndTurn_clicked();

    void on_pbUndo_clicked();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::BoardWindow *ui;
    BoardScene *m_boardScene;
    BoardScene *m_historyboardScene;
    HistoryListModel *m_historyModel;
};

#endif // BOARDWINDOW_H
