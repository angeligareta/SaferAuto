#ifndef YOLOCLASSCLASSIFIER_H
#define YOLOCLASSCLASSIFIER_H

#include <QString>
#include <QProcess>
#include <QDebug>
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
const std::string SPEED_LIMIT_CLASSIFIER_DATA = "./models/cfg/eresl/eresl.data";
const std::string SPEED_LIMIT_CLASSIFIER_CFG = "./models/cfg/eresl/darknet.cfg";
const std::string SPEED_LIMIT_CLASSIFIER_WEIGHTS= "./models/weights/eresl/darknet_5000.weights";


const QString SPEED_LIMIT_CLASSIFIER_PROGRAM = "./darknet/darknet";
const QStringList SPEED_LIMIT_CLASSIFIER_PROGRAM_ARGS = {
    "classifier",
    "predict",
    QString::fromStdString(SPEED_LIMIT_CLASSIFIER_DATA),
    QString::fromStdString(SPEED_LIMIT_CLASSIFIER_CFG),
    QString::fromStdString(SPEED_LIMIT_CLASSIFIER_WEIGHTS)
};



const std::string IMAGES_MODEL_DIR_PATH = "./res/categories_images/";
const std::string IMAGES_MODEL_EXT = ".jpg";


class YoloClassifier : public QObject
{
    Q_OBJECT

private:

    // Detector speed_limit_sign_classifier_;
    QProcess* speed_limit_sign_classifier_;
    std::map<unsigned int, std::string> classified_elements_;

    std::string classifySpeedLimitImage(const std::string &image_class_name, const cv::Mat& detected_image);
    std::string getModelImageFilePath(const std::string& image_class_name);
    std::string extractIntegerWords(const std::string& str);
    std::string getDigits(cv::Mat image);

public:
    YoloClassifier();
    YoloClassifier(const YoloClassifier& yoloClassifier);

    ~YoloClassifier();
    std::string classifyImage(const std::string& image_class_name, const unsigned int tracking_id, cv::Mat detected_image);
    bool hasElementBeenClassified(unsigned int tracking_id);
    std::string getElementClassification(unsigned int tracking_id);
    cv::Mat getModelImage(const std::string& image_class_name, cv::Mat detected_image);

signals:
public slots:
    void readNewProgramOutput();
    void programFinished();

};

#endif // YOLOCLASSCLASSIFIER_H
