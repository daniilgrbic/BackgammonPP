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

signals:
    void requestCreateGame(QString opponentName, qint8 numGames, GameType gameType);
    void requestPreferences(qint16 newVolume);

public slots:
    void handlePreferences(Preferences *preferences);

private slots:
    void on_btReturnToMenu_clicked();
    void on_btPreference_clicked();

    void on_btCreateGame_clicked();
    void on_btStartGame_clicked();
    void on_rbPlayerRemote_clicked();

    void on_btJoinGame_clicked();
    void on_btJoinLobby_clicked();

    void on_btSavePreference_clicked();

    void on_btExit_clicked();

    void on_btCreateGameLobby_clicked();

    void on_btReturnFromCreateGameLobby_clicked();

private:
    Ui::MainWindow *ui;
    BoardWindow *boardWindow;
    QString backgroundPicPath = ":/resources/img/blurred_backgammon_blacked.png";
    QString sketchPicPath = ":/resources/img/menu_sketch.png";

    void resizeEvent(QResizeEvent*);

    void setPicture(QString picturePath, QWidget *pictureWidget);
    bool isValidIpAddress(const QString &ipAddress);
    void displayPreferences();
    void showIpAddress();
    GameType getGameType();
    PlayerType getPlayerType();
    StringListModel *model;
};
