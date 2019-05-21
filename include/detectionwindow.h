#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include <QScreen>
#include <QTimer> // For singleshot event

#include <iostream>

// OpenCV for cv::Mat
#include <opencv2/opencv.hpp>

// Important to first define the name of the class before importing yolodetector for avoiding cycle dependency
namespace Ui {
    class DetectionWindow;
}

#include "yolodetector.h"

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
    QWidget* parent_;

public:
    DetectionWindow(YoloDetector yolo, QWidget *parent = nullptr);
    ~DetectionWindow();

    void displayMainImage(cv::Mat mat_img);
    void displayDetectedElement(cv::Mat mat_img);
    void displayDetectedElementOutput(std::string info_text);
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
