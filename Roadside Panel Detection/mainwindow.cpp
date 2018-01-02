#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_imageOpen_clicked()
{
    QString home = QDir::homePath();
    imagePath = QFileDialog::getOpenFileName(this,"Open Image File",home,"Image File (*.jpg);;Image File (*.png);;Image File (*.jpeg);;Image File (*.bmp)");
    original_q_image = QImage(imagePath.toStdString().c_str());
    ui->inputImageShow->setScaledContents(true);
    ui->inputImageShow->setPixmap(QPixmap::fromImage(original_q_image));
}

void MainWindow::on_directoryOpen_clicked()
{
    QString home = QDir::homePath();
    directoryPath = QFileDialog::getExistingDirectory(this,"Open Output Directory",home);
    ui->pathShow->setText("<font color=\"#cc0000\">Output root path : </font><font color=\"#4d4d4d\">"+directoryPath+"</font>");
}


void MainWindow::on_startButton_clicked()
{
    if(original_q_image.isNull())
    {
        QMessageBox::warning(this,tr("Error"),tr("Please select an image first!"),tr(" OK "));
    }
    else if(directoryPath.isNull())
    {
        QMessageBox::warning(this,tr("Error"),tr("Please select the output path first!"),tr(" OK "));
    }
    else
    {
        ui->startButton->setText("Wait please....");
        // Create Temp Directory
        QString tmp_image_path = "/file/.temp/images/";
        QString tmp_text_path = "/file/.temp/texts/";
        QString output_path = "/file/output/";

        QString dir_q_string = QDir::rootPath();
        QDir dir = QDir::root();

        dir.mkpath(directoryPath+tmp_image_path);
        dir.mkpath(directoryPath+tmp_text_path);
        dir.mkpath(directoryPath+output_path);


        QString png_file_name = "original.png";
        QString final_file_name = "final.png";
        original_q_image.save(dir_q_string+directoryPath+output_path+png_file_name, "PNG");

        QString original_image_q_path = dir_q_string+directoryPath+output_path+png_file_name;
        QString final_image_q_path = dir_q_string+directoryPath+output_path+final_file_name;

        original_image_path = original_image_q_path.toStdString();
        final_image_path = final_image_q_path.toStdString();

        ui->pathShow->setText("<font color=\"#cc0000\">Output path : </font><font color=\"#4d4d4d\">"+directoryPath+output_path+"</font>");

        std::string rootDirectory = directoryPath.toStdString();
        std::string temp_text_folder = tmp_text_path.toStdString();
        temp_image_folder = tmp_image_path.toStdString();
        output_folder = output_path.toStdString();

        std::string lang = "eng";
        obj->imageCode = "test_image";
        obj->setPath(rootDirectory,temp_image_folder,temp_text_folder,output_folder);
        obj->createLog(output_folder,obj->imageCode+"-full-text.txt","CROP LIST AND TEXT WITH MATCHING NUMBERS \n",true);
        obj->setImage(original_image_path);
        obj->initializeImages(1,0.6);
        obj->segmentate();
        obj->writeImagesToDisk(true,true,true);
        obj->readParts(writeOnImage,lang,checkSpelling);

        done = true;

        fullText = obj_txt->readLog(obj->fullLog.c_str());
        ui->log_view_text->setText(QString::fromStdString(fullText));

        std::vector<std::string> parts = obj->explodeWords(fullText,'#',false);
        for(auto n:parts) std::cout << n << std::endl;

        QImage response;
        QImage boxed;
        response.load(directoryPath+output_path+"response.png","PNG");
        boxed.load(directoryPath+output_path+"boxed.png","PNG");
        ui->boxes->setScaledContents(true);
        ui->response->setScaledContents(true);
        ui->boxes->setPixmap(QPixmap::fromImage(boxed));
        ui->response->setPixmap(QPixmap::fromImage(response));



        for(int i = 0; i < obj->panelCount ; i++)
        {
            QString icon_path = directoryPath+tmp_image_path+"crop_"+QString::number(obj->cropNumbers[i])+".jpg";
            QIcon panelIcon = QIcon(icon_path);
            std::cout << icon_path.toStdString() << std::endl;
            QString cropNumberTmp = QString::number(obj->cropNumbers[i]);
            std::cout << "#" << cropNumberTmp.toStdString() << std::endl;
            ui->panelList->addItem(panelIcon,QString::number(obj->cropNumbers[i]));
        }

        ui->startButton->setText("Process Again!");
        ui->review->setEnabled(true);
        ui->panelList->setEnabled(true);
    }
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(done)
    {
        QString text;

        text =  "> There are <span style=\"color: red;\">"+QString::number(obj->panelCount)+"</span> potential panels in this image. These parts are extracted in <span style=\"color: red;\">"
                +QString::number(obj->time)+
                +" </span>ms .<br/>"
               +"> Total number of cropped parts before text processing is <span style=\"color: red;\">"+QString::number(obj->cropCount)
               +"</span>.<br/>"
               +"> More detailed information is available below and at the output directory. (<span style=\"color: #e69900;\">"
               +directoryPath
               +" </span>)<br/>";
        ui->result1->setText(text);

    }
    else
    {
//        QMessageBox::warning(this,tr("Oops!"),tr("Please start the process in order to see the result!"),tr(" OK "));
        ui->result1->setText("<br/><span style=\"color: red;\">Please start the process in order to see the result.</span>");
    }
}

