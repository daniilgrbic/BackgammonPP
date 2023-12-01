#include "checkerholder.h"

CheckerHolder::CheckerHolder()
    : m_checkers(QVector<BoardChecker*>())
{

}

void CheckerHolder::addChecker(BoardChecker *checker)
{
    if(checker->getHolder()){
        checker->getHolder()->removeChecker(checker);

    }
    checker->setHolder(this);
    m_checkers.push_back(checker);
    updateCheckerPos();
}

void CheckerHolder::removeChecker(BoardChecker *checker){
    m_checkers.erase(std::remove(m_checkers.begin(), m_checkers.end(), checker), m_checkers.end());
    updateCheckerPos();
}
