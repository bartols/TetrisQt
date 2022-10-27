#ifndef PIECEFACTORY_H
#define PIECEFACTORY_H

#include "piece.h"

// tetris piece factory
class PieceFactory
{
public:
    static Piece* createRandom(const QPoint& pos);
    static Piece* create(ShapeType shtype, Rotation rottype, const QPoint& pos);
};

#endif // PIECEFACTORY_H
