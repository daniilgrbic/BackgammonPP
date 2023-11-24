#include "boardwindow.h"
#include "boardtriangle.h"
#include "ui_boardwindow.h"

BoardWindow::BoardWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoardWindow),
    m_boardScene(new BoardScene(this))
{
    ui->setupUi(this);
    ui->boardView->setScene(m_boardScene);
    ui->boardView->resize(605,305);
}

BoardWindow::~BoardWindow()
{
    delete ui;
}

void BoardWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    // m_boardScene->setPainter(*painter);
}
