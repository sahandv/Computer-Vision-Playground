#include "textprocess.h"


TextProcess::TextProcess() {
}

TextProcess::TextProcess(const TextProcess& orig) {
}

TextProcess::~TextProcess() {
}

//const std::vector<std::string> TextProcess::explode(const std::string& s, const char& c){
//    std::string buff{""};
//    std::vector<std::string> v;

//    for(auto n:s)
//    {
//        if(n != c) buff+=n; else
//        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
//    }
//    if(buff != "") v.push_back(buff);

//    return v;
//}

std::string TextProcess::textPartExtractor(std::string full_text, std::string code) {
    std::cout << "\nStarting to dig the log!\n";
    size_t pos;
    bool start = false;
    pos = full_text.find(code);
    if (pos!=std::string::npos)
    {
        std::vector<char> temp;
        std::vector<char> temp_whole_text;
        std::cout << "\nPart position: "<< pos<<"\n";
        int j = 0;
        for (int i = pos; i < full_text.size() && full_text[i] != '}' ; i++)
        {
            if(start)
            {
                std::cout << full_text[i];
                temp.push_back(full_text[i]);
//                temp[j] = full_text[i];
            }

            if(full_text[i] == '{')                                                         //start of part string
            {
                start = true;
                std::cout << "\nHere the text is!\n";
            }

            temp_whole_text.push_back(full_text[i]);
            j++;


        }

        temp_whole_text.push_back(full_text[j+1]);

        std::string full_part( temp_whole_text.begin(), temp_whole_text.end() );
        part_text_full = full_part;

        std::string part( temp.begin(), temp.end() );
        part_text = part;

        return part;

    }
    else
    {
        std::cout << "\n String is: \n" << full_text
                  << "\n Code is: " << code
                  << "\nI found nothing...";
       std::string part = "Nothing was found. This might be an error in application. Try choosing a new path or deleting old files. :(";
       return part;
    }

}

std::vector<std::vector<int>> TextProcess::rectExtractor(std::string full_text) {
    std::vector<std::vector<int>> result;

    for(int i = 0; i < full_text.size(); i++)
    {
        if(full_text[i] == '[')                                                         //start of part string
        {
            std::vector<int> rect;
            std::vector<char> single_num;
//            int rect_array[4];
//            int k = 0;
            i++;
            for(int j = i; j < full_text.size(); j++)
            {

                if(full_text[i] != ']' && full_text[i] != ',')
                {
                    single_num.push_back(full_text[i]);
//                    std::cout << "*" << full_text[i];
                }
                else if(full_text[i] == ']')
                {
                    std::string num( single_num.begin(), single_num.end() );
//                    std::cout << ">> " << num << std::endl;
                    std::string::size_type sz;
                    int tmp_num = std::stoi(num,&sz);
                    rect.push_back(tmp_num);
//                    rect_array[k] = tmp_num;
//                    k++;
                    single_num.clear();
                    break;
                }
                else if(full_text[i] == ',')
                {
                    std::string num( single_num.begin(), single_num.end() );
//                    std::cout << ">> " << num << std::endl;
                    std::string::size_type sz;
                    int tmp_num = std::stoi(num,&sz);
                    rect.push_back(tmp_num);
//                    rect_array[k] = tmp_num;
//                    k++;
                    single_num.clear();
                }
                i++;
            }
            std::cout << '[' <<rect[0] << ',' << rect[1] << ',' << rect[2] << ',' << rect[3] << "]\n";

            result.push_back(rect);
            rect.clear();
        }
    }

    return result;
}

std::string TextProcess::textPartRemover(std::string full_text, std::string code) {
//    std::cout << "\n****full text: \n" << full_text << "\****code: \n" <<code << "\n";
    size_t pos;
    pos = full_text.find(code);

    if (pos!=std::string::npos)
    {
        int j = 1;
        for(int i = pos; i < full_text.size() && full_text[i] != '}'; i++ )
        {
            j++;
        }

        full_text.erase(pos,j);
    }

    return full_text;
}

std::string TextProcess::readLog(std::string log_file) {
    std::string full_text;
    std::ifstream file(log_file.c_str());
    std::string str;
    while (std::getline(file, str))
    {
      full_text += str;
      full_text.push_back('\n');
    }
    return full_text;
}
