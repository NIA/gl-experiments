#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug("Application a");
    QApplication a(argc, argv);
    qDebug("Widget w");
    Widget w;
    qDebug("w.show()");
    w.show();

    qDebug("a.exec()");
    return a.exec();
}
