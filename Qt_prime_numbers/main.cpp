#include "mainwindow.h"
#include <QApplication>
#include <QMenu>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;

    w.getMainMenu().addAction("&Exit",
                              &app, SLOT( quit() )
                            );


    w.show();

    return app.exec();
}
