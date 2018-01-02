#include "panelfinder.h"


PanelFinder::PanelFinder( std::string images_code, std::string images_prefix, std::string root, std::string imagesPrefix) {
    std::string tmpImagePath = "file/.temp/images/";
    imageCode = images_code;
    tempPath = tmpImagePath+imagesPrefix;
    folderPath = root;

    uniqueSuggestionId = 1;
    imagePrefix = images_prefix;
    double t = (double)cv::getTickCount();

    cv::Vec3b tempColors[] =
    {
        cv::Vec3b(0,0,255),
        cv::Vec3b(0,128,255),
        cv::Vec3b(0,255,255),
        cv::Vec3b(0,255,0),
        cv::Vec3b(255,128,0),
        cv::Vec3b(255,255,0),
        cv::Vec3b(255,0,0),
        cv::Vec3b(200,10,0),
        cv::Vec3b(100,200,20),
        cv::Vec3b(25,100,200),
        cv::Vec3b(255,0,255),
        cv::Vec3b(50,150,255)
    };

    std::copy(tempColors, tempColors + 12, colorList);

}

PanelFinder::PanelFinder(const PanelFinder& orig) {
}

PanelFinder::~PanelFinder() {
}

void PanelFinder::setPath(std::string folder,std::string image,std::string text,std::string output) {

    folderPath = folder;
    imagePath = image;
    textPath = text;
    outputPath = output;

}

////////////////////////////////////////////////////////////////////////////////
//                      IMAGE PROCESSING
////////////////////////////////////////////////////////////////////////////////

cv::Mat PanelFinder::correctGamma( cv::Mat& img, double gamma ) {
    double inverse_gamma = 1.0 / gamma;

    cv::Mat lut_matrix(1, 256, CV_8UC1 );
    uchar * ptr = lut_matrix.ptr();
    for( int i = 0; i < 256; i++ )
       ptr[i] = (int)( pow( (double) i / 255.0, inverse_gamma ) * 255.0 );

    cv::Mat result;
    cv::LUT( img, lut_matrix, result );

    return result;
}

cv::Mat PanelFinder::setImage( std::string  image) {

    inputImage = cv::imread( image.c_str() );
//    int height;// = 2160;
    double width,aspect_ratio,height;
    aspect_ratio =  (double)inputImage.cols / (double)inputImage.rows;
    std::cout << "*aspect ratio: " <<aspect_ratio << " = " << inputImage.rows << "/"<<inputImage.cols;
    width = 1920;
    height = width / aspect_ratio;
    std::cout << "*height: " <<height;
    sizes=cv::Size((int)width,(int)height);                                     //4K image
    return inputImage;                                                          // Not needed, unless you want to check image on go!
}

void PanelFinder::initializeImages(int blurSize, double gammaValue) {

    cv::resize(inputImage,MSERresponseImage,sizes);
    cv::cvtColor(MSERresponseImage, greyImage, cv::COLOR_BGR2GRAY);
    cv::medianBlur(greyImage, greyImage, blurSize);
    cv::equalizeHist( greyImage, greyImage );

    greyImage = correctGamma( greyImage, gammaValue );
    greyImage.copyTo(boxedImage);
    greyImage.copyTo(finalImage);
}

void PanelFinder::initializeImages(int blurSize) {

    cv::resize(inputImage,MSERresponseImage,sizes);
    cv::cvtColor(MSERresponseImage, greyImage, cv::COLOR_BGR2GRAY);
    cv::medianBlur(greyImage, greyImage, blurSize);
    cv::equalizeHist( greyImage, greyImage );

    greyImage.copyTo(boxedImage);
    greyImage.copyTo(finalImage);
}

