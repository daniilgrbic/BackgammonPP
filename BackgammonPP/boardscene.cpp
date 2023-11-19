#include "boardscene.h"

BoardScene::BoardScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->setSceneRect(QRectF(0, 0, 1, 1));
    this->setBoardTriangles();
}

BoardScene::~BoardScene()
{
    for (int i = 0; i < (int)boardTriangles.size(); ++i) {
        delete boardTriangles[i];
    }
}

void BoardScene::setBoardTriangles() {
    // add triangles to boardTriangles
    for (int i = 0; i < this->triangleNumber ; ++i) {
        qreal x_point = (i / (qreal)this->triangleNumber) * this->m_width;

        qreal bottom_triangle_point = ((qreal)this->m_height - this->triangleHeight) * this->m_height;
        qreal upper_triangle_point = 0.0;

        BoardTriangle *bottomTriangle = new BoardTriangle(nullptr, this->triangleWidth, this->triangleHeight);
        bottomTriangle->setPos(x_point, bottom_triangle_point);

        BoardTriangle *upperTriangle = new BoardTriangle(nullptr, this->triangleWidth, this->triangleHeight);
        upperTriangle->setPos(x_point, upper_triangle_point);

        boardTriangles.push_front(upperTriangle);
        boardTriangles.push_back(bottomTriangle);
    }
}
