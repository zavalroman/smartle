#include "smartle.h"
#include "ui_smartle.h"
#include <math.h>

#include <QKeyEvent>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Smartle::Smartle(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Smartle)
{
    //this->resize(600,600);
    //this->setFixedSize(600,600);

    ui->setupUi(this);

    scene = new CustomScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene->setSceneRect(0,0,520,520);
    triangle = new Triangle();
    triangle->setPos(60,60);
    triangle->setZValue(2);
    scene->addItem(triangle);

    ui->graphicsView->setMouseTracking(true);

    connect(scene, &CustomScene::signalTargetCoordinate, triangle, &Triangle::slotTarget);
    connect(scene, &CustomScene::signalTargetCoordinate, this, &Smartle::coorfChanged);

}

Smartle::~Smartle()
{
    delete ui;
}

void Smartle::coorfChanged(QPointF p)
{
    QLineF vector(triangle->pos(), p);
    angle = ::acos(vector.dx() / vector.length());
    if (vector.dy() < 0)
        angle = TwoPi - angle;
    //triangle->setAngle(angleToTarget);
}

void Smartle::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << event->text();
    if(event->key() == Qt::Key_Space) {
        triangle->setAngle(angle);
        triangle->thrust();
    } else if(event->key() == Qt::Key_A) {
        triangle->rotateLeft();
    } else if(event->key() == Qt::Key_D) {
        triangle->rotateRight();
    } else {
       qDebug() << "You Pressed Other Key";
    }

}

void Smartle::keyReleaseEvent(QKeyEvent *event)
{
    //qDebug() << event->text();
    //if(event->key() == Qt::Key_Q) {
    //    qDebug() << "You Released Key Q";
    //} else {
    //    qDebug() << "You Released Other Key";
    //}
}
