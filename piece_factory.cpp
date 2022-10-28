#include "piece_factory.h"
#include "tetris_defs.h"

#include <QGraphicsRectItem>
#include <QRandomGenerator>

QRandomGenerator g_randGenerator(QRandomGenerator::securelySeeded());

Piece* PieceFactory::createRandom(QGraphicsScene &scene, const QPoint& pos)
{
    ShapeType shtype = static_cast<ShapeType>(g_randGenerator.bounded(0, 7));
    Rotation rottype = static_cast<Rotation>(g_randGenerator.bounded(0, 4));

    return new Piece(scene, shtype, rottype, pos);
}

Piece* PieceFactory::create(QGraphicsScene &scene, ShapeType shtype, Rotation rottype, const QPoint& pos)
{
    return new Piece(scene, shtype, rottype, pos);
}
