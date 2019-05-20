#ifndef YOLOCLASSCLASSIFIER_H
#define YOLOCLASSCLASSIFIER_H

// YOLO methods from DLL
#include "yolo_v2_class.hpp"
#include "opencv2/core/version.hpp"
#include "opencv2/videoio/videoio.hpp"
#pragma comment(lib, "opencv_world320.lib")

// Tesseract
#include <baseapi.h>
#include <allheaders.h> // SET LC_NUMERIC=en_US.UTF-8



class YoloClassClassifier
{
private:
    // Detector speed_limit_sign_classifier_;
    std::vector<std::string> speed_limit_sign_labels_;

    std::string extractIntegerWords(std::string str);
    std::string getDigits(cv::Mat image);

public:
    YoloClassClassifier();
    std::string classifyImage(std::string class_name, cv::Mat img);




};

#endif // YOLOCLASSCLASSIFIER_H
