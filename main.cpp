#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load(QCoreApplication::applicationDirPath() +"/translations/own_gamepad_" + QLocale::system().name());
    a.installTranslator(&translator);
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);
    qDebug()<<QLocale::system().name();
    MainWindow w;
    w.show();

    return a.exec();
}
