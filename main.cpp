#include "MotormeterTest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MotormeterTest w;
    w.show();

    return a.exec();
}
