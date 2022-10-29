#include "board.h"

#include "piece_factory.h"

#include <QGraphicsLineItem>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QKeyEvent>
#include <QTime>
#include <QThread>
#include <QString>
#include <QMessageBox>

#ifdef QT_DEBUG
   #include <QDebug>
   // #define DEBUG_DONT_MOVE
#endif

Board::Board(QObject *parent)
    :QGraphicsScene(parent)
{
    upBorder = new QGraphicsLineItem;
    upBorder->setLine(BOARD_ORIG_X,-10,400,-10);
    this->addItem(upBorder);

    leftBorder = new QGraphicsLineItem;
    leftBorder->setLine(198,-10,198,400);
    this->addItem(leftBorder);

    rigthBorder = new QGraphicsLineItem;
    rigthBorder->setLine(401,-10,401,400);
    this->addItem(rigthBorder);

    bottomBorder = new QGraphicsLineItem;
    bottomBorder->setLine(BOARD_ORIG_X,400,BOARD_ORIG_X+BOARD_ORIG_WIDTH,400);
    this->addItem(bottomBorder);

    this->setSceneRect(0,0,580,410);

    QGraphicsTextItem *text = this->addText("Left Arrow  - Move Left\n"
                                            "Right Arrow - Move Right\n"
                                            "Down Arrow  - Move Down\n\n"
                                            "a key - rotate left\n"
                                            "d key - rotate right\n"
                                            "space - fall");
    text->setPos(2, 20);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));

    newPiece();
}

Board::~Board()
{
}

void Board::start()
{
    _timer.start(SPEED);
}

void Board::stop()
{
    _timer.stop();
}

#ifdef QT_DEBUG
void Board::test(const QString &message)
{
    if(message == "Fade")
    {
        auto elements = _current_piece->move_elements();
        for(auto* element : elements)
        {
            fadeOut(element);
        }
    }
}
#endif

void Board::keyPressEvent(QKeyEvent * keyEvent)
{
    if(!_timer.isActive())
    {
        return;
    }

    // left arrow - move left
    if(keyEvent->key() == Qt::Key_Left)
    {
        applyAction(Piece::move_left);
    }
    // rigth arrow - move right
    else if(keyEvent->key() == Qt::Key_Right)
    {
        applyAction(Piece::move_right);
    }
    // down arrow - move down
    else if(keyEvent->key() == Qt::Key_Down)
    {
        applyAction(Piece::move_down);
    }
#ifdef QT_DEBUG
    // up arrow - move up ONLY FOR DEBUG
    else if(keyEvent->key() == Qt::Key_Up)
    {
        applyAction(Piece::move_up);
    }
#endif
    // A key - rotate left
    else if(keyEvent->key() == Qt::Key_A)
    {
        applyAction(Piece::rotate_left);
    }
    // d key - rotate rigth
    else if(keyEvent->key() == Qt::Key_D)
    {
        applyAction(Piece::rotate_rigth);
    }
    // space key - go down
    else if(keyEvent->key() == Qt::Key_Space)
    {
        while( _current_piece->action(Piece::move_down) )
        {
        }
        applyAction(Piece::move_down);
    }
}

