#include "controller.h"
#include "match.h"
#include "localplayer.h"
#include "botplayer.h"
#include "engine/bot/bot.hpp"

Controller::Controller()
{
    this->preferences = new Preferences();
    this->mainWindow = new MainWindow();
    this->boardWindow = new BoardWindow();

    mainWindow->show();

    this->playThemeSong();

    connect(mainWindow, &MainWindow::requestCreateGame, this, &Controller::createGameFromMenu);
    connect(boardWindow, &BoardWindow::requestCloseGame, this, &Controller::closeGameAndOpenMenu);

    connect(mainWindow, &MainWindow::joinRemoteMatch, this, &Controller::joinRemoteMatchFromMenu);

    connect(mainWindow, &MainWindow::requestPreferences, this, &Controller::getPreferences);
    connect(this, &Controller::sendPreferences, mainWindow, &MainWindow::handlePreferences);
}

void Controller::playThemeSong()
{
    this->themeSong = new QMediaPlayer(this);
    this->themeAudioOutput = new QAudioOutput(this);

    this->themeAudioOutput->setVolume(BASE_THEME_VOLUME);
    this->themeSong->setAudioOutput(this->themeAudioOutput);
    this->themeSong->setSource(this->themeSongPath);
    this->themeSong->setLoops(-1);

    this->themeSong->play();
}

void Controller::getPreferences(qint16 newVolume)
{
    this->themeAudioOutput->setVolume((float)newVolume / (float)MAX_VOLUME);
    this->themeSong->play();
    emit sendPreferences(this->preferences);
}

void Controller::createGameFromMenu(QString opponentName, qint8 numGames, GameType gameType, PlayerType playerType)
{

    if (playerType == PlayerType::BotPlayer) {
        mainWindow->close();

        boardWindow->setOpponentName(opponentName);
        boardWindow->show();
        Player *white = new LocalPlayer(nullptr, this->boardWindow);
        AI::Bot *bot = new AI::Bot("../BackgammonPP/engine/bot/saved_genomes/gen_603.genome");
        Player *black= new BotPlayer(nullptr, this->boardWindow, bot);//LocalPlayer(nullptr, this->boardWindow);
        Match *m = new Match(nullptr, white, black, numGames, gameType);
        connect(white, &LocalPlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
        connect(black, &BotPlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
        white->setParent(m);
        black->setParent(m);
        m->startGame();
    }
    else if (playerType == PlayerType::LocalPlayer) {
        mainWindow->close();

        boardWindow->setOpponentName(opponentName);
        boardWindow->show();
        Player *white = new LocalPlayer(nullptr, this->boardWindow);
        Player *black= new LocalPlayer(nullptr, this->boardWindow);
        Match *m = new Match(nullptr, white, black, numGames, gameType);
        connect(white, &LocalPlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
        connect(black, &LocalPlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
        white->setParent(m);
        black->setParent(m);
        m->startGame();
    }
    else {
        thread_server = new QThread();
        server_local = new Server(opponentName);
        server_local->moveToThread(thread_server);
        thread_server->start();
    }

}

void Controller::joinRemoteMatchFromMenu(QString ipAddress)
{
    client_local = new Client(ipAddress, preferences->playerName);
}

void Controller::closeGameAndOpenMenu()
{
    boardWindow->close();
    mainWindow->show();
}
