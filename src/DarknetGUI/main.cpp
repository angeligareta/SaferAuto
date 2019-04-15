#include <QApplication>
#include <mainwindow.h>
#include <startwindow.h>
#include "yolo.h"

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    //MainWindow window(yolo);
    //window.show();

    StartWindow start_window;
    start_window.show();

    return app.exec();
}
