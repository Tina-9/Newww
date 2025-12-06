#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Set app icon
    a.setWindowIcon(QIcon("data/images/background.png"));

    MainWindow w;
    w.show();

    return a.exec();
}
