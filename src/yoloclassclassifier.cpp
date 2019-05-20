#include "include/yoloclassclassifier.h"

YoloClassClassifier::YoloClassClassifier():
    //speed_limit_sign_classifier("/home/angeliton/Desktop/SaferAuto/models/cfg/eresl/darknet.cfg", "/home/angeliton/Desktop/SaferAuto/models/weights/eresl/darknet_5000.weights"),
    speed_limit_sign_labels_(getObjectNamesFromFile("./models/cfg/ere/eresl.names"))
{}

std::string YoloClassClassifier::classifyImage(std::string class_name, cv::Mat detected_element) {
    if (class_name.compare("prohibitory") == 0) {
        //std::string detected_text = getDigits(detected_element);
        //return (detected_text != "") ? ("SL: " + detected_text) : class_name;

        // Classifier method.
        // int speed_limit_class = speed_limit_sign_classifier.classify_image(detected_element, speed_limit_sign_labels.size());
        // return speed_limit_sign_labels[speed_limit_class];
    }

    return class_name;
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
