#include "include/yolodetector.h"

YoloDetector::YoloDetector():
                               yolo_class_classifier_()
{}


YoloDetector::~YoloDetector() {
    delete yolo_detector_;
    delete detection_window_;
}

cv::Mat YoloDetector::drawBoxes(cv::Mat mat_img, const std::vector<bbox_t>& results, const std::vector<std::string>& element_names) {
    for (auto &i : results) {
        cv::Rect detection_roi = cv::Rect(static_cast<int>(i.x), static_cast<int>(i.y), static_cast<int>(i.w), static_cast<int>(i.h));

        if (yolo_class_classifier_.hasElementBeenClassified(i.track_id)) {
            // Add bounding box of object to main image.
            cv::rectangle(mat_img, detection_roi, BOX_COLOR, 3);
        }
        else {
            cv::Mat detected_element = mat_img(detection_roi);
            cv::resize(detected_element, detected_element, cv::Size(150, 150)); // Zoom detected sign

            std::string element_class = yolo_class_classifier_.classifyImage(element_names[i.obj_id], i.track_id, detected_element);

            // Show results
            double normalized_probability = round(static_cast<double>(i.prob) * 1000.0)/10.0;
            showResult(mat_img, detection_roi, detected_element, element_class, i.track_id, normalized_probability);
        }
    }

    return mat_img;
}

void YoloDetector::showResult(cv::Mat mat_img, cv::Rect detection_roi, cv::Mat detected_element,
                              const std::string element_class, unsigned int tracking_id, const double normalized_probability) {
    std::cout <<  "Last TS detected: " + element_class + ". Probability: " + std::to_string(normalized_probability) + "%" << std::endl;

    // Display model image
    cv::Mat class_model_image = yolo_class_classifier_.getModelImage(element_class, detected_element);
    cv::resize(class_model_image, class_model_image, cv::Size(150, 150)); // Zoom detected sign
    detection_window_ -> displayDetectedElement(class_model_image, element_class, tracking_id, std::to_string(normalized_probability));

    // Add bounding box of object to main image.
    cv::rectangle(mat_img, detection_roi, BOX_COLOR, 3);
}

void YoloDetector::processVideoFile(const std::vector<std::string>& element_names)
{
    cv::Mat frame;
    yolo_detector_->nms = NMS_THRESHOLD; // Set nont-maximum supression threshold for track_id

    unsigned int frame_counter = 1;
    cv::VideoCapture cap(getInputFile());
    while(!hasExitSignal() && cap.isOpened()) {
        // Read new capture
        cap >> frame;

        startTimer();
        // Detect and track
        std::vector<bbox_t> results = yolo_detector_->detect(frame, DETECTION_THRESHOLD);
        results = yolo_detector_->tracking_id(results);

        // Draw boxes and display image.
        try {
            drawBoxes(frame, results, element_names);
            detection_window_ -> displayMainImage(frame);
        } catch (...) {}
        stopTimer();
        detection_window_ -> displayFPS("AVERAGE FPS: " + std::to_string(getCurrentFPS()) + " fps");

        // For not blocking UI
        QCoreApplication::processEvents();
        frame_counter ++;
    }

    cap.release();
}

void YoloDetector::processImageFile(const std::vector<std::string>& element_names) {
    cv::Mat mat_img = cv::imread(getInputFile());
    std::vector<bbox_t> results = yolo_detector_->detect(mat_img, DETECTION_THRESHOLD);

    drawBoxes(mat_img, results, element_names);
    detection_window_ -> displayMainImage(mat_img);

    // Give time to see the image before closing.
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}

void YoloDetector::processInputFile()
{
    if (detection_window_ == nullptr) {
        std::cerr << "Detection window not defined." << std::endl;
    }
    else {
        yolo_detector_ = new Detector(getCfgFile(), getWeightsFile());
        std::vector<std::string> element_names = getObjectNamesFromFile(getNamesFile());

        try {
            const std::string kFileExt = getInputFile().substr(getInputFile().find_last_of(".") + 1);
            if (kFileExt == "avi" || kFileExt == "mp4" || kFileExt == "mjpg" || kFileExt == "mov") {	// Video file
                processVideoFile(element_names);
            }
            else {	// Image file
                processImageFile(element_names);
            }            
        }
        catch (std::exception &e) {
            std::cerr << "exception: " << e.what() << "\n";;
        }
        catch (...) {
            std::cerr << "unknown exception \n";
        }
    }

    detection_window_->close();
}

unsigned int YoloDetector::getCurrentFPS()
{
    double elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end_ - begin_).count();
    return static_cast<unsigned int>(1000.0 / elapsed_milliseconds);
}


void YoloDetector::startTimer() {
    begin_ = std::chrono::steady_clock::now();
}

void YoloDetector::stopTimer(){
    end_ = std::chrono::steady_clock::now();
}

void YoloDetector::setCfgFile(const std::string &value)
{
    settings_->setValue(CFG_FILE, QString::fromStdString(value));
}

void YoloDetector::setNamesFile(const std::string &value)
{
    settings_->setValue(NAMES_FILE, QString::fromStdString(value));
}

void YoloDetector::setWeightsFile(const std::string &value)
{
    settings_->setValue(WEIGHTS_FILE, QString::fromStdString(value));
}

void YoloDetector::setInputFile(const std::string &value)
{
    settings_->setValue(INPUT_FILE, QString::fromStdString(value));
}

void YoloDetector::setDetectionWindow(DetectionWindow* detection_window) {
    detection_window_ = detection_window;
}

void YoloDetector::setExitSignal()
{
    exit_signal_ = true;
}

std::string YoloDetector::getCfgFile() const
{
    return settings_->value(CFG_FILE, CFG_FILE_DEFAULT_PATH).toString().toStdString();
}

std::string YoloDetector::getNamesFile() const
{
    return settings_->value(NAMES_FILE, NAMES_FILE_DEFAULT_PATH).toString().toStdString();
}

std::string YoloDetector::getWeightsFile() const
{
    return settings_->value(WEIGHTS_FILE, WEIGHTS_FILE_DEFAULT_PATH ).toString().toStdString();
}

std::string YoloDetector::getInputFile() const
{
    return settings_->value(INPUT_FILE, INPUT_FILE_DEFAULT_PATH).toString().toStdString();
}

bool YoloDetector::hasExitSignal() const
{
    return exit_signal_;
}
