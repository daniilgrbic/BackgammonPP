#include "boardwindow.h"
#include "boardtriangle.h"
#include "ui_boardwindow.h"
#include "boardplayingdie.h"
#include <QPainter>
#include "engine/core/boardstate.h"
#include "engine/backgammon.h"

BoardWindow::BoardWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoardWindow)
{
    ui->setupUi(this);
    m_boardScene = new BoardScene(this, ui->boardView->size().width()-5, ui->boardView->size().height()-5);
    ui->boardView->setScene(m_boardScene);
    ui->boardView->setRenderHint(QPainter::Antialiasing);
    //ui->boardView->resize(605,305);
    ui->pbEndTurn->setEnabled(false);
    ui->pbRollDice->setEnabled(false);
    connect(m_boardScene, &BoardScene::enableEndTurn, this, &BoardWindow::enableEndTurn);
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
    emit requestCloseGame();
}


void BoardWindow::on_pbRollDice_clicked()
{
    //get these informations from the game state;
    int value1 = 2;
    int value2 = 3;
    BoardPlayingDie::Position pos = BoardPlayingDie::Position::RIGHT;
    m_boardScene->updatePlayingDice(value1, value2, pos);
}

void BoardWindow::requestTurn(const std::vector<Turn> *legalTurns, const Roll *roll){

    m_boardScene->setLegalTurns(legalTurns);
    m_boardScene->setRoll(roll);
    ui->pbRollDice->setEnabled(true);
    return;
}

void BoardWindow::enableEndTurn(){
    ui->pbEndTurn->setEnabled(true);
}
