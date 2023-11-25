#include "boardwindow.h"
#include "boardtriangle.h"
#include "ui_boardwindow.h"
#include <QPainter>

BoardWindow::BoardWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoardWindow)
{
    ui->setupUi(this);
    m_boardScene = new BoardScene(this, ui->boardView->size().width()-5, ui->boardView->size().height()-5);
    ui->boardView->setScene(m_boardScene);
    ui->boardView->setRenderHint(QPainter::Antialiasing);
    //ui->boardView->resize(605,305);
}

BoardWindow::~BoardWindow()
{
    delete ui;
}

void BoardWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    // m_boardScene->setPainter(*painter);
}
