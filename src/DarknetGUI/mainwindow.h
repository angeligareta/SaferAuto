#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

namespace Ui {
    class MainWindow;
}

#include "yolo.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(YOLO yolo, QWidget *parent = nullptr);
    ~MainWindow();
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
        Ui::MainWindow *ui;
        YOLO yolo;
};

#endif // MAINWINDOW_H
