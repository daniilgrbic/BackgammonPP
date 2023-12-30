#include "controller.h"
#include "match.h"
#include "localplayer.h"
#include "botplayer.h"
#include "remoteplayer.h"
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
        Player *black = new BotPlayer(nullptr, this->boardWindow, bot);
        match_current = new Match(nullptr, white, black, boardWindow, numGames, gameType);
        connect(white, &LocalPlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
        connect(black, &BotPlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
        white->setParent(match_current);
        black->setParent(match_current);
        match_current->startGame();
    }
    else if (playerType == PlayerType::LocalPlayer) {
        mainWindow->close();

        boardWindow->setOpponentName(opponentName);
        boardWindow->show();
        Player *white = new LocalPlayer(nullptr, this->boardWindow);
        Player *black = new LocalPlayer(nullptr, this->boardWindow);
        match_current = new Match(nullptr, white, black, boardWindow, numGames, gameType);
        connect(white, &LocalPlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
        connect(black, &LocalPlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
        white->setParent(match_current);
        black->setParent(match_current);
        match_current->startGame();
    }
    else {
        QString ipAddress = mainWindow->getIpAddress();
        mainWindow->close();
        thread_server = new QThread();
        server_local = new Server(opponentName);
        server_local->moveToThread(thread_server);
        thread_server->start();

        boardWindow->setOpponentName(opponentName);
        boardWindow->show();
        Player *white = new LocalPlayer(nullptr, this->boardWindow);
        Player *black = new RemotePlayer(nullptr, ipAddress, preferences->playerName);

        match_current = new Match(nullptr, white, black, boardWindow, numGames, gameType);
        connect(white, &LocalPlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
        connect(black, &RemotePlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
        connect(dynamic_cast<RemotePlayer*>(black)->getClient(), &Client::startGame, match_current, &Match::startGameRequest);
        white->setParent(match_current);
        black->setParent(match_current);

        player_remote = black;
        connect(dynamic_cast<RemotePlayer*>(black), &RemotePlayer::terminateGame, this, &Controller::closeGameAndOpenMenu);
    }

}

void Controller::joinRemoteMatchFromMenu(QString ipAddress)
{
    mainWindow->close();
    boardWindow->show();

    Player *black = new LocalPlayer(nullptr, this->boardWindow);
    Player *white = new RemotePlayer(nullptr, ipAddress, preferences->playerName);

    match_current = new Match(nullptr, white, black, boardWindow, 3, GameType::ClassicGameType, false); //un-hardcode this
    connect(white, &RemotePlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
    connect(black, &LocalPlayer::returnMove, boardWindow->m_historyModel, &HistoryListModel::addTurn);
    white->setParent(match_current);
    black->setParent(match_current);
    match_current->startGame();

    player_remote = white;
    connect(dynamic_cast<RemotePlayer*>(white), &RemotePlayer::terminateGame, this, &Controller::closeGameAndOpenMenu);
}

void Controller::closeGameAndOpenMenu()
{
    qDebug() << "closeGameAndOpenMenu\n";

    if (player_remote != nullptr) {
        disconnect(dynamic_cast<RemotePlayer*>(player_remote), &RemotePlayer::terminateGame, this, &Controller::closeGameAndOpenMenu);
        // delete player_remote;
    }

    qDebug() << "Check1\n";

    if (thread_server != nullptr) {
        // thread_server->deleteLater(); -- this line is the problem
        server_local->deleteLater();

        thread_server = nullptr;
        server_local = nullptr;
    }

    qDebug() << "Check2\n";

    boardWindow->close();

    delete boardWindow;
    boardWindow = new BoardWindow();
    connect(boardWindow, &BoardWindow::requestCloseGame, this, &Controller::closeGameAndOpenMenu);

    qDebug() << "Check3\n";

    if (match_current != nullptr)
        delete match_current;

    qDebug() << "Check4\n";

    match_current = nullptr;
    player_remote = nullptr;

    mainWindow->show();

    qDebug() << "CheckFinal\n";
}
