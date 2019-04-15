#include <QApplication>
#include <detectionwindow.h>
#include <mainwindow.h>
#include "yolo.h"

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    MainWindow start_window;
    start_window.show();

    return app.exec();
}
