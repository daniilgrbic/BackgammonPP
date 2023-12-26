#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "engine/bot/progresssignaler.hpp"
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void on_pushButton_clicked();
    void globalProgress(int value);
    void localRange(int lower, int upper);
    void localProgress();
    void doneTraining();
    void stdoutToTextBox(QString text);

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    ProgressSignaler *ps;
};
#endif // MAINWINDOW_H
