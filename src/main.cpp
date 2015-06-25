#include "analytichiearchymainwindow.h"
#include <QApplication>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load(":/text/text/qt_ru.qm");
    a.installTranslator(&translator);

    AnalyticHiearchyMainWindow w;
    w.show();

    return a.exec();
}