void MainWindow::on_spell_check_clicked()
{
    if(checkSpelling)
        checkSpelling = false;
    else
        checkSpelling = true;
}

void MainWindow::on_draw_text_clicked()
{
    if(writeOnImage)
        writeOnImage = false;
    else
        writeOnImage = true;
}


void MainWindow::on_open_mser_clicked()
{
    obj->openImage("MSER Response",obj->MSERresponseImage);
    std::cout << "open image";
}

void MainWindow::on_open_box_clicked()
{
    obj->openImage("Boxed Image",obj->boxedImage);
    std::cout << "open image";
}

void MainWindow::on_review_clicked()
{
    std::cout << "\n* Opening the part...\n";
    ui->remove->setEnabled(true);
//    ui->save->setEnabled(true);
    QString panel_code = ui->panelList->currentText();
    std::string code = panel_code.toStdString();
    std::cout << "##" << code << std::endl;
    std::string panel_text;
    panel_text = obj_txt->textPartExtractor(fullText,"##"+code);
    ui->panelText->setText(QString::fromStdString(panel_text));
    for(int i = 0; i < obj->cropNumbers.size(); i++)
    {
        std::cout << "+" << obj->cropNumbers[i];
    }
}

void MainWindow::on_remove_clicked()
{
    std::string new_text;
    QString panel_code = ui->panelList->currentText();
    std::string code = panel_code.toStdString();
    new_text = obj_txt->textPartRemover(fullText,"##"+code);
    obj->createLog(output_folder,obj->imageCode+"-full-text.txt",new_text,true);
    fullText = obj_txt->readLog(obj->fullLog.c_str()); // = new_text;
    ui->log_view_text->setText(QString::fromStdString(fullText));
    QString image = directoryPath+QString::fromStdString(temp_image_folder)+"crop_"+panel_code+".jpg";
    std::remove(image.toStdString().c_str());
    int item_index = ui->panelList->currentIndex();
    ui->panelList->removeItem(item_index);
    std::string::size_type sz;
    int tmp_num = std::stoi(code,&sz);
    for(int i = 0; i < obj->cropNumbers.size(); i++)
    {
        if(obj->cropNumbers[i] == tmp_num)
        {
            std::cout << "+code content : " << code;
            std::cout << "-code index : " << i;
            obj->cropNumbers.erase(obj->cropNumbers.begin()+i);
        }
    }
//    obj->cropNumbers.erase();
    obj->panelCount--;
}

void MainWindow::on_toolButton_clicked()
{
    std::vector<std::vector<int>> rect_list = obj_txt->rectExtractor(fullText);
    obj->finalImage = cv::imread( final_image_path.c_str() );
    for(int i = 0; i< rect_list.size(); i++)
    {
        cv::rectangle(obj->finalImage,cv::Rect(rect_list[i][0], rect_list[i][1], rect_list[i][2], rect_list[i][3]), cv::Scalar(0,20,255),3);
    }
//    obj->openImage("final image",obj->finalImage);
    obj->writeImagesToDisk(false,false,true);
    QImage final_image;
    final_image.load(directoryPath+"/file/output/final.png","PNG");
    ui->imageFinalDisplay->setScaledContents(true);
    ui->imageFinalDisplay->setPixmap(QPixmap::fromImage(final_image));
}

void MainWindow::on_toolButton_2_clicked()
{
    std::vector<std::vector<int>> rect_list = obj_txt->rectExtractor(fullText);
    obj->finalImage = cv::imread( final_image_path.c_str() );
    for(int i = 0; i < obj->cropNumbers.size(); i++)
    {
        int x = obj->cropNumbers[i];
        cv::Rect rect = obj->MSERpositions[x];
        cv::rectangle(obj->finalImage,rect, cv::Scalar(0,20,255),3);
    }

    obj->writeImagesToDisk(false,false,true);
    QImage final_image;
    final_image.load(directoryPath+"/file/output/final.png","PNG");
    ui->imageFinalDisplay->setScaledContents(true);
    ui->imageFinalDisplay->setPixmap(QPixmap::fromImage(final_image));
}
