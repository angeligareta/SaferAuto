#include "include/detectionwindow.h"
#include "ui_detectionwindow.h"

DetectionWindow::DetectionWindow(YoloDetector yolo, QWidget *parent) :
                                                                       ui_(new Ui::DetectionWindow),
                                                                       yolo_(yolo),
                                                                       detectedElementsLabelList(QList<DetectedElement>()),
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

    DetectedElement detectedElement1 = {ui_->detectedElement1ClassName, ui_->detectedElement1Image, ui_->detectedElement1Probability};
    DetectedElement detectedElement2 = {ui_->detectedElement2ClassName, ui_->detectedElement2Image, ui_->detectedElement2Probability};
    DetectedElement detectedElement3 = {ui_->detectedElement3ClassName, ui_->detectedElement3Image, ui_->detectedElement3Probability};
    DetectedElement detectedElement4 = {ui_->detectedElement4ClassName, ui_->detectedElement4Image, ui_->detectedElement4Probability};
    DetectedElement detectedElement5 = {ui_->detectedElement5ClassName, ui_->detectedElement5Image, ui_->detectedElement5Probability};

    detectedElementsLabelList << detectedElement1;
    detectedElementsLabelList << detectedElement2;
    detectedElementsLabelList << detectedElement3;
    detectedElementsLabelList << detectedElement4;
    detectedElementsLabelList << detectedElement5;
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

void DetectionWindow::displayDetectedElement(cv::Mat mat_img, std::string element_class, unsigned int tracking_id, std::string probability) {
    DetectedElement detectedElement = detectedElementsLabelList.at(lastDetectedIndex);

    detectedElement.image -> setPixmap(getPixmapImage(mat_img));
    detectedElement.image -> update();
    detectedElement.image -> repaint();

    element_class[0] = static_cast<char>(toupper(element_class[0]));
    std::string element_title = std::to_string(tracking_id) + ": " + element_class;
    detectedElement.class_name -> setText(QString(element_title.c_str()));
    detectedElement.class_name -> update();
    detectedElement.class_name -> repaint();

    detectedElement.probability -> setText(QString(probability.c_str()));
    detectedElement.probability -> update();
    detectedElement.probability -> repaint();

    lastDetectedIndex = (lastDetectedIndex + 1) % detectedElementsLabelList.size();
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
