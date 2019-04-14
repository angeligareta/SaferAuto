#ifndef YOLO_H
#define YOLO_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <thread>

#define OPENCV
//#define TRACK_OPTFLOW
#define GPU

#include "yolo_v2_class.hpp" // imported functions from DLL
#include "opencv2/core/version.hpp"
#include "opencv2/videoio/videoio.hpp"
#pragma comment(lib, "opencv_world320.lib")

class MainWindow;

/*#ifndef MAINWINDOW_H
#include "mainwindow.h"
#include <opencv2/opencv.hpp> // C++s
#endif*/

class YOLO
{

private:
    std::string names_file;
    std::string cfg_file;
    std::string weights_file;
    std::string filename;

public:
    YOLO();
    YOLO(std::string cfg_file, std::string names_file, std::string weights_file, std::string filename);
    cv::Mat draw_boxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names, unsigned int wait_msec = 0);
    void show_result(std::vector<bbox_t> const result_vec, std::vector<std::string> const obj_names);
    std::vector<std::string> objects_names_from_file(const std::string filename);
    void process_video(MainWindow *window);
};

#endif // YOLO_H
#include "mainwindow.h"

