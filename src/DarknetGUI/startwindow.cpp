#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow),
    yolo()
{
    ui->setupUi(this);

    ui->cfgTag->setText(QString::fromStdString(yolo.getCfg_file()));
    ui->namesTag->setText(QString::fromStdString(yolo.getNames_file()));
    ui->weightsTag->setText(QString::fromStdString(yolo.getWeights_file()));
    ui->videoTag->setText(QString::fromStdString(yolo.getFilename()));
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_pushButton_clicked()
{
    MainWindow *window = new MainWindow(yolo);
    window -> show();
    hide();
}

void StartWindow::on_cfgButton_clicked()
{
    std::string current_file = yolo.getCfg_file();
    std::string file_extension = "CFG File(*.cfg);;All Files (*)";
    std::string file_name = getFilePath(current_file, file_extension);

    yolo.setCfg_file(file_name);
    ui->cfgTag->setText(QString::fromStdString(file_name));
}

void StartWindow::on_namesButton_clicked()
{
    std::string current_file = yolo.getNames_file();
    std::string file_extension = "Names File(*.names);;All Files (*)";
    std::string file_name = getFilePath(current_file, file_extension);

    yolo.setNames_file(file_name);
    ui->namesTag->setText(QString::fromStdString(file_name));
}

void StartWindow::on_weightsButton_clicked()
{
    std::string current_file = yolo.getWeights_file();
    std::string file_extension = "Weights File(*.weights);;All Files (*)";
    std::string file_name = getFilePath(current_file, file_extension);

    yolo.setWeights_file(file_name);
    ui->weightsTag->setText(QString::fromStdString(file_name));
}

void StartWindow::on_videoButton_clicked()
{
    std::string current_file = yolo.getFilename();
    std::string file_extension = "Video File(*.mp4);;All Files (*)";
    std::string file_name = getFilePath(current_file, file_extension);

    yolo.setFilename(file_name);
    ui->videoTag->setText(QString::fromStdString(file_name));
}

std::string StartWindow::getFilePath(std::string current_file, std::string file_extension) {
    return QFileDialog::getOpenFileName(this,
                                        tr("Choose File"), QString::fromStdString(current_file),
                                        tr(file_extension.c_str())).toStdString();
}
