#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage imdisplay;
    QTimer* Timer;
    void display_image(cv::Mat mat_img, QApplication* app);

    signals:
    public slots:
        //void display_image(cv::Mat img) ;

private:
        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
