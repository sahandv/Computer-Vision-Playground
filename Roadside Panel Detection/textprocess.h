#ifndef TEXTPROCESS_H
#define TEXTPROCESS_H

#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include <tesseract/baseapi.h>
#include "opencv2/core/utility.hpp"
//#include "opencv2/text.hpp"
#include <tesseract/baseapi.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>
#include <omp.h>
//#include "hunspell-lib/hunspell.hxx"
//#include <hunspell/hunspell.hxx>

class TextProcess {
public:
    std::string part_text;
    std::string part_text_full;
    TextProcess();
    TextProcess(const TextProcess& orig);
    virtual ~TextProcess();
//    const std::vector<std::string> explode(const std::string& s, const char& c);
    std::string textPartExtractor(std::string full_text, std::string code);
    std::vector<std::vector<int>> rectExtractor(std::string full_text);
    std::string textPartRemover(std::string full_text, std::string code);
    std::string readLog(std::string log_file);
private:

};

#endif // TEXTPROCESS_H
