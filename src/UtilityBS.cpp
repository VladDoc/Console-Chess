#include "UtilityBS.h"

#include <algorithm>

#include <sstream>

#include <iostream>

void UtilityBS::StringToLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](char c){ return std::tolower(c); });
}

void UtilityBS::StringToUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](char c){ return std::toupper(c); });
}

void UtilityBS::StringToWhatever(std::string& str,
                                 std::function<char(char)>& func)
{
    std::transform(str.begin(), str.end(), str.begin(), func);
}

void UtilityBS::ClearScreen()
{
    #if defined _WIN32
        system("cls");
    #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        system("clear");
    #elif defined (__APPLE__)
        system("clear");
    #endif
}

Vector2D<int> UtilityBS::ChessStringToCoods(const std::string& str)
{
    // Both values are guarantied to be in non valid range
    Vector2D<int> coords;

    if(str.size() != 2) return {-1, -1};

    char buf = std::tolower(str[0]);
    // ASCII codes of a through h are in 97..104 range
    if(buf < 97 || buf > 104) return {-1, -1};
    coords.x = buf - 97;

    buf = str[1];


    if(buf < 49 || buf > 56) return {-1, -1};
    coords.y = 7 - (buf - 49);

    return coords;
}


void UtilityBS::Pause()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string UtilityBS::trim(const std::string& s)
{
    const int l = (int)s.length();
    int a=0, b=l-1;
    char c;
    while(a<l && ((c=s.at(a))==' '||c=='\t'||c=='\n'||c=='\v'||c=='\f'||c=='\r'||c=='\0')) a++;
    while(b>a && ((c=s.at(b))==' '||c=='\t'||c=='\n'||c=='\v'||c=='\f'||c=='\r'||c=='\0')) b--;
    return s.substr(a, 1+b-a);
}
