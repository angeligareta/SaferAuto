#ifndef YOLO_H
#define YOLO_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <QSettings>
#include <QString>
#include <QCoreApplication>

#define OPENCV
// #define TRACK_OPTFLOW
#define GPU

#include "yolo_v2_class.hpp" // imported functions from DLL
#include "opencv2/core/version.hpp"
#include "opencv2/videoio/videoio.hpp"
#pragma comment(lib, "opencv_world320.lib")

class DetectionWindow;

// CONFIG INI TAGS FOR FILES USED IN SAFER AUTO
const QString CFG_FILE = "cfg_file";
const QString NAMES_FILE = "names_file";
const QString WEIGHTS_FILE = "weights_file";
const QString INPUT_FILE = "input_file";
const QString CONFIG_FILE = "config.ini";

// DEFAULT PATHS FOR FILES USED IN SAFER AUTO
const QString CFG_FILE_DEFAULT_PATH = "./darknet/cfg/edb/yolov3-spp.cfg";
const QString NAMES_FILE_DEFAULT_PATH = "./darknet/cfg/edb/edb.names";
const QString WEIGHTS_FILE_DEFAULT_PATH = "./darknet/weights/edb/yolov3-spp_4000.weights";
const QString INPUT_FILE_DEFAULT_PATH = "./res/media/test-video-light.mp4";

// Box color for the object detection boxes.
const cv::Scalar BOX_COLOR = cv::Scalar(60, 160, 260);

/**
 * @brief The YOLO class contains all the methods for detecting the input media by using the YOLO C++ API,
 * it draw boxes on the input and show the results in the Detection Window interface.
 */
class YOLO {

private:
    /**
     * @brief settings_ saves the current cfg, names, weights and input file paths in a configuration file
     * and update it when the values are changed.
     */
    QSettings* settings_ = new QSettings(CONFIG_FILE, QSettings::NativeFormat);
    /**
     * @brief exit_signal_ is only activated when the user has pressed the canceled button in the detection window,
     * it stops the detection process.
     */
    bool exit_signal_ = false;

public:
    /**
     * @brief YOLO default constructor.
     *
     * @param cfgFile
     * @param namesFile
     * @param weightsFile
     * @param filename
     */
    YOLO();

    /**
     * @brief drawBoxes adds a bounding box for every detected object in the frame.
     *
     * @param mat_img
     * @param result_vec
     * @param obj_names
     * @param window
     * @return
     */
    cv::Mat drawBoxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names, DetectionWindow* window);

    /**
     * @brief addDetectedElement adds the detected element to the detection window for visualizing them separated.
     *
     * @param detected_element
     * @param element_class
     * @param window
     */
    void addDetectedElement(cv::Mat detected_element, std::string element_class, DetectionWindow* window);

    /**
     * @brief showResult sends the result to the detection window to display it..
     *
     * @param result_vec
     * @param obj_names
     * @param window
     */
    void showResult(std::vector<bbox_t> const result_vec, std::vector<std::string> const obj_names, DetectionWindow *window);

    /**
     * @brief getObjectNamesFromFile returns a list of strings representing every line
     * in the text file received by parameter.
     *
     * @param filename
     * @return
     */
    std::vector<std::string> getObjectNamesFromFile(const std::string filename);

    /**
     * @brief processInputFile depending on the file received as input,
     * start processing a video or an image file.
     *
     * @param window
     */
    void processInputFile(DetectionWindow *window);

    /**
     * @brief processVideoFile uses the YOLO C++ API to process a video, calculating the frames per second.
     *
     * @param detector
     * @param obj_names
     * @param window
     */
    void processVideoFile(Detector detector, std::vector<std::string> obj_names, DetectionWindow *window);

    /**
     * @brief processImageFile uses the YOLO C++ API to simply process an image.
     *
     * @param detector
     * @param obj_names
     * @param window
     */
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

    /**
     * Updates exit signal to stop yolo.
     * @brief setExit_signal
     * @param value
     */
    void setExitSignal();

    /**
     * @brief hasExitSignal
     */
    bool hasExitSignal();
};

#endif // YOLO_H
#include "detectionwindow.h"
