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
//#define GPU

#include "yolo_v2_class.hpp" // imported functions from DLL
#include "opencv2/core/version.hpp"
#include "opencv2/videoio/videoio.hpp"
#pragma comment(lib, "opencv_world320.lib")

class DetectionWindow;

/*#ifndef MAINWINDOW_H
#include "detectionwindow.h"
#include <opencv2/opencv.hpp> // C++s
#endif*/

class YOLO
{

private:
    std::string cfg_file_;
    std::string names_file_;
    std::string weights_file_;
    std::string input_file_;
    const cv::Scalar BOX_COLOR = cv::Scalar(60, 160, 260);

public:
    YOLO();
    YOLO(std::string cfgFile, std::string namesFile, std::string weightsFile, std::string filename);

    cv::Mat drawBoxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names);
    void showResult(std::vector<bbox_t> const result_vec, std::vector<std::string> const obj_names, DetectionWindow *window);
    std::vector<std::string> getObjectNamesFromFile(const std::string filename);

    void processInputFile(DetectionWindow *window);
    void processVideoFile(Detector detector, std::vector<std::string> obj_names, DetectionWindow *window);
    void processImageFile(Detector detector, std::vector<std::string> obj_names, DetectionWindow *window);

    // GETTERS
    /**
     * @brief Getter of configuration file for Darknet.
     * @return
     */
    std::string getCfgFile() const;
    /**
     * @brief Getter of names file for Darknet.
     * @return
     */
    std::string getNamesFile() const;
    /**
     * @brief Getter of weights file for Darknet.
     * @return
     */
    std::string getWeightsFile() const;
    /**
     * @brief Getter of input file for Darknet detection.
     * @return
     */
    std::string getInputFile() const;

    // SETTERS
    /**
     * @brief Setter of configuration file for Darknet.
     * @param value
     */
    void setCfgFile(const std::string &value);
    /**
     * @brief Setter of names file for Darknet.
     * @param value
     */
    void setNamesFile(const std::string &value);
    /**
     * @brief Setter of weights file for Darknet.
     * @param value
     */
    void setWeightsFile(const std::string &value);
    /**
     * @brief Setter of input file for Darknet detection.
     * @param value
     */
    void setInputFile(const std::string &value);
};

#endif // YOLO_H
#include "detectionwindow.h"

