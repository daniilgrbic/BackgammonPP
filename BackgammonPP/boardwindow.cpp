#include "boardwindow.h"
#include "ui_boardwindow.h"

BoardWindow::BoardWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoardWindow)
{
    ui->setupUi(this);
}

BoardWindow::~BoardWindow()
{
    delete ui;
}
