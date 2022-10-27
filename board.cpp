#include "board.h"

#include "piece_factory.h"

#include <QGraphicsLineItem>
#include <QKeyEvent>
#include <QTime>

#ifdef QT_DEBUG
   #include <QDebug>
#endif

Board::Board(QObject *parent)
    :QGraphicsScene(parent)
{
    leftBorder = new QGraphicsLineItem;
    leftBorder->setLine(198,-10,198,400);
    this->addItem(leftBorder);

    rigthBorder = new QGraphicsLineItem;
    rigthBorder->setLine(401,-10,401,400);
    this->addItem(rigthBorder);

    bottomBorder = new QGraphicsLineItem;
    bottomBorder->setLine(201,400,399,400);
    this->addItem(bottomBorder);

    this->setSceneRect(0,0,580,410);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));

    _current_piece = PieceFactory::createRandom({280, 0});
    _current_piece->add(*this);
}

void Board::start()
{
    _timer.start(SPEED);
}

void Board::stop()
{
    _timer.stop();
}

void Board::keyPressEvent(QKeyEvent * keyEvent)
{
    if(!_timer.isActive())
    {
        return;
    }

    if(_current_piece == nullptr)
    {
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "key" << keyEvent->key();
#endif

    // left arrow - move left
    if(keyEvent->key() == Qt::Key_Left)
    {
        _current_piece->action(Piece::move_left);
    }
    // rigth arrow - move right
    else if(keyEvent->key() == Qt::Key_Right)
    {
        _current_piece->action(Piece::move_right);
    }
    // down arrow - move down
    else if(keyEvent->key() == Qt::Key_Down)
    {
        _current_piece->action(Piece::move_down);
    }
    // A key - rotate left
    else if(keyEvent->key() == Qt::Key_A)
    {
        _current_piece->action(Piece::rotate_left);
    }
    // d key - rotate rigth
    else if(keyEvent->key() == Qt::Key_D)
    {
        _current_piece->action(Piece::rotate_rigth);
    }
    // space key - go down
    else if(keyEvent->key() == Qt::Key_Space)
    {
        // todo
    }
}

void Board::update()
{
    if(_current_piece == nullptr)
    {
        return;
    }

    //_current_piece->action(Piece::move_down);
}

