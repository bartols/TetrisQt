#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <QTimer>

#include "tetris_defs.h"
#include "piece.h"

// Main Platform Scene
class Board : public QGraphicsScene
{
    Q_OBJECT
public:
    Board(QObject *parent);
    ~Board();

    void start();
    void stop();

#ifdef QT_DEBUG
    void test(const QString& message);
#endif

private slots:
    void update();

private:
    QGraphicsLineItem * upBorder;
    QGraphicsLineItem * leftBorder;
    QGraphicsLineItem * rigthBorder;
    QGraphicsLineItem * bottomBorder;

    QTimer _timer;

    QGraphicsPixmapItem * m_tiles[NUM_W_BLOCK][NUM_H_BLOCK] = {};
    Piece * _current_piece = nullptr;
    Piece * _next_piece = nullptr;

    void keyPressEvent(QKeyEvent * keyEvent);
    void newPiece();
    void applyAction(Piece::Action act);
    void fadeOut(QGraphicsPixmapItem * item);
};

#endif // BOARD_H
