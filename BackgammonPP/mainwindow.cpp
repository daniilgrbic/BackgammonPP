#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->horizontalSlider->setValue(BASE_THEME_VOLUME);
    setWindowTitle("Backgammon++");
    setWindowIcon(QIcon(ICON_PATH));

    setFixedSize(width(), height());

    setPicture(this->backgroundPicPath, this);
    setPicture(this->sketchPicPath, ui->optionsFrameContent);


    // Create Game Lobby
    this->on_rbPlayerRemote_clicked();
    this->showIpAddress();

    // Preferences - labelPrefUsername, btSavePreferences
    ui->lineEdit->setText(Preferences().playerName);
    ui->labelPrefUsername->setText(Preferences().playerName);
    menuVolume = ui->horizontalSlider->value();
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

void MainWindow::on_btPreferences_clicked()
{
    emit requestPreferences(this->ui->horizontalSlider->value());
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::returnToMenu()
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
    const QString &ipAddress = ui->inputIP->text();

    QStringList ipParts = ipAddress.split(".");

    if (ipParts.size() != 4 || !this->isValidIpAddress(ipAddress)) {
        QMessageBox::information(nullptr, "Alert", "Enter valid IP address");
    }
    else {
        emit joinRemoteMatch(ipAddress);
    }
}

void MainWindow::on_btStartGame_clicked()
{
    GameType gameType = this->getGameType();
    PlayerType playerType = this->getPlayerType();
    QString opponentName = this->ui->teOpponentUsername->text();
    qint32 gameNumber = this->ui->sbGameDuration->value();

    if (gameNumber < MIN_NUM_GAMES or gameNumber > MAX_NUM_GAMES) {
        QMessageBox::information(nullptr, "Alert", "Enter valid game number: [" + QString::number(MIN_NUM_GAMES) + ", " + QString::number(MAX_NUM_GAMES) + "]");
        return;
    }

    if (playerType == PlayerType::BotPlayer) {
        emit requestCreateGame("Bot", gameNumber, gameType, playerType);
    }
    else if (playerType == PlayerType::LocalPlayer) {
        emit requestCreateGame("LocalPlayer", gameNumber, gameType, playerType);
    }
    else {
        if (opponentName.size() < MIN_USERNAME_SIZE or opponentName.size() > MAX_USERNAME_SIZE) {
            QMessageBox::information(nullptr, "Alert", "Enter Username between " + QString::number(MIN_USERNAME_SIZE) + " and " + QString::number(MAX_USERNAME_SIZE) + " characters");
            return;
        }
        else {
            emit requestCreateGame(opponentName, gameNumber, gameType, playerType);
        }
    }
}

void MainWindow::on_btSavePreferences_clicked()
{
    if (this->ui->lineEdit->text().size() < MIN_USERNAME_SIZE or this->ui->lineEdit->text().size() > MAX_USERNAME_SIZE) {
        QMessageBox::information(nullptr, "Alert", "Enter Username between " + QString::number(MIN_USERNAME_SIZE) + " and " + QString::number(MAX_USERNAME_SIZE) + " characters");
        return;
    }
    menuVolume = this->ui->horizontalSlider->value();
    emit requestPreferences(menuVolume);
}

void MainWindow::handlePreferences(Preferences *preferences)
{
    preferences->playerName = this->ui->lineEdit->text();
    ui->labelPrefUsername->setText(preferences->playerName);
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

void MainWindow::on_rbPlayerRemote_clicked() {
    if (ui->rbPlayerRemote->isChecked()) {
        ui->teOpponentUsername->setDisabled(false);
        ui->teOpponentUsername->setStyleSheet("background-color: #EDE9E8; color: black");
    }
    else {
        ui->teOpponentUsername->setDisabled(true);
        ui->teOpponentUsername->setStyleSheet("background-color: gray; color: black");
        ui->teOpponentUsername->setText("");
    }
}

void MainWindow::on_rbPlayerLocal_clicked() {
    ui->teOpponentUsername->setDisabled(true);
    ui->teOpponentUsername->setStyleSheet("background-color: gray; color: black");
    ui->teOpponentUsername->setText("");
}

void MainWindow::on_rbPlayerBot_clicked() {
    ui->teOpponentUsername->setDisabled(true);
    ui->teOpponentUsername->setStyleSheet("background-color: gray; color: black");
    ui->teOpponentUsername->setText("");
}

QString MainWindow::getIpAddress() {
    return m_ipAddress;
}

void MainWindow::showIpAddress() {
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    QString ipAddress = "Can't find";
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            ipAddress = address.toString();
        }
    }
    m_ipAddress = ipAddress;
    ui->labeIP->setText(ipAddress);
}

void MainWindow::on_btBackFromCreateToMenu_clicked() {
    returnToMenu();
}

void MainWindow::on_btBackFromJoinLobby_clicked() {
    returnToMenu();
}

void MainWindow::on_btReturnFromPreferences_clicked() {
    ui->horizontalSlider->setValue(menuVolume);
    returnToMenu();
}