int PanelFinder::segmentate(int minArea, int maxArea, int delta, double minDiversity, bool keepTemp) {

    double t = (double)cv::getTickCount();
    cv::Ptr<cv::MSER> algo = cv::MSER::create(delta,minArea,maxArea,0.10,minDiversity,200,1.01,2,0);
    algo->detectRegions(greyImage, MSERcontours, MSERboxes);
    t = (double)cv::getTickCount() - t;
    time = t;
    printf( "MSER extracted %d contours in %g ms.\n", (int)MSERcontours.size(),
           t*1000./cv::getTickFrequency() );
    time = t*1000./cv::getTickFrequency();

    std::vector<int> rect_internals;
    std::vector<std::vector<int> >  history_rect;                                // (id, xmin, ymin, xmax, ymax)
    int history_rect_i = 0;
    cropCount = 0;

    for( int i = 0; i < (int)MSERcontours.size() ; i++ )                        // for all number of regions
    {
//        vector<char> char_crop;
        std::cout << "\n ---------------------------------------\n";
        const std::vector<cv::Point>& r = MSERcontours[i];
        for ( int j=0; j < (int)r.size(); j++ )                                 // r.size => number of points in r
        {
            cv::Point pt = r[j];                                                // Copy each point in copied mser (r) into a point named pt
            MSERresponseImage.at<cv::Vec3b>(pt) = colorList[i%12];              // Color point pt of image img to i%9
        }

        int xmin=MSERboxes[i].x,ymin=MSERboxes[i].y,xmax=MSERboxes[i].width,ymax=MSERboxes[i].height;

        if(history_rect_i == 0)
        {
            std::cout << "\n The very first rectangle it is!.\n";

            rect_internals.push_back(i);
            rect_internals.push_back(xmin);
            rect_internals.push_back(ymin);
            rect_internals.push_back(xmax);
            rect_internals.push_back(ymax);

            history_rect.push_back(rect_internals);
            rect_internals.clear();
            history_rect_i++;
        }

        int distance = 150;
        if( ( (history_rect[history_rect_i-1][1]<xmin+distance and history_rect[history_rect_i-1][1]>xmin-distance)
           and (history_rect[history_rect_i-1][2]<ymin+distance and history_rect[history_rect_i-1][2]>ymin-distance)
           and (history_rect[history_rect_i-1][3]<xmax+distance and history_rect[history_rect_i-1][3]>xmax-distance)
           and (history_rect[history_rect_i-1][4]<ymax+distance and history_rect[history_rect_i-1][4]>ymax-distance)
           and history_rect_i > 0) )
        {
            std::cout << "\n This is a new neighbor, adding it to the history_rect.";
//              last history_rect rect and this rect are similar

            rect_internals.push_back(i);
            rect_internals.push_back(xmin);
            rect_internals.push_back(ymin);
            rect_internals.push_back(xmax);
            rect_internals.push_back(ymax);

            history_rect.push_back(rect_internals);
            rect_internals.clear();
            history_rect_i++;

        }
        else
        {
            std::cout << "\n It's not a neighbor. Resetting the history_rect and making average rectangle.";
            // find median rectangle and crop it.
            int rows =  history_rect_i;
            int avg_xmin = 0;
            int avg_ymin = 0;
            int avg_xmax = 0;
            int avg_ymax = 0;
            for(int iii= 0; iii < rows; iii++)
            {
                //xmin
                avg_xmin = avg_xmin + history_rect[iii][1];
                //ymin
                avg_ymin = avg_ymin + history_rect[iii][2];
                //xmax
                avg_xmax = avg_xmax + history_rect[iii][3];
                //ymax
                avg_ymax = avg_ymax + history_rect[iii][4];
            }
            avg_xmin = avg_xmin/rows;
            //ymin
            avg_ymin = avg_ymin/rows;
            //xmax
            avg_xmax = avg_xmax/rows;
            //ymax
            avg_ymax = avg_ymax/rows;

            std::cout << "\n The average rectangle calculated.";
            std::cout << "\n xmin = "<< avg_xmin;
            std::cout << "\n ymin = "<< avg_ymin;
            std::cout << "\n xmax = "<< avg_xmax;
            std::cout << "\n ymax = "<< avg_ymax;


            std::ostringstream filename;
            filename << folderPath << imagePath << imagePrefix << cropCount << ".jpg";
            cropCount++;
            MSERpart=greyImage(cv::Rect(avg_xmin, avg_ymin, avg_xmax, avg_ymax));

            std::cout << "\n The rectangle marked on grey image..";

            if(keepTemp)
            {
                std::cout<< "\n Rectangle crop is being saved to disk... ";
                cv::imwrite(filename.str().c_str(),MSERpart);
                std::cout << " Rectangle crop is now saved to disk!";
            }

            cv::RotatedRect box = cv::fitEllipse( r );
            cv::rectangle(boxedImage,cv::Rect(avg_xmin, avg_ymin, avg_xmax, avg_ymax), cv::Scalar(255,0,0));
            MSERpositions.push_back(cv::Rect(avg_xmin, avg_ymin, avg_xmax, avg_ymax));

            box.angle=(float)CV_PI/2-box.angle;
            std::cout << "\n Rectangle box is marked on original image.";

            // START OVER

            history_rect_i = 0;
            history_rect.clear();

            rect_internals.push_back(i);
            rect_internals.push_back(xmin);
            rect_internals.push_back(ymin);
            rect_internals.push_back(xmax);
            rect_internals.push_back(ymax);

            history_rect.push_back(rect_internals);
            rect_internals.clear();
            history_rect_i = 1;

            std::cout << "\n History step counter set to zero and new points are saved in history_rect[0][] .";
            std::cout << "\n ---------------------------------------\n";
        }


    }

    return cropCount;
}

