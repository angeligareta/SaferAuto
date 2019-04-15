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
    QImage imdisplay;
    QTimer* Timer;
    void display_detection(std::string info_text);
    void display_fps(std::string fps_text);

signals:
public slots:
    void showEvent(QShowEvent *event);
    void display_image(cv::Mat mat_img);
    void start_detection();

private:
    Ui::DetectionWindow *ui;
    YOLO yolo;
};

#endif // MAINWINDOW_H
