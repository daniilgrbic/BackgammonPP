#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include "boardscene.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QVector>
#include <QPaintEvent>

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

signals:
    void requestCloseGame();

private slots:
    void on_pbExitGame_clicked();

    void on_pbRollDice_clicked();

private:
    Ui::BoardWindow *ui;
    BoardScene *m_boardScene;
};

#endif // BOARDWINDOW_H
