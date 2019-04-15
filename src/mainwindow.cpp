#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    yolo_()
{
    this->setWindowTitle(QApplication::translate("SaferAuto Detection", "SaferAuto Detection", nullptr));

    ui->setupUi(this);

    ui->cfgTag->setText(QString::fromStdString(yolo_.getCfgFile()));
    ui->namesTag->setText(QString::fromStdString(yolo_.getNamesFile()));
    ui->weightsTag->setText(QString::fromStdString(yolo_.getWeightsFile()));
    ui->videoTag->setText(QString::fromStdString(yolo_.getInputFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    DetectionWindow *window = new DetectionWindow(yolo_);
    window -> show();
    hide();
}

void MainWindow::on_cfgButton_clicked()
{
    std::string current_file = yolo_.getCfgFile();
    std::string file_extension = "CFG File(*.cfg);;All Files (*)";
    std::string file_name = getFilePath(current_file, file_extension);

    yolo_.setCfgFile(file_name);
    ui->cfgTag->setText(QString::fromStdString(file_name));
}

void MainWindow::on_namesButton_clicked()
{
    std::string current_file = yolo_.getNamesFile();
    std::string file_extension = "Names File(*.names);;All Files (*)";
    std::string file_name = getFilePath(current_file, file_extension);

    yolo_.setNamesFile(file_name);
    ui->namesTag->setText(QString::fromStdString(file_name));
}

void MainWindow::on_weightsButton_clicked()
{
    std::string current_file = yolo_.getWeightsFile();
    std::string file_extension = "Weights File(*.weights);;All Files (*)";
    std::string file_name = getFilePath(current_file, file_extension);

    yolo_.setWeightsFile(file_name);
    ui->weightsTag->setText(QString::fromStdString(file_name));
}

void MainWindow::on_videoButton_clicked()
{
    std::string current_file = yolo_.getInputFile();
    std::string file_extension = "Video File(*.mp4);;All Files (*)";
    std::string file_name = getFilePath(current_file, file_extension);

    yolo_.setInputFile(file_name);
    ui->videoTag->setText(QString::fromStdString(file_name));
}

std::string MainWindow::getFilePath(std::string current_file, std::string file_extension) {
    return QFileDialog::getOpenFileName(this,
                                        tr("Choose File"), QString::fromStdString(current_file),
                                        tr(file_extension.c_str())).toStdString();
}
