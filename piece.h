#ifndef PIECE_H
#define PIECE_H

#include <QPoint>
#include <array>

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
#ifdef QT_DEBUG
const char * to_string(ShapeType type);
#endif

enum class Rotation : int {
    rot_0,
    rot_90,
    rot_180,
    rot_270,
};
#ifdef QT_DEBUG
const char * to_string(Rotation rot);
#endif

class QGraphicsRectItem;
class QGraphicsScene;

class Piece
{
public:
    enum Action {
        no_action,
        move_up,
        move_down,
        move_left,
        move_right,
        rotate_left,
        rotate_rigth
    };

    Piece() = delete;
    Piece(const Piece&) = delete;
    Piece(QGraphicsScene& scene, ShapeType type, Rotation rotation, const QPoint &pos);
    ~Piece();

    ShapeType type() const      { return _type; }
    Rotation rotation() const   { return _rotation; }

    // move/rotate piece
    bool action(Action action, bool check_collines = true);

    // collide with something
    bool isColliding();

    // move elements pointers
    std::array<QGraphicsRectItem *,4> move_elements();

private:
    QGraphicsScene& _scene;
    ShapeType _type;
    Rotation _rotation;
    std::array<QGraphicsRectItem *,4> _elements = {};
    QPoint _position;
    Action _last_action = no_action;
    //QGraphicsItemGroup _group;

    // redraw blocks regarding to rotation, type and point
    void redraw();

    Rotation rotate(int versus) const;
};

#endif // PIECE_H
