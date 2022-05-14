#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include "strnatcmp.hpp"
#include <string.h>

void print_help()
{
    std::cout << "Usage:\tfilepacker <folder> <number of files per group>\n";
    std::cout << "Options:\n\t-r\t--recursive\tRecursive packing\n";
    std::cout << "\t-o\t--output\tSelect output folder. It must exist\n";
    std::cout << "\t-m\t--move\tMoves the files instead of copying\n";

    std::cout << "\t-h\t--help\t\tDisplay this help\n";
}

std::vector<std::string> getFilesInFolder(std::string folda, bool recursive)
{
    std::vector<std::string> toreturn;
    if(!recursive)
    {
        for (const auto & file : std::filesystem::directory_iterator(folda))
        {
            if(!std::filesystem::is_directory(file.path()))
            {
                toreturn.push_back(file.path().string());
            }
        }
    }
    else
    {
        for (const auto & file : std::filesystem::recursive_directory_iterator(folda))
        {
            if(!std::filesystem::is_directory(file.path()))
            {
                toreturn.push_back(file.path().string());
            }
        }
    }
    std::sort(toreturn.begin(), toreturn.end(), compareNat);
    return toreturn;
}
void reverseStr(std::string& str)
{
    int n = str.length();
  
    for (int i = 0; i < n / 2; i++)
    {
        std::swap(str[i], str[n - i - 1]);
    }
}

std::string cutFileName(std::string file_path)
{
    std::string r{""};
    for(int i{static_cast<int>(file_path.size())}; i >= 0; i--)
    {
        if(file_path[i] == '/')
        {
            break;
        }
        r += file_path[i];
    }
    reverseStr(r);
    return r;
}

int main(int argc, char ** argv)
{
    if(argc < 3)
    {
        print_help();
        return EXIT_FAILURE;
    }
    std::string outdir{argv[1]};
    bool recursive{false};
    bool move{false};
    if (argc > 3)
    {
        for(int i{3}; i < argc; i++)
        {
            if(strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--recursive") == 0)
            {
                recursive = true;
            }
            else if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
            {
                i++;
                outdir = argv[i];
            }
            else if(strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--move") == 0)
            {
                move = true;
            }
            else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
            {
                print_help();
                return EXIT_SUCCESS;
            }
        }
    
    }
    std::vector<std::string> filesInFolder{getFilesInFolder(argv[1], recursive)};
    int loops{0}, j_buffer{0};
    
    
    for(int i{0}; i < (filesInFolder.size() / std::stoi(argv[2])) + 1; i++)
    {
        std::filesystem::create_directory(outdir +"/"+ std::to_string(i));
    }
    for(int i{0}; i < filesInFolder.size(); i++)
    {
        for(int j{0}; j < std::stoi(argv[2]); j++)
        {
            if((loops * std::stoi(argv[2])) + j >= filesInFolder.size())
            {
                break;
            }
            std::filesystem::copy_file(filesInFolder[loops * std::stoi(argv[2]) + j], outdir +"/"+ std::to_string(i) + "/" + cutFileName(filesInFolder[loops * std::stoi(argv[2]) + j]));
            if(move)
            {
                std::filesystem::remove_all(filesInFolder[loops * std::stoi(argv[2]) + j]);
            }
            j_buffer = j;
        }
        if(loops * std::stoi(argv[2]) + j_buffer >= filesInFolder.size())
        {
            break;
        }
        loops++;
    }
}