#ifndef PANELFINDER_H
#define PANELFINDER_H

#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include <tesseract/baseapi.h>
#include "opencv2/core/utility.hpp"
//#include "opencv2/text.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <utility>
#include <omp.h>
#include "hunspell-lib/hunspell.hxx"
//#include <hunspell/hunspell.hxx>

class PanelFinder{
public:

    PanelFinder(std::string images_code = "#image_code#",std::string images_prefix = "crop_",  std::string root = "", std::string imagesPrefix = "crop-");
    PanelFinder(const PanelFinder& orig);
    virtual ~PanelFinder();

    char AnyKey;
    std::string tempPath, folderPath, imagePath, textPath, outputPath, fullLog, imageCode, imagePrefix;
    cv::Mat inputImage, MSERresponseImage ,boxedImage , greyImage, MSERpart, finalImage;
    cv::Size sizes ;                                                          //the dst image size,e.g.100x100 // CHANGE IT HERE. HEIGHT OR WIDTH BETTER BE DYNAMIC
    std::vector<std::vector<cv::Point> > MSERcontours;
    std::vector<cv::Rect> MSERboxes;
    cv::Vec3b colorList[12];
    std::vector<cv::Rect> MSERpositions;
    std::vector<int> cropNumbers;
    double time;
    int cropCount,panelCount,uniqueSuggestionId;

    bool createLog( std::string path = "" , std::string file = "log.txt", std::string openingLine = "",bool fullLogSet = false);
    cv::Mat correctGamma( cv::Mat& img, double gamma );
    cv::Mat setImage( std::string  image);
    void setPath(std::string folder,std::string image,std::string text,std::string output);
    void initializeImages(int blurSize = 1);
    void initializeImages(int blurSize = 1, double gammaValue = 0.8);
    int segmentate(int minArea = 10000, int maxArea=100000, int delta = 5, double minDiversity = 0.2, bool keepTemp = true);
    void readParts(bool writeOnImage = true, std::string lang = "tur", bool spell_check = true, bool fromDisk = true);
    void writeImagesToDisk(bool mser = true, bool box = true, bool final = false);
    void openImage(std::string imageName,cv::Mat image);
    std::string preProcessText(std::string inputText);
    std::vector<std::string> explodeWords(std::string const & s, char delim=' ', bool no_line=true);
    std::vector<std::string> spellCheck(std::string input, int & result);


private:

};

#endif // PANELFINDER_H
