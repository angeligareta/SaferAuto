#include "include/yoloclassifier.h"

YoloClassifier::YoloClassifier():
                                   speed_limit_sign_classifier(new QProcess),
                                   classified_elements_()
{
    connect(speed_limit_sign_classifier, SIGNAL(finished(int , QProcess::ExitStatus )), this, SLOT(programFinished()));

    speed_limit_sign_classifier -> setWorkingDirectory(SPEED_LIMIT_CLASSIFIER_WD);
    speed_limit_sign_classifier -> start(SPEED_LIMIT_CLASSIFIER_PROGRAM, SPEED_LIMIT_CLASSIFIER_PROGRAM_ARGS);

    if (speed_limit_sign_classifier -> waitForStarted()) {
        std::cout << "YOLO CLASSIFIER PROGRAM started!" << std::endl;
        if (speed_limit_sign_classifier -> waitForReadyRead()) {
            qDebug() << speed_limit_sign_classifier -> readAllStandardOutput();
        }
    }
    else {
        std::cout << "YOLO CLASSIFIER PROGRAM did not start" << std::endl;
    }
}

YoloClassifier::YoloClassifier(const YoloClassifier& yoloClassifier) {
    this->classified_elements_ = yoloClassifier.classified_elements_;
    this->speed_limit_sign_classifier = yoloClassifier.speed_limit_sign_classifier;
}

YoloClassifier::~YoloClassifier() {

}

void YoloClassifier::programFinished() {
    qDebug() << "YOLO CLASSIFIER PROGRAM FINISHED. Last output:" << speed_limit_sign_classifier->readAllStandardError();
}

std::string YoloClassifier::classifySpeedLimitImage(const std::string &image_class_name, cv::Mat detected_image) {
    qDebug() << speed_limit_sign_classifier->readAll();

    std::string filename = image_class_name + ".ppm";
    cv::imwrite(filename, detected_image);

    speed_limit_sign_classifier->write(QString::fromStdString("../" + filename + "\n").toLatin1());
    if (speed_limit_sign_classifier->waitForReadyRead()) {
        QString output(speed_limit_sign_classifier->readAllStandardOutput());
        QString detectedSignalRaw = output.split("\n")[0];
        qDebug() << output;

        if (detectedSignalRaw.contains("-")) {
            QStringList detectedSignal = detectedSignalRaw.split("- ");
            std::string detected_class_name = detectedSignal[0].toStdString();
            double probability = detectedSignal[1].toDouble();

            if (probability > 0.85) {
                std::cout << "Detected:" << detected_class_name << " | Prob: " << probability;
                return detected_class_name;
            }
        }
    }

    return image_class_name;
}

std::string YoloClassifier::classifyImage(const std::string& image_class_name, const unsigned int tracking_id, cv::Mat detected_image) {

    // Disabled for speed
    /*if (image_class_name.compare("prohibitory") == 0) {
        std::string detected_text = getDigits(detected_image);

        // If detection succesfuull, clasify element
        if (detected_text == "") {
            std::cout << "Inserting... " << image_class_name;
            classified_elements_.insert(std::pair<unsigned int, std::string>(tracking_id, image_class_name));
            return  ("SL: " + detected_text);
        }
    }*/
    if (image_class_name == "prohibitory") {
        std::string detected_class_name = classifySpeedLimitImage(image_class_name, detected_image);

        if (detected_class_name != image_class_name) {
            if (tracking_id > 0) {
                classified_elements_.insert(std::pair<unsigned int, std::string>(tracking_id, detected_class_name));
            }
            return detected_class_name;
        }
    }

    return image_class_name;
}

cv::Mat YoloClassifier::getModelImage(const std::string& image_class_name, cv::Mat detected_image) {
    std::string model_image_file_path = getModelImageFilePath(image_class_name);

    // If model file exists return it
    if (std::ifstream(model_image_file_path).good()) {
        return cv::imread(model_image_file_path);
    }
    else {
        std::cerr << "File " + model_image_file_path + " does not exist.";
        return detected_image;
    }
}

std::string YoloClassifier::getModelImageFilePath(const std::string& image_class_name) {
    return IMAGES_MODEL_DIR_PATH + image_class_name + IMAGES_MODEL_EXT;
}

bool YoloClassifier::hasElementBeenClassified(unsigned int tracking_id) {
    return (tracking_id > 0) && (classified_elements_.count(tracking_id) != 0);
}

std::string YoloClassifier::getElementClassification(unsigned int tracking_id) {
    return hasElementBeenClassified(tracking_id) ? classified_elements_.at(tracking_id) : "";
}

// Probé a detectarlo solo y si estaba en las labels devolverlo.
// Probé a coger el nivel de precisión y descartarlo si no pasa un máximo, con todos los PageIteratorLevel distintos.
/*std::string YoloClassifier::getDigits(cv::Mat image) {
    // Create Tesseract object
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();

    // Initialize tesseract to use English (eng) and the LSTM OCR engine.
    ocr->Init("./lib/tesseract-api/tessdata", "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetVariable("tessedit_char_whitelist", "0123456789");

    // Set image data
    ocr->SetImage(image.data, image.cols, image.rows, 3, static_cast<int>(image.step));

    ocr->Recognize(nullptr);
    const float kMinConfidence = 50.00;
    std::string detected_text = "";

    // Get confidence from result
    tesseract::ResultIterator* ri = ocr->GetIterator();
    tesseract::PageIteratorLevel level = tesseract::RIL_BLOCK;
    if (ri != nullptr) {
        std::string word = ri->GetUTF8Text(level);
        float confidence = ri->Confidence(level);
        //std::cout << word << std::endl;

        if ((confidence > kMinConfidence)) { // MAX CONFIDENCE
            detected_text = extractIntegerWords(word);
        }
    }

    return detected_text;
}*/

std::string YoloClassifier::extractIntegerWords(const std::string& str)
{
    std::string digits = "";
    for (char character : str) {
        if (std::isdigit(character)) {
            digits += character;
        }
    }
    return digits;
}
