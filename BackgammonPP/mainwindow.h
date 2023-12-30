#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QCoreApplication>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QPalette>
#include <QMessageBox>
#include <QHostAddress>
#include <QNetworkInterface>

#include "boardwindow.h"
#include "consts.h"
#include "stringlistmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getIpAddress();

signals:
    void requestCreateGame(QString opponentName, qint8 numGames, GameType gameType, PlayerType playerType);
    void joinRemoteMatch(QString ipAddress);
    void requestPreferences(qint16 newVolume);

public slots:
    void handlePreferences(Preferences *preferences);

private slots:
    void returnToMenu();
    void on_btPreferences_clicked();

    void on_btCreateGame_clicked();
    void on_btStartGame_clicked();
    void on_rbPlayerBot_clicked();
    void on_rbPlayerLocal_clicked();
    void on_rbPlayerRemote_clicked();
    void on_btBackFromCreateToMenu_clicked();

    void on_btJoinGame_clicked();
    void on_btJoinLobby_clicked();
    void on_btBackFromJoinLobby_clicked();

    void on_btSavePreferences_clicked();
    void on_btReturnFromPreferences_clicked();

    void on_btExit_clicked();

private:
    Ui::MainWindow *ui;
    BoardWindow *boardWindow;
    QString backgroundPicPath = ":/resources/img/blurred_backgammon_blacked.png";
    QString sketchPicPath = ":/resources/img/menu_sketch.png";
    QString m_ipAddress;

    void setPicture(QString picturePath, QWidget *pictureWidget);
    bool isValidIpAddress(const QString &ipAddress);
    void displayPreferences();
    void showIpAddress();
    GameType getGameType();
    PlayerType getPlayerType();
    StringListModel *model;
    qint16 menuVolume;
};
