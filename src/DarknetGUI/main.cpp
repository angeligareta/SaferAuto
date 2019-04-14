#include <QApplication>
#include <mainwindow.h>
#include "yolo.h"

int main(int argc, char **argv)
{
    std::string  names_file = "../../darknet/cfg/general/general.names";
    std::string  cfg_file = "../../darknet/cfg/general/yolov3-spp.cfg";
    std::string  weights_file = "../../darknet/weights/general/yolov3-spp_4000.weights";
    //std::string  weights_file = "../../darknet/weights/btsdb/yolov3-spp_5000.weights";
    std::string  filename = "../../darknet/test-video-light.mp4";

    YOLO yolo(cfg_file, names_file, weights_file, filename);

    QApplication app (argc, argv);
    MainWindow window(yolo);
    window.show();

    return app.exec();
}
