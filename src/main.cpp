#include "analytichiearchymainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AnalyticHiearchyMainWindow w;
    w.show();

    return a.exec();
}
