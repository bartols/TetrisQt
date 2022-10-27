#ifndef PIECE_H
#define PIECE_H

#include <QPoint>

//#include <QGraphicsItemGroup>
enum class ShapeType : int {
    SquareShape,
    LineShape,
    MirroredLShape,
    SShape,
    ZShape,
    LShape,
    TShape
};

enum class Rotation : int {
    rot_0,
    rot_90,
    rot_180,
    rot_270,
};

class QGraphicsRectItem;
class QGraphicsScene;

class Piece
{
public:
    enum Action {
        move_down,
        move_left,
        move_right,
        rotate_left,
        rotate_rigth
    };

    Piece() = delete;
    Piece(ShapeType type, Rotation rotation, const QPoint &pos);
    Piece(Piece&& other) noexcept;

    bool action(Action action);

    void add(QGraphicsScene& scene);

private:
    ShapeType _type;
    Rotation _rotation;
    QGraphicsRectItem * _elements[4] = {};
    QPoint _position;
    //QGraphicsItemGroup _group;

    // redraw blocks regarding to rotation, type and point
    void redraw();

    Rotation rotate(int versus) const;
};

#endif // PIECE_H
