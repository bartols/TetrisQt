#include "piece.h"

#include <QGraphicsScene>
#include <QGraphicsRectItem>

#include "tetris_defs.h"
#include <algorithm>
#include <type_traits>

// cast enum to underling type in constexpr
template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

#ifdef QT_DEBUG
#include <QDebug>

const char * to_string(Rotation rot)
{
    switch(rot)
    {
    case Rotation::rot_0:   return "0 degree";
    case Rotation::rot_90:  return "90 degree";
    case Rotation::rot_180: return "180 degree";
    case Rotation::rot_270: return "270 degree";
    }
    return "???";
}

const char * to_string(ShapeType type)
{
    switch(type)
    {
    case ShapeType::SquareShape:    return "SquareShape";
    case ShapeType::LineShape:      return "LineShape";
    case ShapeType::MirroredLShape: return "MirroredLShape";
    case ShapeType::SShape:         return "SShape";
    case ShapeType::ZShape:         return "ZShape";
    case ShapeType::LShape:         return "LShape";
    case ShapeType::TShape:          return "TShapw";
    }
    return "???";
}
#endif

QPoint tetromini[7][4][4] = {
    // SquareShape
    {
        // 0
        { {0,0}, {BLOCK_SIZE, 0}, {0, BLOCK_SIZE}, {BLOCK_SIZE, BLOCK_SIZE} },
        // 90
        { {0,0}, {BLOCK_SIZE, 0}, {0, BLOCK_SIZE}, {BLOCK_SIZE, BLOCK_SIZE} },
        // 180
        { {0,0}, {BLOCK_SIZE, 0}, {0, BLOCK_SIZE}, {BLOCK_SIZE, BLOCK_SIZE} },
        // 270
        { {0,0}, {BLOCK_SIZE, 0}, {0, BLOCK_SIZE}, {BLOCK_SIZE, BLOCK_SIZE} },
    },
    // LShape
    {
        // 0
        { {0, 0}, {0, BLOCK_SIZE}, {0, BLOCK_SIZE*2}, {0, BLOCK_SIZE*3} },
        // 90
        { {-BLOCK_SIZE*2, 0}, {-BLOCK_SIZE, 0}, {0, 0}, {BLOCK_SIZE, 0} },
        // 180
        { {0, 0}, {0, BLOCK_SIZE}, {0, BLOCK_SIZE*2}, {0, BLOCK_SIZE*3} },
        // 270
        { {-BLOCK_SIZE*2, 0}, {-BLOCK_SIZE, 0}, {0, 0}, {BLOCK_SIZE, 0} },
    },
    // MirroredLShape
    {
        // 0
        { {BLOCK_SIZE, 0}, {BLOCK_SIZE, BLOCK_SIZE}, {BLOCK_SIZE, BLOCK_SIZE*2}, {0, BLOCK_SIZE*2} },
        // 90
        { {BLOCK_SIZE, BLOCK_SIZE}, {0, BLOCK_SIZE}, {-BLOCK_SIZE, BLOCK_SIZE}, {-BLOCK_SIZE, 0} },
        // 180
        { {0, BLOCK_SIZE}, {0, 0}, {0, -BLOCK_SIZE}, {BLOCK_SIZE, -BLOCK_SIZE} },
        // 270
        { {0, 0}, {BLOCK_SIZE, 0}, {BLOCK_SIZE*2, 0}, {BLOCK_SIZE*2, BLOCK_SIZE} },
    },
    // SShape
    {
        // 0
        { {0, 0}, {0, BLOCK_SIZE}, {BLOCK_SIZE, BLOCK_SIZE}, {BLOCK_SIZE, BLOCK_SIZE*2} },
        // 90
        { {BLOCK_SIZE, 0}, {0, 0}, {0, BLOCK_SIZE}, {-BLOCK_SIZE, BLOCK_SIZE} },
        // 180
        { {0, 0}, {0, BLOCK_SIZE}, {BLOCK_SIZE, BLOCK_SIZE}, {BLOCK_SIZE, BLOCK_SIZE*2} },
        // 270
        { {BLOCK_SIZE, 0}, {0, 0}, {0, BLOCK_SIZE}, {-BLOCK_SIZE, BLOCK_SIZE} },
    },
    // ZShape
    {
        // 0
        { {BLOCK_SIZE, 0}, {BLOCK_SIZE, BLOCK_SIZE}, {0, BLOCK_SIZE}, {0, BLOCK_SIZE*2} },
        // 90
        { {BLOCK_SIZE, BLOCK_SIZE}, {0, BLOCK_SIZE}, {0, 0}, {-BLOCK_SIZE, 0} },
        // 180
        { {BLOCK_SIZE, 0}, {BLOCK_SIZE, BLOCK_SIZE}, {0, BLOCK_SIZE}, {0, BLOCK_SIZE} },
        // 270
        { {BLOCK_SIZE, BLOCK_SIZE}, {0, BLOCK_SIZE}, {0, 0}, {-BLOCK_SIZE, 0} },
    },
    // LineShape
    {
        // 0
        { {0,0}, {0, BLOCK_SIZE}, {0, BLOCK_SIZE*2}, {BLOCK_SIZE, BLOCK_SIZE*2} },
        // 90
        { {BLOCK_SIZE, 0}, {0,0}, {-BLOCK_SIZE, 0}, {-BLOCK_SIZE, BLOCK_SIZE} },
        // 180
        { {BLOCK_SIZE, BLOCK_SIZE}, {BLOCK_SIZE, 0}, {BLOCK_SIZE, -BLOCK_SIZE}, {0, -BLOCK_SIZE} },
        // 270
        { {0, BLOCK_SIZE}, {BLOCK_SIZE, BLOCK_SIZE}, {BLOCK_SIZE*2, BLOCK_SIZE}, {BLOCK_SIZE*2, 0} },
    },
    // TShape
    {
        // 0
        { {0, 0}, {-BLOCK_SIZE, BLOCK_SIZE}, {0, BLOCK_SIZE}, {BLOCK_SIZE, BLOCK_SIZE} },
        // 90
        { {BLOCK_SIZE, 0}, {0, -BLOCK_SIZE}, {0, 0}, {0, BLOCK_SIZE} },
        // 180
        { {0, BLOCK_SIZE}, {BLOCK_SIZE, 0}, {0, 0}, {-BLOCK_SIZE, 0} },
        // 270
        { {-BLOCK_SIZE, 0}, {0, BLOCK_SIZE}, {0, 0}, {0, -BLOCK_SIZE} },
    }
};


