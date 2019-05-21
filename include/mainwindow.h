#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QLabel>
#include <QDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QFileDialog>

#include "yolodetector.h"
#include "detectionwindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_cfgButton_clicked();
    void on_namesButton_clicked();
    void on_weightsButton_clicked();
    void on_mediaButton_clicked();

private:
    Ui::MainWindow *ui;
    YoloDetector yolo_;

    /**
     * @brief setTagText Set a tag text in the main window only if the file exists.
     * @param tag
     * @param text
     */
    void setTagText(QLabel* tag, QString text);
    /**
     * @brief centerAndResizeWindow adjust the window depending on the user screen size.
     */
    void centerAndResizeWindow();
    /**
     * @brief getFilePath Open a file explorer dialog and returns the file choosen by the user.
     * @param current_file
     * @param file_extension
     * @return
     */
    std::string getFilePath(std::string current_file, std::string file_extension);
};

#endif // STARTWINDOW_H
