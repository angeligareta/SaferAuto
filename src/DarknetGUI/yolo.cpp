#include "yolo.h"

YOLO::YOLO()
{
    this->names_file_ = "../../darknet/cfg/general/general.names";
    this->cfg_file_ = "../../darknet/cfg/general/yolov3-spp.cfg";
    this->weights_file_ = "../../darknet/weights/general/yolov3-spp_4000.weights";
    this->input_file_ = "../../darknet/test-video-light.mp4";
}

YOLO::YOLO(std::string cfg_file_, std::string names_file_, std::string weights_file_, std::string filename)
{
    this->cfg_file_ = cfg_file_;
    this->names_file_ = names_file_;
    this->weights_file_ = weights_file_;
    this->input_file_ = filename;
}

cv::Mat YOLO::drawBoxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names) {
    for (auto &i : result_vec) {
        cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), BOX_COLOR, 3);
        if(i.obj_id < obj_names.size())
            putText(mat_img, obj_names[i.obj_id], cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, BOX_COLOR);
        if(i.track_id > 0)
            putText(mat_img, std::to_string(i.track_id), cv::Point2f(i.x + 5, i.y + 15), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, BOX_COLOR);
    }

    return mat_img;
}

void YOLO::showResult(std::vector<bbox_t> const result_vec, std::vector<std::string> const obj_names, DetectionWindow* window) {
    for (auto &i : result_vec) {
        if (obj_names.size() > i.obj_id) {
            std::cout << obj_names[i.obj_id] << " - ";
        }
        std::string info_text = "Last TS detected: " + obj_names[i.obj_id] + ". Probability: " + std::to_string(i.prob) + "%";
        window->display_detection(info_text);

    }
}

std::vector<std::string> YOLO::getObjectNamesFromFile(std::string const filename) {
    std::ifstream file(filename);
    std::vector<std::string> file_lines;

    if (!file.is_open()) {
        return file_lines;
    }

    // Read lines from object names file.
    for(std::string line; file >> line;) {
        file_lines.push_back(line);
    }
    std::cout << "Object names loaded \n";
    return file_lines;
}


void YOLO::processVideoFile(Detector detector, std::vector<std::string> obj_names, DetectionWindow* window)
{
    cv::Mat frame;
    detector.nms = 0.02f;

    preview_boxes_t large_preview(100, 150, false);
    const float kDetectionThreshold = 0.2f;

    for(cv::VideoCapture cap(input_file_); cap >> frame, cap.isOpened();) {
        auto begin = std::chrono::steady_clock::now();
        std::vector<bbox_t> result_vec = detector.detect(frame, kDetectionThreshold);
        result_vec = detector.tracking_id(result_vec);
        auto end = std::chrono::steady_clock::now();

        double elapsed_secs = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        int fps = static_cast<int>(1000.0 / elapsed_secs);
        std::string fps_text = "CURRENT FPS: " + std::to_string(fps) + " fps";
        window -> display_fps(fps_text);

        cv::Mat imdisplay = drawBoxes(frame, result_vec, obj_names);
        window -> display_image(imdisplay);
        window -> display_fps(fps_text);

        large_preview.set(frame, result_vec);
        showResult(result_vec, obj_names, window);
    }
}

void YOLO::processImageFile(Detector detector, std::vector<std::string> obj_names, DetectionWindow* window) {
    cv::Mat mat_img = cv::imread(input_file_);
    std::vector<bbox_t> result_vec = detector.detect(mat_img);
    drawBoxes(mat_img, result_vec, obj_names);
    showResult(result_vec, obj_names, window);
}

void YOLO::processInputFile(DetectionWindow* window)
{

    Detector detector(cfg_file_, weights_file_);
    auto obj_names = getObjectNamesFromFile(names_file_);

    try {
        const std::string kFileExt = input_file_.substr(input_file_.find_last_of(".") + 1);
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
    cfg_file_ = value;
}

void YOLO::setNamesFile(const std::string &value)
{
    names_file_ = value;
}

void YOLO::setWeightsFile(const std::string &value)
{
    weights_file_ = value;
}

void YOLO::setInputFile(const std::string &value)
{
    input_file_ = value;
}

std::string YOLO::getCfgFile() const
{
    return cfg_file_;
}

std::string YOLO::getNamesFile() const
{
    return names_file_;
}

std::string YOLO::getWeightsFile() const
{
    return weights_file_;
}

std::string YOLO::getInputFile() const
{
    return input_file_;
}
