#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix(this->backgroundPicPath);
    pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
    // ui->bg_label->setPixmap(pix);

    QPalette palette;
    palette.setBrush(QPalette::Window, pix);
    this->setPalette(palette);


    boardWindow = new BoardWindow();
    connect(ui->btCreateGame, SIGNAL(clicked()), this, SLOT(on_btCreateGame_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btCreateGame_clicked()
{
    boardWindow->show();
    this->close();
}


void MainWindow::on_btExit_clicked()
{
    this->close();
}

