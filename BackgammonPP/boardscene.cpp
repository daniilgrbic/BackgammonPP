#include "boardscene.h"

BoardScene::BoardScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->setSceneRect(QRectF(0, 0, 1, 1));
    this->setBoardTriangles();
}

void BoardScene::setBoardTriangles() {
    for (int i = this->triangleNumber - 1; i >= 0 ; --i) {
        qreal left_point = (i / (qreal)this->triangleNumber) * this->m_width;
        // qreal right_point = ((i + 1) / (qreal)this->triangleNumber) * this->m_width;

        qreal bottom_triangle_point = ((qreal)this->m_height - this->triangleHeight) * this->m_height;
        // qreal upper_triangle_point = this->triangleHeight * this->m_height;

        BoardTriangle *bottomTriangle = new BoardTriangle(nullptr, this->triangleWidth, this->triangleHeight);
        bottomTriangle->setPos(left_point, bottom_triangle_point);

        BoardTriangle *upperTriangle = new BoardTriangle(nullptr, this->triangleWidth, this->triangleHeight);
        upperTriangle->setPos(left_point, 0.0);


        boardTriangles.push_back(bottomTriangle);
        boardTriangles.push_back(upperTriangle);
    }
}
