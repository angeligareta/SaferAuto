#ifndef YOLOCLASSCLASSIFIER_H
#define YOLOCLASSCLASSIFIER_H

#include <QDebug>
#include <QProcess>
#include <QObject>
#include <map>
#include <iostream>
#include <fstream>

// Tesseract
//#include <baseapi.h>
//#include <allheaders.h> // SET LC_NUMERIC=en_US.UTF-8

// YOLO methods from DLL
// #include "yolo_v2_class.hpp"
// #include "opencv2/core/version.hpp"
// #include "opencv2/videoio/videoio.hpp"
// #pragma comment(lib, "opencv_world320.lib")
#include <opencv/cv.hpp> // For cv::imread

// Classifier constants
const double SPEED_LIMIT_PRECISSION = 0.70;
const QString SPEED_LIMIT_CLASSIFIER_CFG = "../models/cfg/eresl/darknet.cfg";
const QString SPEED_LIMIT_CLASSIFIER_WEIGHTS= "../models/weights/eresl/darknet_last.weights";
const QString SPEED_LIMIT_CLASSIFIER_DATA = "../models/cfg/eresl/eresl.data";
const std::string SPEED_LIMIT_CLASSIFIER_NAMES = "./models/cfg/eresl/labels.txt";

const QString SPEED_LIMIT_CLASSIFIER_WD = "./darknet"; // Working directory
const QString SPEED_LIMIT_CLASSIFIER_PROGRAM = "python3";
const QStringList SPEED_LIMIT_CLASSIFIER_PROGRAM_ARGS = QStringList{
    "darknet_classify.py",
    SPEED_LIMIT_CLASSIFIER_CFG,
    SPEED_LIMIT_CLASSIFIER_WEIGHTS,
    SPEED_LIMIT_CLASSIFIER_DATA
};

const std::string IMAGES_MODEL_DIR_PATH = "./res/categories_images/";
const std::string IMAGES_MODEL_EXT = ".jpg";


class YoloClassifier : public QObject
{
    Q_OBJECT

private:
    QProcess* speed_limit_sign_classifier;
    std::map<unsigned int, std::string> classified_elements_;

    std::string classifySpeedLimitImage(const std::string &image_class_name, cv::Mat detected_image);
    std::string getModelImageFilePath(const std::string& image_class_name);
    std::string extractIntegerWords(const std::string& str);
    // std::string getDigits(cv::Mat image);

public:
    YoloClassifier();
    YoloClassifier(const YoloClassifier&);
    ~YoloClassifier();
    std::string classifyImage(const std::string& image_class_name, const unsigned int tracking_id, cv::Mat detected_image);
    bool hasElementBeenClassified(unsigned int tracking_id);
    std::string getElementClassification(unsigned int tracking_id);
    cv::Mat getModelImage(const std::string& image_class_name, cv::Mat detected_image);

signals:
public slots:
    void programFinished();
};

#endif // YOLOCLASSCLASSIFIER_H
