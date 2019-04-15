#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QFileDialog>

#include "yolo.h"
#include "detectionwindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::string getFilePath(std::string current_file, std::string file_extension);

    void centerAndResize();
private slots:
    void on_pushButton_clicked();
    void on_cfgButton_clicked();
    void on_namesButton_clicked();
    void on_weightsButton_clicked();
    void on_mediaButton_clicked();

private:
    Ui::MainWindow *ui;
    YOLO yolo_;
};

#endif // STARTWINDOW_H
