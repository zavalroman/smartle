#include "triangle.h"
#include <math.h>

#include <QDebug>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

Triangle::Triangle(QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    setRotation(0);      /// Устанавливаем исходный разворот треугольника

    target = QPointF(0,0);  /// Устанавливаем изначальное положение курсора
    shot = false;

    gameTimer = new QTimer();   /// Инициализируем игровой таймер
    /// Подключаем сигнал от таймера и слоту обработки игрового таймера
    connect(gameTimer, &QTimer::timeout, this, &Triangle::slotGameTimer);
    gameTimer->start(50);   /// Стартуем таймер

    bulletTimer = new QTimer(); /// Инициализируем таймер создания пуль
    connect(bulletTimer, &QTimer::timeout, this, &Triangle::slotBulletTimer);
    bulletTimer->start(1000/6); /// Стреляем 6 раз в секунду
}

Triangle::~Triangle()
{

}

void Triangle::setAngle(qreal angle)
{
    //this->angle = angle;
}

void Triangle::thrust()
{
    force += 10;
}

void Triangle::rotateLeft()
{
    setRotation(rotation()-10);
}

void Triangle::rotateRight()
{
    setRotation(rotation()+10);
}

// Изменены размеры героя
QRectF Triangle::boundingRect() const
{
    return QRectF(-12,-15,24,30);
}

/* Возвращаем форму героя
 * В данном случае лучше использовать эллипс
 * */
QPainterPath Triangle::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void Triangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /** Отрисовка треугольника
     * */
    QPolygon polygon;
    polygon << QPoint(0,-15) << QPoint(12,15) << QPoint(-12,15);
    painter->setBrush(Qt::red);
    painter->drawPolygon(polygon);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Triangle::slotTarget(QPointF point)
{
    return;
    /// Определяем расстояние до цели
    target = point;
    QLineF lineToTarget(QPointF(0, 0), mapFromScene(target));
    /// Угол поворота в направлении к цели
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    if (lineToTarget.dy() < 0)
        angleToTarget = TwoPi - angleToTarget;

    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);


    /** В Зависимости от того, слева или справа находится Муха от Паука,
     * устанавливаем направление поворота паука в данном тике таймера
     * Скорость разворота зависит от угла, на который необходимо повернуться треугольнику
     * */
    if (angleToTarget >= 0 && angleToTarget < Pi) {
        /// Rotate left
        setRotation(rotation() - angleToTarget * 180 /Pi);
    } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
        /// Rotate right
        setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
    }
}

void Triangle::slotGameTimer()
{
    angle = rotation() * Pi / 180;
    angle = angle - Pi / 2;
    //qDebug() << angle;

    qreal x_shift = force * cos(angle);
    qreal y_shift = force * sin(angle);

    x_force += x_shift;
    y_force += y_shift;

    setX(this->x()+x_force);
    setY(this->y()+y_force);

    if(abs(x_force)>1) x_force -= x_force/20;
                    else x_force  = 0.0;

    if(abs(y_force)>1) y_force -= y_force/20;
                    else y_force  = 0.0;
    //vector = QLineF(QPointF(0.0,0.0),QPointF(x_force*10,y_force*(-10)));

    force = 0;
    //qDebug() << x_shift << y_shift;
}

void Triangle::slotBulletTimer()
{
    /// Если стрельба разрешена, то вызываем сигнал на создание пули
    if(shot) emit signalBullet(mapToScene(0,-25), target);
}

void Triangle::slotShot(bool shot)
{
    this->shot = shot;  /// Получаем разрешение или запрет на стрельбу
}
