#include "controller.h"
#include <QApplication>
#include <QVector>
#include <iostream>
#include <algorithm>
#include <string>

auto main(int argc, char *argv[]) -> int
{
    QApplication a(argc, argv);
    Controller controller;
    return a.exec();
}
