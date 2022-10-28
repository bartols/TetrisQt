#ifndef PIECEFACTORY_H
#define PIECEFACTORY_H

#include "piece.h"

// tetris piece factory
class PieceFactory
{
public:
    static Piece* createRandom(QGraphicsScene& scene, const QPoint& pos);
    static Piece* create(QGraphicsScene& scene, ShapeType shtype, Rotation rottype, const QPoint& pos);
};

#endif // PIECEFACTORY_H
