#ifndef CHECKERHOLDER_H
#define CHECKERHOLDER_H
#include "boardchecker.h"
#include <QVector>

class BoardChecker;

class CheckerHolder
{
public:
    CheckerHolder();
    virtual void addChecker(BoardChecker* checker);
    virtual void removeChecker(BoardChecker* checker);
    virtual void updateCheckerPos() = 0;
    QVector<BoardChecker*> m_checkers;
};

#endif // CHECKERHOLDER_H
