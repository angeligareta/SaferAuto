#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QStyle>
#include <QTimer>
#include <QString>

#include <iostream>

MainWindow::MainWindow(YOLO yolo, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    yolo(yolo)
{
    QRect screen_geometry = QApplication::desktop() -> availableGeometry();
    double screen_width = screen_geometry.width();
    double screen_height = screen_geometry.height();

    double reduction_proportion = 80.0 / 100.0;
    int reducted_width = screen_width * reduction_proportion;
    int reducted_height = screen_height * reduction_proportion;

    int x_pos = screen_width / 2;
    int y_pos = screen_height / 2;

    this->resize(reducted_width, reducted_height);
    //this->move(x_pos, y_pos);
    this->setWindowTitle("SaferAuto");

    ui->setupUi(this);

    /*Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(display_image()));
    Timer->start();*/

    ui->detectiondisplay->setAlignment(Qt::AlignCenter);
    ui->detectionoutput->setAlignment(Qt::AlignCenter);
    ui->fpsoutput->setAlignment(Qt::AlignCenter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_detection() {
    yolo.process_video(this);
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);
    // TODO Move to button result:
    QTimer::singleShot(50, this, SLOT(start_detection()));
}

void MainWindow::display_image(cv::Mat mat_img) {
    cv::cvtColor(mat_img, mat_img, CV_BGR2RGB);
    QImage imdisplay((uchar*)mat_img.data, mat_img.cols, mat_img.rows, mat_img.step, QImage::Format_RGB888); //Converts the CV image into Qt standard format
    ui->detectiondisplay->setPixmap(QPixmap::fromImage(imdisplay));
    ui->detectiondisplay->update();
    ui->detectiondisplay->repaint();
    //ui->retranslateUi(this);
}

void MainWindow::display_detection(std::string info_text) {
    ui->detectionoutput->setText(QString(info_text.c_str()));
    ui->detectionoutput->update();
    ui->detectionoutput->repaint();
}

void MainWindow::display_fps(std::string fps_text) {
    ui->fpsoutput->setText(QString(fps_text.c_str()));
    ui->fpsoutput->update();
    ui->fpsoutput->repaint();
}
