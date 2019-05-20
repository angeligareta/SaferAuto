#include "include/detectionwindow.h"
#include "ui_detectionwindow.h"

#include <QDesktopWidget>
#include <QStyle>
#include <QTimer>
#include <QString>

#include <iostream>

DetectionWindow::DetectionWindow(YOLO yolo, QWidget *parent) :
                                                               ui(new Ui::DetectionWindow),
                                                               yolo_(yolo),
                                                               parent_(parent)
{
    QRect screen_geometry = QApplication::desktop() -> availableGeometry();
    int screen_width = screen_geometry.width();
    int screen_height = screen_geometry.height();

    double reduction_proportion = 80.0 / 100.0;
    int reducted_width = round(screen_width * reduction_proportion);
    int reducted_height = screen_height * reduction_proportion;

    this->resize(reducted_width, reducted_height);
    //this->move(x_pos, y_pos);

    ui->setupUi(this);

    ui->detectiondisplay->setAlignment(Qt::AlignCenter);
    ui->detectionoutput->setAlignment(Qt::AlignCenter);
    ui->fpsoutput->setAlignment(Qt::AlignCenter);
    ui->detectedElements->setAlignment(Qt::AlignCenter);
}

DetectionWindow::~DetectionWindow()
{
    delete ui;
}

void DetectionWindow::startDetection() {
    yolo_.setDetectionWindow(this);
    yolo_.processInputFile();
}

void DetectionWindow::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    QTimer::singleShot(50, this, SLOT(startDetection())); // Timer compulsory
}

void DetectionWindow::displayDetectedElement(cv::Mat mat_img) {
    cv::cvtColor(mat_img, mat_img, CV_BGR2RGB);
    QImage img_display((uchar*) mat_img.data, mat_img.cols, mat_img.rows, mat_img.step, QImage::Format_RGB888);

    ui->detectedElements->setPixmap(QPixmap::fromImage(img_display));
    ui->detectedElements->update();
    ui->detectedElements->repaint();
}

void DetectionWindow::displayImage(cv::Mat mat_img) {
    cv::cvtColor(mat_img, mat_img, CV_BGR2RGB);
    QImage img_display((uchar*)mat_img.data, mat_img.cols, mat_img.rows, mat_img.step, QImage::Format_RGB888); //Converts the CV image into Qt standard format

    ui->detectiondisplay->setPixmap(QPixmap::fromImage(img_display));
    ui->detectiondisplay->update();
    ui->detectiondisplay->repaint();
}

void DetectionWindow::displayDetection(std::string info_text) {
    ui->detectionoutput->setText(QString(info_text.c_str()));
    ui->detectionoutput->update();
    ui->detectionoutput->repaint();
}

void DetectionWindow::displayFPS(std::string fps_text) {
    ui->fpsoutput->setText(QString(fps_text.c_str()));
    ui->fpsoutput->update();
    ui->fpsoutput->repaint();
}

void DetectionWindow::closeEvent(QCloseEvent *event)
{
    std::cout << "Thanks for using the SaferAuto detector." << std::endl;

    yolo_.setExitSignal();
    parent_->show();
    event->accept();
}
