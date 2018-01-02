#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <stdio.h>
#include <string>
#include <omp.h>
#include <QImageReader>
#include <QDir>
#include <fstream>
#include <QEventLoop>
#include "panelfinder.h"
#include "textprocess.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString directoryPath;
    QString imagePath;
    std::string original_image_path, final_image_path;
    std::string fullText;
    std::string output_folder;
    std::string temp_image_folder;
    QImage original_q_image;
    bool done = false;
    bool checkSpelling = false;
    bool writeOnImage = false;

    PanelFinder *obj = new PanelFinder("#image_1#","crop_");
    TextProcess *obj_txt = new TextProcess();

private slots:

    void on_imageOpen_clicked();

    void on_directoryOpen_clicked();

    void on_startButton_clicked();

    void on_tabWidget_tabBarClicked(int index);


    void on_spell_check_clicked();

    void on_draw_text_clicked();

    void on_open_mser_clicked();

    void on_open_box_clicked();

    void on_review_clicked();

    void on_remove_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
