#include "include/yolo.h"

YOLO::YOLO():
               tracked_elements(),
               yolo_class_classifier_(){}

cv::Mat YOLO::drawBoxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names, DetectionWindow* window) {
    for (auto &i : result_vec) {
        cv::Rect detection_roi = cv::Rect(i.x, i.y, i.w, i.h);
        cv::Mat detected_element = mat_img(detection_roi);

        std::string element_class;
        // If the element has been tracked, do not classify again and return the classified class.
        if (tracked_elements.count(i.track_id) != 0) {
            element_class = tracked_elements.at(i.track_id);
        }
        else { // Classify new tracked element
            element_class = yolo_class_classifier_.classifyImage(obj_names[i.obj_id], detected_element);
            if (element_class != obj_names[i.obj_id]) {
                std::cout << "Inserting... " << element_class;
                tracked_elements.insert(std::pair<int, std::string>(i.track_id, element_class));
            }
        }

        std::cout << element_class << std::endl;

        addDetectedElement(detected_element, element_class, window);
        cv::rectangle(mat_img, detection_roi, BOX_COLOR, 3); // Add bounding box of object to img

        // if(i.obj_id < obj_names.size())
            // putText(mat_img, element_class, cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, BOX_COLOR);
        // if(i.track_id > 0)
            // putText(mat_img, std::to_string(i.track_id), cv::Point2f(i.x + 5, i.y + 15), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, BOX_COLOR);
    }

    return mat_img;
}

void YOLO::addDetectedElement(cv::Mat detected_element, std::string element_class, DetectionWindow* window) {
    cv::resize(detected_element, detected_element, cv::Size(120, 120)); // Zoom detected sign
    putText(detected_element, element_class, cv::Point(25, 110), cv::FONT_HERSHEY_DUPLEX, 1, BOX_COLOR);
    window -> displayDetectedElement(detected_element);
}

void YOLO::showResult(std::vector<bbox_t> const result_vec, std::vector<std::string> const obj_names, DetectionWindow* window) {
    for (auto &i : result_vec) {
        if (obj_names.size() > i.obj_id) {
            //std::cout << obj_names[i.obj_id] << " - ";
        }
        double normalized_prob = round(i.prob * 1000.0)/10.0;
        std::string info_text = "Last TS detected: " + obj_names[i.obj_id] + ". Probability: " + std::to_string(normalized_prob) + "%";

        window->displayDetection(info_text);
    }
}

void YOLO::processVideoFile(Detector detector, std::vector<std::string> obj_names, DetectionWindow* window)
{
    cv::Mat frame;
    detector.nms = 0.02f;
    const float kDetectionThreshold = 0.5f;

    cv::VideoCapture cap(getInputFile());
    while(!hasExitSignal() && cap.isOpened()) {
        cap >> frame; // Read new capture

        auto begin = std::chrono::steady_clock::now();
        std::vector<bbox_t> result_vec = detector.detect(frame, kDetectionThreshold);
        result_vec = detector.tracking_id(result_vec);

        // preview_boxes_t large_preview(100, 150, false);
        // large_preview.set(frame, result_vec);
        // large_preview.draw(frame, true);

        try {
            drawBoxes(frame, result_vec, obj_names, window);
        } catch (...) {
            // TODO: Correct
        }
        window -> displayImage(frame);
        showResult(result_vec, obj_names, window);

        auto end = std::chrono::steady_clock::now();
        double elapsed_secs = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        int fps = static_cast<int>(1000.0 / elapsed_secs);
        std::string fps_text = "CURRENT FPS: " + std::to_string(fps) + " fps";
        window -> displayFPS(fps_text);

        // For not blocking UI
        QCoreApplication::processEvents();
    }

    cap.release();
}

void YOLO::processImageFile(Detector detector, std::vector<std::string> obj_names, DetectionWindow* window) {
    cv::Mat mat_img = cv::imread(getInputFile());
    std::vector<bbox_t> result_vec = detector.detect(mat_img);

    drawBoxes(mat_img, result_vec, obj_names, window);
    showResult(result_vec, obj_names, window);
    window -> displayImage(mat_img);

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}

void YOLO::processInputFile(DetectionWindow* window)
{
    Detector detector(getCfgFile(), getWeightsFile());
    auto obj_names = getObjectNamesFromFile(getNamesFile());

    try {
        const std::string kFileExt = getInputFile().substr(getInputFile().find_last_of(".") + 1);
        if (kFileExt == "avi" || kFileExt == "mp4" || kFileExt == "mjpg" || kFileExt == "mov") {	// Video file
            processVideoFile(detector, obj_names, window);
        }
        else {	// Image file
            processImageFile(detector, obj_names, window);
        }
    }
    catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << "\n";;
    }
    catch (...) {
        std::cerr << "unknown exception \n";
    }
}

void YOLO::setCfgFile(const std::string &value)
{
    settings_->setValue(CFG_FILE, QString::fromStdString(value));
}

void YOLO::setNamesFile(const std::string &value)
{
    settings_->setValue(NAMES_FILE, QString::fromStdString(value));
}

void YOLO::setWeightsFile(const std::string &value)
{
    settings_->setValue(WEIGHTS_FILE, QString::fromStdString(value));
}

void YOLO::setInputFile(const std::string &value)
{
    settings_->setValue(INPUT_FILE, QString::fromStdString(value));
}

std::string YOLO::getCfgFile() const
{
    return settings_->value(CFG_FILE, CFG_FILE_DEFAULT_PATH).toString().toStdString();
}

std::string YOLO::getNamesFile() const
{
    return settings_->value(NAMES_FILE, NAMES_FILE_DEFAULT_PATH).toString().toStdString();
}

std::string YOLO::getWeightsFile() const
{
    return settings_->value(WEIGHTS_FILE, WEIGHTS_FILE_DEFAULT_PATH ).toString().toStdString();
}

std::string YOLO::getInputFile() const
{
    return settings_->value(INPUT_FILE, INPUT_FILE_DEFAULT_PATH).toString().toStdString();
}

void YOLO::setExitSignal()
{
    exit_signal_ = true;
}

bool YOLO::hasExitSignal()
{
    return exit_signal_;
}
