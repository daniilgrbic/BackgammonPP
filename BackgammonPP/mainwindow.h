#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QCoreApplication>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QPalette>
#include <iostream>
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
    void on_btCreateGame_clicked();

    void on_btExit_clicked();

private:
    Ui::MainWindow *ui;
    BoardWindow *boardWindow;
    QString backgroundPicPath = ":/resources/img/blurred_backgammon_blacked.png";

};
#endif // MAINWINDOW_H
