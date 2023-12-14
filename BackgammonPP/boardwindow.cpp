#include "boardwindow.h"
#include "boardtriangle.h"
#include "ui_boardwindow.h"
#include "boardplayingdie.h"
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
}

void BoardWindow::on_pbExitGame_clicked()
{
    this->close();
}


void BoardWindow::on_pbRollDice_clicked()
{
    //get these informations from the game state;
    int value1 = 2;
    int value2 = 3;
    BoardPlayingDie::Position pos = BoardPlayingDie::Position::LEFT;
    m_boardScene->updatePlayingDice(value1, value2, pos);
}

