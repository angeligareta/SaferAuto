#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <opencv2/opencv.hpp>

namespace Ui {
    class DetectionWindow;
}

#include "yolo.h"

class DetectionWindow : public QDialog
{
    Q_OBJECT

public:
    DetectionWindow(YOLO yolo, QWidget *parent = nullptr);
    ~DetectionWindow();
    void displayDetection(std::string info_text);
    void displayFPS(std::string fps_text);
    void displayDetectedElement(cv::Mat mat_img);

signals:
public slots:
    void showEvent(QShowEvent *event);
    void displayImage(cv::Mat mat_img);
    void startDetection();

private:
    Ui::DetectionWindow *ui;
    YOLO yolo_;
};

#endif // MAINWINDOW_H
