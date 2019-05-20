#ifndef YOLOCLASSCLASSIFIER_H
#define YOLOCLASSCLASSIFIER_H

#include <map>

// YOLO methods from DLL
#include "yolo_v2_class.hpp"
#include "opencv2/core/version.hpp"
#include "opencv2/videoio/videoio.hpp"
#pragma comment(lib, "opencv_world320.lib")

// Tesseract
#include <baseapi.h>
#include <allheaders.h> // SET LC_NUMERIC=en_US.UTF-8

// Classifier constants
const std::string SPEED_LIMIT_CLASSIFIER_NAMES = "./models/cfg/ere/eresl.names";
const std::string SPEED_LIMIT_CLASSIFIER_WEIGHTS= "/models/weights/eresl/darknet_5000.weights";
const std::string SPEED_LIMIT_CLASSIFIER_CFG = "./models/cfg/eresl/darknet.cfg";


class YoloClassClassifier
{
private:
    // Detector speed_limit_sign_classifier_;
    std::vector<std::string> speed_limit_sign_labels_;
    std::map<unsigned int, std::string> classified_elements_;

    std::string extractIntegerWords(std::string str);
    std::string getDigits(cv::Mat image);

public:
    YoloClassClassifier();
    std::string classifyImage(std::string image_class_name, cv::Mat img);
    bool hasElementBeenClassified(unsigned int tracking_id);
    std::string getElementClassification(unsigned int tracking_id);




};

#endif // YOLOCLASSCLASSIFIER_H
