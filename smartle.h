#ifndef SMARTLE_H
#define SMARTLE_H

#include <QMainWindow>

#include <triangle.h>
#include <customscene.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Smartle; }
QT_END_NAMESPACE

class Smartle : public QMainWindow
{
    Q_OBJECT

public:
    Smartle(QWidget *parent = nullptr);
    ~Smartle();

public slots:
   void coorfChanged(QPointF);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::Smartle *ui;

    CustomScene  *scene;
    Triangle *triangle;
    qreal angle{};
};
#endif // SMARTLE_H
