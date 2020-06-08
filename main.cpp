#include "smartle.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Smartle w;
    w.show();
    return a.exec();
}
