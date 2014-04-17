#include "reamerabs.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ReamerABS w;
    w.show();

    return a.exec();
}