void PanelFinder::readParts(bool writeOnImage, std::string lang, bool spell_check, bool fromDisk){
    panelCount = 0;
    for( int i = cropCount-1; i >= 0; i-- )                            // for all number of regions
    {
        std::ostringstream filename;
        filename << folderPath << imagePath << imagePrefix << i << ".jpg";

        cv::Mat temp_part;
        std::cout << "\n ---------------------------------------\n";
        if(fromDisk)
        {
            temp_part=cv::imread(filename.str().c_str(),CV_LOAD_IMAGE_GRAYSCALE);
            std::cout<< "> Reading from disk";
        }
        else
        {
            temp_part=greyImage(MSERpositions[i]);
            std::cout<< "> Reading from memory";
        }

        std::cout << "\nNow reading" << i <<"\n";

        tesseract::TessBaseAPI tess;
        tess.Init(NULL, lang.c_str(), tesseract::OEM_DEFAULT);
        tess.SetVariable("tessedit_char_blacklist", "!?~â“ê*”':`«»_›<>%^/\\\"()î#Â{}ý;|$,.[]—Ä±");
        tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
        tess.SetImage((uchar*)temp_part.data, temp_part.cols, temp_part.rows, 1, temp_part.cols);
        std::string char_crop = tess.GetUTF8Text();
        std::cout << "\noriginal: \n" << char_crop << "\n";

        char_crop = PanelFinder::preProcessText(char_crop);
        //      End of Regex pre processing

        if((int)char_crop.length()<4)
        {
            std::remove(filename.str().c_str());
        }
        else
        {
            panelCount++;
            ////////////////////////////////////////////////////////////////////
            if(writeOnImage==true)
            {

                int bottom = 100;
                cv::copyMakeBorder( temp_part, temp_part, 10, bottom, 100, 100, cv::BORDER_CONSTANT, cv::Scalar::all(0) );

                int fontFace = cv::FONT_HERSHEY_DUPLEX;
                double fontScale = 1;
                int thickness = 1;
                int baseline=0;
                cv::Size textSize = cv::getTextSize(char_crop, fontFace,fontScale, thickness, &baseline);
                cv::Point textOrg((temp_part.cols - textSize.width)/2,(temp_part.rows + textSize.height)*2/3);
                cv::putText(temp_part, char_crop, textOrg, fontFace, fontScale,cv::Scalar::all(255), thickness, cv::LINE_8, false); //Scalar::all(255)
                std::ostringstream filename_new;
                filename_new << folderPath << imagePath << imagePrefix << i << "_read.jpg";
                cv::imwrite(filename_new.str().c_str(),temp_part);
            }
            ////////////////////////////////////////////////////////////////////

            // Save Potential Panels To A Paragraph Of Log
            std::cout << char_crop << std::endl;
            std::ofstream panelTexts (fullLog.c_str(), std::ios_base::app);
            cv::Rect temp_rect = MSERpositions[i];

            if (panelTexts.is_open())
            {
              panelTexts << "\n##" << i
                         << "\n[" << temp_rect.x
                         << "," << temp_rect.y
                         << "," << temp_rect.width
                         << "," << temp_rect.height
                         <<"]"
                         << "\n{" << char_crop << "}";
              panelTexts.close();
            }
            cropNumbers.push_back(i);

            if(spell_check)
            {
                // Extract Words Of The Paragraph And Create Suggestions
                std::vector<std::string> words = PanelFinder::explodeWords(char_crop,' ');
                for(int word_step = 0; word_step < words.size(); word_step++)
                {
                    std::vector<std::string> suggestionsList;
                    std::cout << "\nword " << word_step << " -> " << words[word_step];
                    int spellCheckResult;
    //                std::cin >> spellCheckResult;
                    suggestionsList = PanelFinder::spellCheck(words[word_step],spellCheckResult);

                    if(spellCheckResult<1 and words[word_step].size()>2)
                    {
                        std::cout << std::endl << "spelled word " << words[word_step] << " check: (0 = wrong) " << spellCheckResult << std::endl;

                    // If A Word Is Misspelled, Add It To Database
                        std::ostringstream suggestionDataFile;
                        suggestionDataFile << imageCode << "--" << i << "--" <<word_step;
                        uniqueSuggestionId++;
                        std::ostringstream suggestionLogLines;
                        suggestionLogLines << "sg-" << uniqueSuggestionId << "\n"
                                << imageCode << "\n"
                                << i << "\n"
                                << word_step << "\n"
                                <<  words[word_step] << "\n"
                                <<  suggestionsList.size() << "\n"
                                ;

                        for(int suggestion_step = 0; suggestion_step < suggestionsList.size(); suggestion_step++)
                        {
                            suggestionLogLines << suggestionsList[suggestion_step] << "\n" ;
                        }


                        PanelFinder::createLog(textPath,suggestionDataFile.str(),suggestionLogLines.str());
                    }
                }
            }
        }

    }

}