Piece::Piece(Piece &&other) noexcept
{
    std::swap(_elements[0], other._elements[0]);
    std::swap(_elements[1], other._elements[1]);
    std::swap(_elements[2], other._elements[2]);
    std::swap(_elements[3], other._elements[3]);
    std::swap(_type, other._type);
    std::swap(_rotation, other._rotation);
}

Piece::Piece(ShapeType t, Rotation r, const QPoint &pos)
    :_type(t), _rotation(r), _position(pos)
{
#ifdef QT_DEBUG
    qDebug() << "New Piece " << to_string(_type) << to_string(_rotation);
#endif

    _elements[0] = new QGraphicsRectItem(0, 0, BLOCK_DIM, BLOCK_DIM);
    _elements[1] = new QGraphicsRectItem(0, 0, BLOCK_DIM, BLOCK_DIM);
    _elements[2] = new QGraphicsRectItem(0, 0, BLOCK_DIM, BLOCK_DIM);
    _elements[3] = new QGraphicsRectItem(0, 0, BLOCK_DIM, BLOCK_DIM);

    redraw();

#ifdef QT_DEBUG
    qDebug() << "1" << _elements[0]->pos();
    qDebug() << "2" << _elements[1]->pos();
    qDebug() << "3" << _elements[2]->pos();
    qDebug() << "4" << _elements[3]->pos();
#endif
}

bool Piece::action(Action action)
{
    if(action == move_down)
    {
        _position += QPoint(0, BLOCK_SIZE);
        std::for_each( std::begin(_elements), std::end(_elements), [](auto* elem){
            elem->moveBy(0,BLOCK_SIZE);
        });
    }
    else if(action == move_left)
    {
        _position += QPoint(-BLOCK_SIZE, 0);
        std::for_each( std::begin(_elements), std::end(_elements), [](auto* elem){
            elem->moveBy(-BLOCK_SIZE, 0);
        });
    }
    else if(action == move_right)
    {
        _position += QPoint(BLOCK_SIZE, 0);
        std::for_each( std::begin(_elements), std::end(_elements), [](auto* elem){
            elem->moveBy(BLOCK_SIZE, 0);
        });
    }
    else if(action == rotate_left)
    {
        _rotation = rotate(-1);
        redraw();
    }
    else if(action == rotate_rigth)
    {
        _rotation = rotate(1);
        redraw();
    }

    return true;
}

void Piece::add(QGraphicsScene &scene)
{
    scene.addItem(_elements[0]);
    scene.addItem(_elements[1]);
    scene.addItem(_elements[2]);
    scene.addItem(_elements[3]);
}

// redraw blocks regarding to rotation, type and point
void Piece::redraw()
{
    int type = to_underlying(_type);
    int type_rot  = to_underlying(_rotation);

    _elements[0]->setPos(_position.x() + tetromini[type][type_rot][0].x(),
                         _position.y() + tetromini[type][type_rot][0].y());

    _elements[1]->setPos(_position.x() + tetromini[type][type_rot][1].x(),
                         _position.y() + tetromini[type][type_rot][1].y());

    _elements[2]->setPos(_position.x() + tetromini[type][type_rot][2].x(),
                         _position.y() + tetromini[type][type_rot][2].y());

    _elements[3]->setPos(_position.x() + tetromini[type][type_rot][3].x(),
                         _position.y() + tetromini[type][type_rot][3].y());
}

Rotation Piece::rotate(int versus) const
{
    int type_rot = to_underlying(_rotation) + versus;
    return type_rot > 3 ? Rotation::rot_0 :
           type_rot < 0 ? Rotation::rot_270 : static_cast<Rotation>(type_rot);
}
