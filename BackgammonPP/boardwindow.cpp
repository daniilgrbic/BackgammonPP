#include "boardwindow.h"
#include "boardtriangle.h"
#include "ui_boardwindow.h"
#include "boardplayingdie.h"
#include <QPainter>
#include "engine/core/boardstate.h"
#include "engine/backgammon.h"
#include "historylistmodel.h"
#include <iostream>

BoardWindow::BoardWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoardWindow)
{
    ui->setupUi(this);
    m_historyModel = new HistoryListModel();
    ui->listView->setModel(m_historyModel);
    ui->historyboardView->hide();
    m_boardScene = new BoardScene(this, ui->boardView->size().width()-5, ui->boardView->size().height()-5);
    m_historyboardScene = new BoardScene(this, ui->historyboardView->size().width()-5, ui->historyboardView->size().height()-5);
    ui->boardView->setScene(m_boardScene);
    ui->boardView->setRenderHint(QPainter::Antialiasing);
    setWindowTitle("Backgammon++");
    setWindowIcon(QIcon(ICON_PATH));
    setFixedSize(width(), height());

    //ui->boardView->resize(605,305);
    ui->pbEndTurn->setEnabled(false);
    ui->pbRollDice->setEnabled(false);
    ui->pbUndo->setEnabled(false);
    connect(m_boardScene, &BoardScene::enableEndTurn, this, &BoardWindow::enableEndTurn);
    //requestTUrn should be connected to from the LocalPlayer class
    connect(m_boardScene, &BoardScene::sendTurnFinish, this, &BoardWindow::forwardTurnFinish);
    connect(this, &BoardWindow::setBoardState, m_boardScene, &BoardScene::setBoardState);
    connect(m_boardScene, &BoardScene::setUndoEnabled, this, &BoardWindow::setUndoEnabled);
    // connect(this, &BoardWindow::forwardTurnFinish, this->m_historyModel, &HistoryListModel::addTurn);

    // connect(ui->pbExitCurrentGame, SIGNAL(clicked()), this, SLOT(on_pbExitGame_clicked()));
}

BoardWindow::~BoardWindow()
{
    delete ui;
}

void BoardWindow::setOpponentName(QString opponentName) {
    this->m_opponentName = opponentName;
}

void BoardWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
}

void BoardWindow::setExitPoints(GameType type){
    m_boardScene->setExitPoints(type);
}

void BoardWindow::on_pbRollDice_clicked()
{
    //get these informations from the game state;

    m_boardScene->updatePlayingDice();
    ui->pbRollDice->setEnabled(false);
    m_boardScene->getTurnInit();
    emit diceRolled(*(m_boardScene->getRoll()));
}

void BoardWindow::showRoll(const Roll& roll){
    m_boardScene->updatePlayingDice(roll);
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

void BoardWindow::on_pbEndTurn_clicked()
{
    ui->pbEndTurn->setEnabled(false);
    m_boardScene->getTurnFinish();
}


void BoardWindow::on_pbUndo_clicked()
{
    ui->pbUndo->setEnabled(false);
    ui->pbEndTurn->setEnabled(false);
    m_boardScene->undoMove();

}


void BoardWindow::setUndoEnabled(bool enabled){
    ui->pbUndo->setEnabled(enabled);
}

void BoardWindow::on_listView_clicked(const QModelIndex &index)
{
   // std::cout << index.data().toString().toStdString() << std::endl;

    bool isCurrentBoardState = index.data(Qt::UserRole + 1).toBool();
    if(isCurrentBoardState){
        ui->pbEndTurn->show();
        ui->pbRollDice->show();
        ui->pbUndo->show();
        ui->boardView->show();
        ui->historyboardView->hide();
        return;
    }

    bool isTurn = index.data(Qt::UserRole + 2).toBool();
    if(!isTurn)
        return;

    ui->pbEndTurn->hide();
    ui->pbRollDice->hide();
    ui->pbUndo->hide();
    ui->boardView->hide();
    Turn turn;
    turn.fromVariant(index.data(Qt::UserRole));
    m_historyboardScene->setBoardState(turn.m_finalBoard);

    ui->historyboardView->setScene(m_historyboardScene);
    ui->historyboardView->setRenderHint(QPainter::Antialiasing);
    ui->historyboardView->show();
}

void BoardWindow::on_pbExitCurrentGame_clicked()
{
    emit requestCloseGame();
}

void BoardWindow::setScore(int white, int black){
    ui->lbScoreWhite->setText(QString::number(white));
    ui->lbScoreBlack->setText(QString::number(black));
}

void BoardWindow::scrollLogToBottom()
{
    ui->listView->scrollToBottom();
}
