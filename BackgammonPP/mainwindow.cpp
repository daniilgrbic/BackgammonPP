#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    setFixedSize(width(), height());

    setPicture(this->backgroundPicPath, this);
    setPicture(this->sketchPicPath, ui->optionsFrame);

    connect(ui->btCreateGame, SIGNAL(clicked()), this, SLOT(on_btCreateGame_clicked()));
    connect(ui->btJoinGame, SIGNAL(clicked()), this, SLOT(on_btJoinGame_clicked()));
    connect(ui->btPreferences, SIGNAL(clicked()), this, SLOT(on_btPreference_clicked()));


    // Create Game Lobby
    connect(ui->btBackFromCreateToMenu, SIGNAL(clicked()), this, SLOT(on_btReturnToMenu_clicked()));
    connect(ui->btStartGame, SIGNAL(clicked()), this, SLOT(on_btStartGame_clicked()));
    // btStartGame

    // Join Game Lobby
    connect(ui->btBackFromJoinLobby, SIGNAL(clicked()), this, SLOT(on_btReturnToMenu_clicked()));
    connect(ui->btJoinLobby, SIGNAL(clicked()), this, SLOT(on_btJoinLobby_clicked()));

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
    emit requestPreferences();
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_btReturnToMenu_clicked()
{
    ui->messageFromServer->setText("");
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
    const QString &userName = ui->inputName->toPlainText();
    if (!this->isValidIpAddress(ipAddress)) {
        QMessageBox::information(nullptr, "Alert", "Enter valid IP address");
    }
    else if (userName.size() < MIN_USERNAME_SIZE or userName.size() > MAX_USERNAME_SIZE) {
        QMessageBox::information(nullptr, "Alert", "Enter Username between " + QString::number(MIN_USERNAME_SIZE) + " and " + QString::number(MAX_USERNAME_SIZE) + " characters");
    }
    else {
        ui->messageFromServer->setText(
            QString("PLEASE WAIT...\n") +
            QString("Finding hosted game...\n") +
            QString("IP Address: ") + ipAddress + QString("\n") +
            QString("Host Player Username: ") + userName + QString("\n")
        );
        // qDebug() << " " << ipAddress << " | " << userName << "\n";
    }
}

void MainWindow::on_btStartGame_clicked()
{
    GameType gameType = this->getGameType();
    PlayerType playerType = this->getPlayerType();
    QString opponentPlayer = this->ui->labelTextEdit->toPlainText();
    qint32 moveNumber = this->ui->sbGameDuration->value();

    if (playerType == BotPlayer) {
        emit requestCreateGame();
    }
    else {
        // pass the arguments -> IGOR CALL FUNCTION HERE (create instance of your window in controller and emit signal for switching up here)
    }
}

void MainWindow::on_btSavePreference_clicked()
{
    if (this->ui->lineEdit->text().size() < MIN_USERNAME_SIZE or this->ui->lineEdit->text().size() > MAX_USERNAME_SIZE) {
        QMessageBox::information(nullptr, "Alert", "Enter Username between " + QString::number(MIN_USERNAME_SIZE) + " and " + QString::number(MAX_USERNAME_SIZE) + " characters");
        return;
    }
    emit requestPreferences();
}

void MainWindow::handlePreferences(Preferences *preferences)
{
    preferences->playerName = this->ui->lineEdit->text();
    this->ui->labelPrefUsername->setText(preferences->playerName);
}

GameType MainWindow::getGameType()
{
    if (this->ui->rbModeClassic->isChecked())
        return ClassicGameType;
    else if (this->ui->rbModeNardy->isChecked())
        return LongNardyGameType;
    return ClassicGameType;
}

PlayerType MainWindow::getPlayerType()
{
    if (this->ui->rbPlayerBot->isChecked())
        return BotPlayer;
    else if (this->ui->rbPlayerLocal->isChecked())
        return LocalPlayer;
    else if (this->ui->rbPlayerRemote->isChecked())
        return RemotePlayer;
    return BotPlayer;
}



bool MainWindow::isValidIpAddress(const QString &ipAddress) {
    QHostAddress address;
    return address.setAddress(ipAddress);
}
