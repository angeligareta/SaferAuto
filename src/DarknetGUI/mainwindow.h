#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include <yolo.h>
#include <detectionwindow.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::string getFilePath(std::string current_file, std::string file_extension);

private slots:
    void on_pushButton_clicked();
    void on_cfgButton_clicked();
    void on_namesButton_clicked();
    void on_weightsButton_clicked();
    void on_videoButton_clicked();

private:
    Ui::MainWindow *ui;
    YOLO yolo_;
};

#endif // STARTWINDOW_H
