#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <QTimer>

#include "tetris_defs.h"

class Piece;

// Main Platform Scene
class Board : public QGraphicsScene
{
    Q_OBJECT
public:
    Board(QObject *parent);

    void start();
    void stop();

    void keyPressEvent(QKeyEvent * keyEvent);

private slots:
    void update();

private:
    QGraphicsLineItem * leftBorder;
    QGraphicsLineItem * rigthBorder;
    QGraphicsLineItem * bottomBorder;

    QTimer _timer;

    QGraphicsItem * m_tiles[NUM_H_BLOCK][NUM_W_BLOCK] = {};
    Piece * _current_piece = nullptr;

};

#endif // BOARD_H
