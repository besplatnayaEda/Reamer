#include "reamerabs.h"
#include <QApplication>
// слив на реп
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ReamerABS w;
    w.show();

    return a.exec();
}
