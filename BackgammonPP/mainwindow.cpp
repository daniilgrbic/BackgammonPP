#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stringlistmodel.h"
#include <iostream>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->horizontalSlider->setValue(BASE_THEME_VOLUME);

    setFixedSize(width(), height());

    setPicture(this->backgroundPicPath, this);
    setPicture(this->sketchPicPath, ui->optionsFrame);

    connect(ui->btCreateGame, SIGNAL(clicked()), this, SLOT(on_btCreateGame_clicked()));
    connect(ui->btJoinGame, SIGNAL(clicked()), this, SLOT(on_btJoinGame_clicked()));
    connect(ui->btPreferences, SIGNAL(clicked()), this, SLOT(on_btPreference_clicked()));


    // Create Game Lobby
    connect(ui->btBackFromCreateToMenu, SIGNAL(clicked()), this, SLOT(on_btReturnToMenu_clicked()));
    connect(ui->btStartGame, SIGNAL(clicked(false)), this, SLOT(on_btStartGame_clicked()));

    // Join Game Lobby
    connect(ui->btBackFromJoinLobby, SIGNAL(clicked()), this, SLOT(on_btReturnToMenu_clicked()));
    connect(ui->btJoinLobby, SIGNAL(clicked(false)), this, SLOT(on_btJoinLobby_clicked()));

    // Preferences - labelPrefUsername, btSavePreferences
    connect(ui->btReturnFromPreferences, SIGNAL(clicked()), this, SLOT(on_btReturnToMenu_clicked()));
    connect(ui->btSavePreferences, SIGNAL(clicked()), this, SLOT(on_btSavePreference_clicked()));
    this->ui->lineEdit->setText(Preferences().playerName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPicture(QString picturePath, QWidget *pictureWidget) {
    QPixmap pix(picturePath);
    pix = pix.scaled(pictureWidget->size(), Qt::IgnoreAspectRatio);

    QPalette pallete;
    pallete.setBrush(QPalette::Window, pix);
    pictureWidget->setPalette(pallete);
    pictureWidget->setAutoFillBackground(true);
    pictureWidget->setFixedSize(pix.size());
}

void MainWindow::on_btCreateGame_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btPreference_clicked()
{
    emit requestPreferences(this->ui->horizontalSlider->value());
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_btReturnToMenu_clicked()
{
    this->ui->lineEdit->setText(this->ui->labelPrefUsername->text());
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btExit_clicked()
{
    this->close();
}

void MainWindow::on_btJoinGame_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_btJoinLobby_clicked()
{

    const QString &ipAddress = ui->inputIP->toPlainText();
    if (!this->isValidIpAddress(ipAddress)) {
        QMessageBox::information(nullptr, "Alert", "Enter valid IP address");
    }
    else {
        // CONNECT
    }
}

void MainWindow::on_btStartGame_clicked()
{
    GameType gameType = this->getGameType();
    PlayerType playerType = this->getPlayerType();
    QString opponentPlayer = this->ui->labelTextEdit->toPlainText();
    qint32 gameNumber = this->ui->sbGameDuration->value();

    if (gameNumber < MIN_NUM_GAMES or gameNumber > MAX_NUM_GAMES) {
        QMessageBox::information(nullptr, "Alert", "Enter valid game number: [" + QString::number(MIN_NUM_GAMES) + ", " + QString::number(MAX_NUM_GAMES) + "]");
        return;
    }

    if (playerType == PlayerType::BotPlayer) {
        emit requestCreateGame();
    }
    else {        
        if (opponentPlayer.size() < MIN_USERNAME_SIZE or opponentPlayer.size() > MAX_USERNAME_SIZE) {
            QMessageBox::information(nullptr, "Alert", "Enter Username between " + QString::number(MIN_USERNAME_SIZE) + " and " + QString::number(MAX_USERNAME_SIZE) + " characters");
            return;
        }

        // pass the arguments -> IGOR CALL FUNCTION HERE (create instance of your window in controller and emit signal for switching up here)
        QStringList opponents;
        model = new StringListModel(opponents);
        ui->lvOpponents->setModel(model);
        ui->lvOpponents->setSelectionMode(QAbstractItemView::SingleSelection);
        model->addOpponent("pera"); // Example how to add names to listview
        ui->stackedWidget->setCurrentIndex(4);
    }
}

void MainWindow::on_btSavePreference_clicked()
{
    if (this->ui->lineEdit->text().size() < MIN_USERNAME_SIZE or this->ui->lineEdit->text().size() > MAX_USERNAME_SIZE) {
        QMessageBox::information(nullptr, "Alert", "Enter Username between " + QString::number(MIN_USERNAME_SIZE) + " and " + QString::number(MAX_USERNAME_SIZE) + " characters");
        return;
    }
    qint16 newVolume = this->ui->horizontalSlider->value();
    emit requestPreferences(newVolume);
}

void MainWindow::handlePreferences(Preferences *preferences)
{
    preferences->playerName = this->ui->lineEdit->text();
    this->ui->labelPrefUsername->setText(preferences->playerName);
}

GameType MainWindow::getGameType()
{
    if (this->ui->rbModeClassic->isChecked())
        return GameType::ClassicGameType;
    else if (this->ui->rbModeNardy->isChecked())
        return GameType::LongNardyGameType;
    return GameType::ClassicGameType;
}

PlayerType MainWindow::getPlayerType()
{
    if (this->ui->rbPlayerBot->isChecked())
        return PlayerType::BotPlayer;
    else if (this->ui->rbPlayerLocal->isChecked())
        return PlayerType::LocalPlayer;
    else if (this->ui->rbPlayerRemote->isChecked())
        return PlayerType::RemotePlayer;
    return PlayerType::BotPlayer;
}



bool MainWindow::isValidIpAddress(const QString &ipAddress) {
    QHostAddress address;
    return address.setAddress(ipAddress);
}

void MainWindow::on_btCreateGameLobby_clicked()
{
    std::string selectedOpponent = ui->lvOpponents->currentIndex().data(Qt::DisplayRole).toString().toStdString();
    if(selectedOpponent == ""){
        QMessageBox::information(nullptr, "Alert", "You must select an opponent");
    }else{
        std::cout << selectedOpponent << std::endl;
        //emit request for game creation
    }
}


void MainWindow::on_btReturnFromCreateGameLobby_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

