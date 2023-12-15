#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    setFixedSize(width(), height());

    setPicture(this->backgroundPicPath, this);
    setPicture(this->sketchPicPath, ui->optionsFrame);

    connect(ui->btCreateGame, SIGNAL(clicked()), this, SLOT(on_btCreateGame_clicked()));
    connect(ui->btPreferences, SIGNAL(clicked()), this, SLOT(on_btPreference_clicked()));
    connect(ui->btReturnToMenu, SIGNAL(clicked()), this, SLOT(on_btReturnToMenu_clicked()));
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
    emit requestCreateGame();
}

void MainWindow::on_btPreferences_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btReturnToMenu_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btExit_clicked()
{
    this->close();
}

