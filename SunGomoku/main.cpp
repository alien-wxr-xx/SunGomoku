#include "xrUI/xrhall.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    xrHall hall;
    hall.show();

    return a.exec();
}
