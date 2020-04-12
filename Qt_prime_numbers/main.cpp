#include "mainwindow.h"
#include <QApplication>
#include <QMenu>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;

    window.getMainMenu().addAction("&Exit", &app, SLOT(quit()));

    window.show();

    return app.exec();
}