void PanelFinder::writeImagesToDisk(bool mser, bool box, bool final){
    std::string path = folderPath+outputPath;
    if(mser) cv::imwrite(path+"response.png",MSERresponseImage);
    if(box)
    {
        cv::imwrite(path+"boxed.png",boxedImage);
        cv::imwrite(path+"grey.png",greyImage);
    }
    if(final) cv::imwrite(path+"final.png",finalImage);
}

void PanelFinder::openImage(std::string imageName,cv::Mat image) {
    cv::namedWindow(imageName, cv::WINDOW_NORMAL );
    cv::imshow(imageName,image);
}

////////////////////////////////////////////////////////////////////////////////
//                      TEXT PROCESSING
////////////////////////////////////////////////////////////////////////////////

bool PanelFinder::createLog( std::string path , std::string file, std::string openingLine, bool fullLogSet) {

    std::string log = folderPath+path+file;
    std::ofstream textLog (log.c_str());

    if(fullLogSet)
    {
        fullLog = log;
    }

    if (textLog.is_open())
    {
      textLog << openingLine;
      textLog.close();
      return true;
    }
    else
        return false;

}

std::vector<std::string> PanelFinder::explodeWords(std::string const & s, char delim, bool no_line)   // Default delim is ' '
{
    std::vector<std::string> result;

    std::string char_crop_tmp = s;
    for(int char_crop_tmp_step = 0; char_crop_tmp_step<char_crop_tmp.size() && no_line==true; char_crop_tmp_step++)
    {
        if(char_crop_tmp[char_crop_tmp_step]=='\n')
            char_crop_tmp[char_crop_tmp_step]=' ';
    }

    std::istringstream iss(char_crop_tmp);
    for (std::string token; std::getline(iss, token, delim); )
    {
        result.push_back(std::move(token));
    }

    return result;
}


std::vector<std::string> PanelFinder::spellCheck(std::string inputWord, int & result){

        std::vector<std::string> resultList;
        char ** suggestionsList;
        Hunspell * spellChecker = new Hunspell("dictionaries/TR.aff","dictionaries/TR.dic");
        int correct = spellChecker->spell(inputWord.c_str());
        result = correct;
        int suggestions = spellChecker->suggest(&suggestionsList,inputWord.c_str());
        for(int i = 0; i < suggestions; i++)
        {
            std::string thisSuggestion = suggestionsList[i];
            resultList.push_back(thisSuggestion);
        }
        delete spellChecker;

        return resultList;

}

std::string PanelFinder::preProcessText(std::string inputText){

    for(int str_step = 0, inline_step = 0, start_step = 0 ; str_step < inputText.length(); str_step++)
    {
        if((inputText[str_step] == '\n' or str_step+1==inputText.length()) && inline_step<3)
        {
//                        char_crop[start_step] = "";
//                        char_crop[str_step] = "";
            inputText.erase(start_step,inline_step+1);

            inline_step = 0;
            start_step = str_step+1;
        }

        if(inputText[str_step] == '\n')
        {
            inline_step = 0;
            start_step = str_step+1;
        }
        else
        {
            inline_step++;
        }

    }

    int repeated = 1;
    int tmp_length = inputText.length();
    int str_step = 0;
    while( str_step < tmp_length )
    {
        if(inputText[str_step] == inputText[str_step+1])
        {
            // if me == me + 1

            repeated++;
            std::cout << repeated << " >"<< inputText[str_step] <<">";

        }
        else
        {
            if(repeated > 3)
            {
                std::cout << "this char is repeated too many times '";
                std::cout << inputText[str_step-1] << "'\n";
                int pos = str_step-repeated+1;
                int len = repeated;
                std::cout << "pos > '"<< pos <<"' which is '"<< inputText[pos] <<"'.\n";
                std::cout << "len > '"<< len <<"' which is '"<< inputText[pos+len] <<"'.\n";
                inputText.erase((pos),len);
                tmp_length--;
                std::cout << "Good! I took care of it ;) \n";
            }

            repeated = 1;
        }
        str_step++;
    }


    for(int str_step = 0; str_step < inputText.length(); str_step++)
    {
        if(inputText[str_step]==inputText[str_step+1] &&(
                inputText[str_step]== ' ' ||
                inputText[str_step]== '\n' ||
                inputText[str_step]== ':' ||
                inputText[str_step]== '.' ||
                inputText[str_step]== '-'))
        {
            inputText.erase(str_step,1);
        }

        if(inputText[str_step]=='?')
            inputText.erase(str_step,1);

    }


    return inputText;
}
