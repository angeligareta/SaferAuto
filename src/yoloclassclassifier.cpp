#include "include/yoloclassclassifier.h"

YoloClassClassifier::YoloClassClassifier():
     // speed_limit_sign_classifier(SPEED_LIMIT_CLASSIFIER_NAMES, SPEED_LIMIT_CLASSIFIER_WEIGHTS),
     speed_limit_sign_labels_(getObjectNamesFromFile(SPEED_LIMIT_CLASSIFIER_NAMES)),
     classified_elements_(){}

std::string YoloClassClassifier::classifyImage(std::string image_class_name, cv::Mat detected_element) {
    if (image_class_name.compare("prohibitory") == 0) {
        std::cout << "Inserting... " << image_class_name;
        //std::string detected_text = getDigits(detected_element);
        //return (detected_text != "") ? ("SL: " + detected_text) : class_name;

        // Classifier method.
        // int speed_limit_class = speed_limit_sign_classifier.classify_image(detected_element, speed_limit_sign_labels.size());
        // return speed_limit_sign_labels[speed_limit_class];
    }

    return image_class_name;
}

bool YoloClassClassifier::hasElementBeenClassified(unsigned int tracking_id) {
    return (tracking_id > 0) && (classified_elements_.count(tracking_id) != 0);
}

std::string YoloClassClassifier::getElementClassification(unsigned int tracking_id) {
    return hasElementBeenClassified(tracking_id) ? classified_elements_.at(tracking_id) : "";
}

// Probé a detectarlo solo y si estaba en las labels devolverlo.
// Probé a coger el nivel de precisión y descartarlo si no pasa un máximo, con todos los PageIteratorLevel distintos.
std::string YoloClassClassifier::getDigits(cv::Mat image) {
    // Create Tesseract object
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();

    // Initialize tesseract to use English (eng) and the LSTM OCR engine.
    ocr->Init("./lib/tesseract-api/tessdata", "eng", tesseract::OEM_DEFAULT);
    ocr->SetVariable("tessedit_char_whitelist", "0123456789");

    // Set image data
    ocr->SetImage(image.data, image.cols, image.rows, 3, image.step);

    ocr->Recognize(0);
    const float kMinConfidence = 50.00;
    std::string detected_text = "";

    // Get confidence from result
    tesseract::ResultIterator* ri = ocr->GetIterator();
    tesseract::PageIteratorLevel level = tesseract::RIL_BLOCK;
    if (ri != 0) {
        std::string word = ri->GetUTF8Text(level);
        float confidence = ri->Confidence(level);
        //std::cout << word << std::endl;

        if ((confidence > kMinConfidence)) { // MAX CONFIDENCE
            detected_text = extractIntegerWords(word);
        }
    }

    return detected_text;
}

std::string YoloClassClassifier::extractIntegerWords(std::string str)
{
    std::string digits = "";
    for (char character : str) {
        if (std::isdigit(character)) {
            digits += character;
        }
    }
    return digits;
}
