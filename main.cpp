#include "mainwindow.h"
#include <QApplication>
#include "abstract.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Table");
    w.show();

    return a.exec();
}
