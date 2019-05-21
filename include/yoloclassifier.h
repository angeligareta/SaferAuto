#ifndef YOLOCLASSCLASSIFIER_H
#define YOLOCLASSCLASSIFIER_H

#include <map>

// Tesseract
#include <baseapi.h>
#include <allheaders.h> // SET LC_NUMERIC=en_US.UTF-8

// YOLO methods from DLL
#include "yolo_v2_class.hpp"
#include "opencv2/core/version.hpp"
#include "opencv2/videoio/videoio.hpp"
#include <opencv/cv.hpp> // For cv::imread
#pragma comment(lib, "opencv_world320.lib")

#include <experimental/filesystem> // std::experimental::filesystem::exists

// Classifier constants
const std::string SPEED_LIMIT_CLASSIFIER_NAMES = "./models/cfg/ere/eresl.names";
const std::string SPEED_LIMIT_CLASSIFIER_WEIGHTS= "/models/weights/eresl/darknet_5000.weights";
const std::string SPEED_LIMIT_CLASSIFIER_CFG = "./models/cfg/eresl/darknet.cfg";

const std::string IMAGES_MODEL_DIR_PATH = "./res/categories_images/";
const std::string IMAGES_MODEL_EXT = ".png";


class YoloClassifier
{
private:
    // Detector speed_limit_sign_classifier_;
    std::vector<std::string> speed_limit_sign_labels_;
    std::map<unsigned int, std::string> classified_elements_;

    std::string getModelImageFilePath(const std::string& image_class_name);
    std::string extractIntegerWords(const std::string& str);
    std::string getDigits(cv::Mat image);

public:
    YoloClassifier();
    std::string classifyImage(const std::string& image_class_name, const unsigned int tracking_id, cv::Mat detected_image);
    bool hasElementBeenClassified(unsigned int tracking_id);
    std::string getElementClassification(unsigned int tracking_id);
    cv::Mat getModelImage(const std::string& image_class_name, cv::Mat detected_image);

};

#endif // YOLOCLASSCLASSIFIER_H
