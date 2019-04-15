#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include <yolo.h>
#include <mainwindow.h>

namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();
    YOLO yolo;
    std::string getFilePath(std::string current_file, std::string file_extension);

private slots:
    void on_pushButton_clicked();

    void on_cfgButton_clicked();

    void on_namesButton_clicked();

    void on_weightsButton_clicked();

    void on_videoButton_clicked();

private:
    Ui::StartWindow *ui;
};

#endif // STARTWINDOW_H
