#include "include/detectionwindow.h"
#include "include/mainwindow.h"
#include "include/yolo.h"

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    MainWindow start_window;
    start_window.show();

    return app.exec();
}
