#include "FileUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string ReadFileToString(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::in);
    if (!file.is_open()) 
    {
        std::cerr << "ERROR: Could not open file: " << filepath << std::endl;
        return std::string();
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}