#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    ui(new Ui::MainWindow),
    yolo_()
{
    centerAndResize();

    ui->setupUi(this);

    setTagText(ui->cfgTag, QString::fromStdString(yolo_.getCfgFile()));
    setTagText(ui->namesTag, QString::fromStdString(yolo_.getNamesFile()));
    setTagText(ui->weightsTag, QString::fromStdString(yolo_.getWeightsFile()));
    setTagText(ui->mediaTag, QString::fromStdString(yolo_.getInputFile()));
}

void MainWindow::centerAndResize() {
    QRect screen_geometry = QApplication::desktop() -> availableGeometry();
    int screen_width = screen_geometry.width();
    int screen_height = screen_geometry.height();

    double reduction_proportion = 80.0 / 100.0;
    int reducted_width = round(screen_width * reduction_proportion);
    int reducted_height = screen_height * reduction_proportion;

    int x_pos = screen_width / 2;
    int y_pos = screen_height / 2;

    this->resize(reducted_width, reducted_height);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    DetectionWindow *window = new DetectionWindow(yolo_, this);
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

void MainWindow::on_mediaButton_clicked()
{
    std::string current_file = yolo_.getInputFile();
    std::string file_extension = "Media File(*.mp4, *.avi, *.jpeg, *.jpg, *.png);;All Files (*)";
    std::string file_name = getFilePath(current_file, file_extension);

    yolo_.setInputFile(file_name);
    ui->mediaTag->setText(QString::fromStdString(file_name));
}

std::string MainWindow::getFilePath(std::string current_file, std::string file_extension) {
    return QFileDialog::getOpenFileName(this,
                                        tr("Choose File"), QString::fromStdString(current_file),
                                        tr(file_extension.c_str())).toStdString();
}

void MainWindow::setTagText(QLabel* tag, QString text) {
    tag->setText((std::ifstream(text.toStdString()).good()) ? text : "");
}
