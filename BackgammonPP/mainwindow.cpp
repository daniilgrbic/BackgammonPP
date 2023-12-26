#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "engine/bot/neat.hpp"
#include <QThread>
#include <thread>
#include <future>
#include <QProgressBar>
#include "engine/bot/progresssignaler.hpp"
#include <QProcess>
#include <QWidget>
#include <QFileDialog>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spinBox->setMinimum(1);
    ui->spinBox->setMaximum(10000);
    ui->spinBox_2->setMinimum(1);
    ui->spinBox_2->setMaximum(std::thread::hardware_concurrency());
    ui->progressBar->setValue(0);
    ui->progressBar_2->setMinimum(0);
    ui->progressBar_2->setMaximum(0);
    ps = new ProgressSignaler();
    connect(this->ps, &ProgressSignaler::updateGlobalProgress, this, &MainWindow::globalProgress);
    connect(this->ps, &ProgressSignaler::updateLocalProgress, this, &MainWindow::localProgress);
    connect(this->ps, &ProgressSignaler::updateLocalRange, this, &MainWindow::localRange);
    connect(this->ps, &ProgressSignaler::done, this, &MainWindow::doneTraining);
    connect(this->ps, &ProgressSignaler::sendText, this, &MainWindow::stdoutToTextBox);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    this->ui->spinBox->setEnabled(false);
    this->ui->spinBox_2->setEnabled(false);
    this->ui->pushButton_2->setEnabled(false);
    this->ui->pushButton->setEnabled(false);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(ui->spinBox->value());
    new std::thread(AI::Neat::train,ui->spinBox->value(), ui->spinBox_2->value(), ps);
}

void MainWindow::globalProgress(int value){
    this->ui->progressBar->setValue(value);
    QApplication::processEvents();
}

void MainWindow::localRange(int lower, int upper){
    ui->progressBar_2->reset();
    ui->progressBar_2->setRange(lower, upper);
}
void MainWindow::localProgress(){
    ui->progressBar_2->setValue(ui->progressBar_2->value() + 1);
}

void MainWindow::doneTraining(){
    this->ui->spinBox->setEnabled(true);
    this->ui->spinBox_2->setEnabled(true);
    this->ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
}

void MainWindow::stdoutToTextBox(QString text){
    this->ui->textEdit->append(text);
}



void MainWindow::on_pushButton_2_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Population"), "/home", tr("Population files (*.population)"));
    AI::popPath = fileName.toStdString();
    if(AI::popPath != ""){
        ui->textEdit->append(QString("Loaded population path: ").append(AI::popPath));
    }
}

