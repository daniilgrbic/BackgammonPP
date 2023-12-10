#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::cout << this->backgroundPicPath.toStdString() << std::endl;
    QPixmap pix(this->backgroundPicPath);
    ui->bg_label->setPixmap(pix);

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

