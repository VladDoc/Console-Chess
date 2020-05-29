#ifndef UTILITYBS_H
#define UTILITYBS_H

#include <string>
#include <functional>

#include "Vector2D.h"

namespace UtilityBS
{
    void StringToLower(std::string&);
    void StringToUpper(std::string&);
    void StringToWhatever(std::string&, std::function<char(char)>&);
    void ClearScreen();

    Vector2D<int> ChessStringToCoods(const std::string& str);

    void Pause();

    // removes whitespace characters from beginning and end of string s
    std::string trim(const std::string& s);
}

#endif // UTILITYBS_H
