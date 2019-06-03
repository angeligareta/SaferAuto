#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QLabel>
#include <QDialog>
#include <QCloseEvent>
#include <QScreen>
#include <QTimer> // For singleshot event

#include <iostream>
#include <cctype> // toupper

// OpenCV for cv::Mat
#include <opencv2/opencv.hpp>

// Important to first define the name of the class before importing yolodetector for avoiding cycle dependency
namespace Ui {
    class DetectionWindow;
}

#include "yolodetector.h"

struct DetectedElement {
    QLabel* class_name;
    QLabel* image;
    QLabel* probability;
    unsigned int tracking_id;
};

/**
 * @brief The DetectionWindow class displays a view for the detection model. It includes methods
 * to display the image
 */
class DetectionWindow : public QDialog
{
    Q_OBJECT

private:
    Ui::DetectionWindow* ui_;
    YoloDetector yolo_;
    QList<DetectedElement> detectedElementsLabelList;
    QWidget* parent_;

    int lastDetectedIndex = 0;

public:
    DetectionWindow(YoloDetector yolo, QWidget *parent = nullptr);
    ~DetectionWindow();

    void displayMainImage(cv::Mat mat_img);
    void displayDetectedElement(cv::Mat mat_img, std::string element_class, unsigned int tracking_id, std::string probability);
    void displayFPS(std::string fps_text);

    QPixmap getPixmapImage(cv::Mat mat_img);

signals:
public slots:
    void showEvent(QShowEvent *event);
    void startDetection();

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
