#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include "boardscene.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QVector>
#include <QPaintEvent>
#include "engine/core/game.h"
namespace Ui {
class BoardWindow;
}

class BoardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BoardWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    ~BoardWindow();
public slots:
    void enableEndTurn();
    void requestTurn(const std::vector<Turn> *legalTurns, const Roll *roll);
    void showRoll(const Roll& roll);
signals:
    void setBoardState(const BoardState &state);
    void forwardTurnFinish(Turn);
    void requestCloseGame();
private slots:
    void on_pbExitGame_clicked();

    void on_pbRollDice_clicked();

private:
    Ui::BoardWindow *ui;
    BoardScene *m_boardScene;
};

#endif // BOARDWINDOW_H
