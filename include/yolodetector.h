#ifndef YOLO_H
#define YOLO_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <map>
#include <QSettings>
#include <QString>
#include <QCoreApplication>

#define OPENCV
// #define TRACK_OPTFLOW
#define GPU

// YOLO methods from DLL
#include "yolo_v2_class.hpp"
#include "opencv2/core/version.hpp"
#include "opencv2/videoio/videoio.hpp"
#pragma comment(lib, "opencv_world320.lib")

#include "yoloclassifier.h"

class DetectionWindow;

// CONFIG INI TAGS FOR FILES USED IN SAFER AUTO
const QString CFG_FILE = "cfg_file";
const QString NAMES_FILE = "names_file";
const QString WEIGHTS_FILE = "weights_file";
const QString INPUT_FILE = "input_file";
const QString CONFIG_FILE = "config.ini";

// DEFAULT PATHS FOR FILES USED IN SAFER AUTO
const QString CFG_FILE_DEFAULT_PATH = "./models/cfg/edb/yolov3-tiny.cfg";
const QString NAMES_FILE_DEFAULT_PATH = "./models/cfg/edb/edb.names";
const QString WEIGHTS_FILE_DEFAULT_PATH = "./models/weights/edb-tiny/yolov3-tiny_37000.weights";
const QString INPUT_FILE_DEFAULT_PATH = "./res/media/test-video-light.mp4";

// Box color for the object detection boxes.
const cv::Scalar BOX_COLOR = cv::Scalar(65, 105, 225);
const float DETECTION_THRESHOLD = 0.6f;
const float NMS_THRESHOLD = 0.02f;

/**
 * @brief The YOLO class contains all the methods for detecting the input media by using the YOLO C++ API,
 * it draw boxes on the input and show the results in the Detection Window interface.
 */
class YoloDetector {

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

    /**
     * @brief yolo_detector_ represents the YOLO detector.
     */
    Detector* yolo_detector_ = nullptr;

    /**
     * @brief yolo_class_classifier_ object contains methods for classifying an object subclass.
     */
    YoloClassifier yolo_class_classifier_;

    /**
     * @brief detection_window_ represents the view where all the result will be displayed.
     */
    DetectionWindow* detection_window_ = nullptr;

    /**
     * @brief begin_ stores the start of the timer.
     */
    std::chrono::_V2::steady_clock::time_point begin_;

    /**
     * @brief end_ stores the end of the timer.
     */
    std::chrono::_V2::steady_clock::time_point end_;

public:
    /**
     * @brief YOLO default constructor.
     *
     * @param cfgFile
     * @param namesFile
     * @param weightsFile
     * @param filename
     */
    YoloDetector();

    ~YoloDetector();

    /**
     * @brief drawBoxes adds a bounding box for every detected object in the frame.
     *
     * @param mat_img
     * @param results
     * @param element_names
     * @param window
     * @return
     */
    cv::Mat drawBoxes(cv::Mat mat_img, const std::vector<bbox_t>& results, const std::vector<std::string>& element_names);

    /**
     * @brief addDetectedElement adds the detected element to the detection window for visualizing them separated.
     *
     * @param detected_element
     * @param element_class
     * @param window
     */
    void displayDetectedElement(cv::Mat detected_element, const std::string element_class);

    /**
     * @brief showResult sends the result to the detection window to display it..
     *
     * @param results
     * @param element_names
     * @param window
     */
    void showResult(cv::Mat mat_img, cv::Rect detection_roi, cv::Mat detected_element,
                    const std::string element_class, unsigned int tracking_id, const double normalized_probability);

    /**
     * @brief processInputFile depending on the file received as input,
     * start processing a video or an image file.
     *
     * @param window
     */
    void processInputFile();

    /**
     * @brief processVideoFile uses the YOLO C++ API to process a video, calculating the frames per second.
     *
     * @param detector
     * @param element_names
     * @param window
     */
    void processVideoFile(const std::vector<std::string>& element_names);

    /**
     * @brief processImageFile uses the YOLO C++ API to simply process an image.
     *
     * @param detector
     * @param element_names
     * @param window
     */
    void processImageFile(const std::vector<std::string>& element_names);

    /**
     * @brief getCurrentFPS
     * @param frame_counter
     * @return
     */
    unsigned int getCurrentFPS();

    /**
     * @brief startTimer
     */
    void startTimer();

    /**
     * @brief stopTimer
     */
    void stopTimer();

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
     * @brief setDetectionWindow
     * @param detection_window
     */
    void setDetectionWindow(DetectionWindow* detection_window);

    /**
     * @brief hasExitSignal
     */
    bool hasExitSignal() const;
 };

#endif // YOLO_H
#include "detectionwindow.h"