void Board::applyAction(Piece::Action act)
{
    if(_current_piece == nullptr)
    {
        return;
    }

    // can move
    if(_current_piece->action(act) )
    {
        return;
    }

    // can't move but not in down
    if( act != Piece::move_down)
    {
        return;
    }

    // piece cannot move anymore, fill tiles struct
    auto elements = _current_piece->move_elements();
#ifdef _DEBUG
//    qDebug() << "Take" << to_string(_current_piece->type()) << to_string(_current_piece->rotation());
#endif
    for(auto* element : elements)
    {
        int x = (element->pos().x() - BOARD_ORIG_X) / BLOCK_SIZE;
        int y = element->pos().y() / BLOCK_SIZE;
#ifdef _DEBUG
//        qDebug() << element->pos();
//        qDebug() << x << y;
#endif
        assert(x<NUM_W_BLOCK);
        assert(y<NUM_H_BLOCK);
        m_tiles[x][y] = element;
    }

    delete _current_piece;

    //
    // check if any row full (if a row is all empty we could stop)
    //
#ifdef _DEBUG
//    for(int y=0; y < NUM_H_BLOCK; ++y)
//    {
//        QString str_row;
//        for(int x=0; x<NUM_W_BLOCK; ++x)
//        {
//            str_row += (m_tiles[x][y] == nullptr) ? "[ ]" : "[X]";
//        }
//        qDebug() << str_row;
//    }
#endif
    auto checkRows = [&]() -> int {
        for(int y=NUM_H_BLOCK-1; y >= 0; --y)
        {
            bool filled{true};
            for(int x=0; x<NUM_W_BLOCK; ++x)
            {
                if(m_tiles[x][y] == nullptr)
                {
                    filled = false;
                    break;
                }
            }
            if( filled )
                return y;
        }
        return -1;
    };

    int row;
    while((row = checkRows()) != -1)
    {
#ifdef _DEBUG
        qDebug() << "Found row fill"<< row;
#endif
        // destroy row
        for(int x=0; x<NUM_W_BLOCK; ++x)
        {
            fadeOut(m_tiles[x][row]);
            m_tiles[x][row] = nullptr;
        }

        // todo meglio non rispetta la gravita'
        // low every row-1
        for(int y=row-1; y>=0; --y)
        {
            bool empty{true};
            for(int x=0; x<NUM_W_BLOCK; ++x)
            {
                if(m_tiles[x][y] != nullptr)
                {
                    empty = false;
                    m_tiles[x][y]->moveBy(0, BLOCK_SIZE);
                    m_tiles[x][y+1] = m_tiles[x][y];
                    m_tiles[x][y] = nullptr;
                }
            }
            if(empty)
                break;
        }

        break;
    }

    // start new piece
    newPiece();
}

void Board::fadeOut(QGraphicsPixmapItem *item)
{
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    item->setGraphicsEffect(effect);
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(500);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->setEasingCurve(QEasingCurve::OutBack);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
    connect(animation, &QPropertyAnimation::finished, this, [item, this](){
        removeItem( item );
        delete item;
    });
}

void Board::update()
{
    if(_current_piece == nullptr)
    {
        return;
    }

// I don't love ifndef
#ifdef DEBUG_DONT_MOVE
#else
    applyAction(Piece::move_down);
#endif
}

void Board::newPiece()
{
    if(_next_piece == nullptr)
    {
        _next_piece = PieceFactory::createRandom(*this, {480, 0});
        _current_piece = PieceFactory::createRandom(*this, {280, 0});
    }
    else
    {
        _current_piece = _next_piece;
        _current_piece->set_pos({280, 0});
        _next_piece = PieceFactory::createRandom(*this, {480, 0});
    }

    if( _current_piece->isColliding() )
    {
#ifdef QT_DEBUG
        qDebug() << "New Piece colliding";
#endif
        _current_piece->action(Piece::move_down, false);
    }

    // if still collinding game over
    if( _current_piece->isColliding() )
    {
#ifdef QT_DEBUG
        qDebug() << "GameOver";
#endif
        _timer.stop();

        QMessageBox msgBox;
        msgBox.setText("GAME OVER");
        msgBox.exec();

        for(int y=0; y < NUM_H_BLOCK; ++y)
        {
            for(int x=0; x<NUM_W_BLOCK; ++x)
            {
                if(m_tiles[x][y] != nullptr)
                {
                    removeItem(m_tiles[x][y]);
                    delete m_tiles[x][y];
                    m_tiles[x][y] = nullptr;
                }
            }
        }

        delete _current_piece;
        _current_piece = nullptr;
        delete _next_piece;
        _next_piece = nullptr;

        QTimer::singleShot(500, [this](){
            newPiece();
        });
    }
}
