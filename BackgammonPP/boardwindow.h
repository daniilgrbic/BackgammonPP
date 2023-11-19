#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include "boardscene.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QVector>

namespace Ui {
class BoardWindow;
}

class BoardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BoardWindow(QWidget *parent = nullptr);
    ~BoardWindow();

private:
    Ui::BoardWindow *ui;
    BoardScene *m_boardScene;
};

#endif // BOARDWINDOW_H
