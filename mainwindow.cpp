#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "board.h"

#include <QGraphicsSceneMouseEvent>

#include <QEvent>
#include <QDebug>

#define DEBUG_POS


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setGeometry(0,30,610,420);

    _scene = new Board(this);
#ifdef DEBUG_POS
    _scene->installEventFilter(this);
#endif
    ui->graphicsView->setScene(_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    qApp->exit();
}

// start
void MainWindow::on_pushButton_clicked()
{
    _scene->start();
    _scene->setFocus();
}

void MainWindow::on_pushButton_3_clicked()
{
    _scene->stop();
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneMousePress)
    {
        const QGraphicsSceneMouseEvent* const me = static_cast<const QGraphicsSceneMouseEvent*>(event);
        const QPointF position = me->scenePos();
        qDebug() << position.x() << "," << position.y() << endl;
    }
    return QMainWindow::eventFilter(target, event);
}
