#include "checkerholder.h"

CheckerHolder::CheckerHolder()
    : m_checkers(QVector<BoardChecker*>())
{

}

void CheckerHolder::addChecker(BoardChecker *checker)
{
    if(checker->getHolder()){
        checker->getHolder()->removeChecker(checker);
        if(!checker->getHolder()->m_checkers.empty()){
            BoardChecker *lastChecker = checker->getHolder()->m_checkers.back();
            lastChecker->setFlag(QGraphicsItem::ItemIsSelectable, true);
            lastChecker->setFlag(QGraphicsItem::ItemIsMovable, true);
        }

    }
    checker->setHolder(this);
    if(!m_checkers.empty()){
        BoardChecker *lastChecker = m_checkers.back();
        lastChecker->setFlag(QGraphicsItem::ItemIsSelectable, false);
        lastChecker->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
    m_checkers.push_back(checker);
    checker->setFlag(QGraphicsItem::ItemIsSelectable, true);
    checker->setFlag(QGraphicsItem::ItemIsMovable, true);
    updateCheckerPos();
}

void CheckerHolder::removeChecker(BoardChecker *checker){
    m_checkers.erase(std::remove(m_checkers.begin(), m_checkers.end(), checker), m_checkers.end());
    updateCheckerPos();
}
