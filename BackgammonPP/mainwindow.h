#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "boardwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btHost_clicked();

    void on_pbBackHosting_clicked();

    void on_btPrefs_clicked();

    void on_btJoin_clicked();

    void on_pbBackJoin_clicked();

    void on_pbExit_clicked();

    void on_pushButton_clicked();

    void on_pbStartGame_clicked();

private:
    Ui::MainWindow *ui;
    BoardWindow *boardWindow;
};
#endif // MAINWINDOW_H
