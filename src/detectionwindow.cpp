#include "include/detectionwindow.h"
#include "ui_detectionwindow.h"

DetectionWindow::DetectionWindow(YoloDetector yolo, QWidget *parent) :
                                                                       ui_(new Ui::DetectionWindow),
                                                                       yolo_(yolo),
                                                                       parent_(parent)
{
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screen_geometry = screen -> geometry();
    int screen_width = screen_geometry.width();
    int screen_height = screen_geometry.height();

    double reduction_proportion = 80.0 / 100.0;
    int reducted_width = static_cast<int>(round(screen_width * reduction_proportion));
    int reducted_height = static_cast<int>(screen_height * reduction_proportion);

    this->resize(reducted_width, reducted_height);

    ui_->setupUi(this);

    ui_->detectiondisplay->setAlignment(Qt::AlignCenter);
    ui_->detectionoutput->setAlignment(Qt::AlignCenter);
    ui_->fpsoutput->setAlignment(Qt::AlignCenter);
    ui_->detectedElements->setAlignment(Qt::AlignCenter);
}

DetectionWindow::~DetectionWindow()
{
    delete ui_;
}

void DetectionWindow::startDetection() {
    yolo_.setDetectionWindow(this);
    yolo_.processInputFile();
}

void DetectionWindow::displayMainImage(cv::Mat mat_img) {
    ui_->detectiondisplay->setPixmap(getPixmapImage(mat_img));
    ui_->detectiondisplay->update();
    ui_->detectiondisplay->repaint();
}

void DetectionWindow::displayDetectedElement(cv::Mat mat_img) {
    ui_->detectedElements->setPixmap(getPixmapImage(mat_img));
    ui_->detectedElements->update();
    ui_->detectedElements->repaint();
}

void DetectionWindow::displayDetectedElementOutput(std::string info_text) {
    ui_->detectionoutput->setText(QString(info_text.c_str()));
    ui_->detectionoutput->update();
    ui_->detectionoutput->repaint();
}

void DetectionWindow::displayFPS(std::string fps_text) {
    ui_->fpsoutput->setText(QString(fps_text.c_str()));
    ui_->fpsoutput->update();
    ui_->fpsoutput->repaint();
}

QPixmap DetectionWindow::getPixmapImage(cv::Mat mat_img) {
    //Converts from BG 2 RGB color format
    cv::cvtColor(mat_img, mat_img, CV_BGR2RGB);

    //Converts the CV image into Qt standard format
    QImage img_display(static_cast<uchar*>(mat_img.data), mat_img.cols, mat_img.rows, static_cast<int>(mat_img.step), QImage::Format_RGB888);
    return QPixmap::fromImage(img_display);
}

void DetectionWindow::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    // Timer compulsory for starting detection
    QTimer::singleShot(50, this, SLOT(startDetection()));
}

void DetectionWindow::closeEvent(QCloseEvent *event)
{
    std::cout << "Thanks for using the SaferAuto detector." << std::endl;

    yolo_.setExitSignal();
    parent_->show();
    event->accept();
}
