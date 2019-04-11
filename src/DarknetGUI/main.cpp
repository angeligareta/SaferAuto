#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <thread>

#include <QApplication>
#include <mainwindow.h>

#define OPENCV
//#define TRACK_OPTFLOW
#define GPU

#include "yolo_v2_class.hpp" // imported functions from DLL

#ifdef OPENCV
#include <opencv2/opencv.hpp>			// C++
#include "opencv2/core/version.hpp"
#ifndef CV_VERSION_EPOCH
#include "opencv2/videoio/videoio.hpp"
#pragma comment(lib, "opencv_world320.lib")
#else
#pragma comment(lib, "opencv_core2413.lib")
#pragma comment(lib, "opencv_imgproc2413.lib")
#pragma comment(lib, "opencv_highgui2413.lib")
#endif

cv::Scalar BOX_COLOR(60, 160, 260);


cv::Mat draw_boxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names, unsigned int wait_msec = 0) {
    for (auto &i : result_vec) {
        cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), BOX_COLOR, 3);
        if(i.obj_id < obj_names.size())
            putText(mat_img, obj_names[i.obj_id], cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, BOX_COLOR);
        if(i.track_id > 0)
            putText(mat_img, std::to_string(i.track_id), cv::Point2f(i.x + 5, i.y + 15), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, BOX_COLOR);
    }

    return mat_img;
    //cv::imshow("Live detections", mat_img);
    //cv::waitKey(wait_msec);
}
#endif	// OPENCV


void show_result(std::vector<bbox_t> const result_vec, std::vector<std::string> const obj_names) {
    for (auto &i : result_vec) {
        if (obj_names.size() > i.obj_id) std::cout << obj_names[i.obj_id] << " - ";
        std::cout << "obj_id = " << i.obj_id << ",  x = " << i.x << ", y = " << i.y
            << ", w = " << i.w << ", h = " << i.h
            << std::setprecision(3) << ", prob = " << i.prob << std::endl;
    }
}

std::vector<std::string> objects_names_from_file(std::string const filename) {
    std::ifstream file(filename);
    std::vector<std::string> file_lines;

    if (!file.is_open()) {
      return file_lines;
    }

    // Read lines from object names file.
    for(std::string line; file >> line;) {
        file_lines.push_back(line);
    }
    std::cout << "Object names loaded \n";
    return file_lines;
}


void process_video(std::string cfg_file, std::string names_file, std::string weights_file, std::string filename, MainWindow* window, QApplication* app)
{
    unsigned int time_bt_detections = 1;

    Detector detector(cfg_file, weights_file);
    auto obj_names = objects_names_from_file(names_file);
    preview_boxes_t large_preview(100, 150, false);

    try {
        std::string const file_ext = filename.substr(filename.find_last_of(".") + 1);
        if (file_ext == "avi" || file_ext == "mp4" || file_ext == "mjpg" || file_ext == "mov") {	// video file
            cv::Mat frame;
            detector.nms = 0.02;	// comment it - if track_id is not required

            for(cv::VideoCapture cap(filename); cap >> frame, cap.isOpened();) {
                auto begin = std::chrono::steady_clock::now();
                std::vector<bbox_t> result_vec = detector.detect(frame, 0.2);
                result_vec = detector.tracking_id(result_vec);	// comment it - if track_id is not required
                auto end = std::chrono::steady_clock::now();

                double elapsed_secs = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
                int fps = std::round(1000.0/elapsed_secs);
                std::cout << "CURRENT FPS: " << fps << " fps" << std::endl;

                cv::Mat imdisplay = draw_boxes(frame, result_vec, obj_names, time_bt_detections);
                window -> display_image(imdisplay, app);

                large_preview.set(frame, result_vec);
                show_result(result_vec, obj_names);
            }
        }
        else {	// image file
            cv::Mat mat_img = cv::imread(filename);
            std::vector<bbox_t> result_vec = detector.detect(mat_img);
            draw_boxes(mat_img, result_vec, obj_names);
            show_result(result_vec, obj_names);
        }
    }
    catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << "\n";;
    }
    catch (...) {
        std::cerr << "unknown exception \n";
    }
}

int main(int argc, char **argv)
{
    std::string  names_file = "../../darknet/cfg/general/general.names";
    std::string  cfg_file = "../../darknet/cfg/general/yolov3-spp.cfg";
    std::string  weights_file = "../../darknet/weights/general/yolov3-spp_4000.weights";
    //std::string  weights_file = "../../darknet/weights/btsdb/yolov3-spp_5000.weights";
    std::string  filename = "../../darknet/test-video-light.mp4";

    QApplication app (argc, argv);
    MainWindow window;
    window.show();

    process_video(cfg_file, names_file, weights_file, filename, &window, &app);

    return app.exec();
}
