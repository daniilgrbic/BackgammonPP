#include "controller.h"
#include <QApplication>
#include <QVector>
#include <iostream>
#include <algorithm>
#include <string>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller controller;
    return a.exec();
}
