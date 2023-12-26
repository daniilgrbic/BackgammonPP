#include "checkerholder.h"

CheckerHolder::CheckerHolder()
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

void CheckerHolder::enableCheckers(const PlayerColor &color){
    for(BoardChecker *checker: m_checkers){
        if(checker->getColor() == color){
            checker->setEnabledUpdate(true);
        }
    }
}

void CheckerHolder::setAllowDropoff(bool allowDropoff){
    this->m_allowDropoff = allowDropoff;
    this->updateHighlighting();
}

bool CheckerHolder::canDropoff(){
    return m_allowDropoff;
}
