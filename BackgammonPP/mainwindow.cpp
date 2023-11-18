#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btHost_clicked()
{
   ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pbBackHosting_clicked()
{
   ui->stackedWidget->setCurrentIndex(0);
}



void MainWindow::on_btPrefs_clicked()
{
   ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_btJoin_clicked()
{
   ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pbBackJoin_clicked()
{
   ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pbExit_clicked()
{
   exit(0);
}


void MainWindow::on_pushButton_clicked()
{
   ui->stackedWidget->setCurrentIndex(0);
}

