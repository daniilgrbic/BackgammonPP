#ifndef CHECKERHOLDER_H
#define CHECKERHOLDER_H
#include "boardchecker.h"
#include <QVector>

class BoardChecker;

class CheckerHolder
{
public:
    CheckerHolder();
    virtual void addChecker(BoardChecker* checker) = 0;
    virtual void removeChecker(BoardChecker* checker) = 0;
    QVector<BoardChecker*> m_checkers;
};

#endif // CHECKERHOLDER_H
