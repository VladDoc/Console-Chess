#include "Move.h"

static char intToChar(int val, std::string dict, char def = '0')
{
    if(val < 0 || val >= dict.size()) return def;

    return dict[val];
}

static int charToInt(char val, std::string dict, int def = -1)
{
    for(size_t i = 0; i < dict.size(); ++i)
        if(val == dict[i]) return i;

    return def;
}

static const std::string letters = "abcdefgh";

static const std::string numbersRev = "87654321";

std::string Move::to_string() const
{
    std::string str;

    str += intToChar(this->from.x, letters);

    str += intToChar(this->from.y, numbersRev);

    str += intToChar(this->to.x, letters);

    str += intToChar(this->to.y, numbersRev);

    if(promotion != '\0') str += promotion;
    else str += ' ';

    return str;
}

void Move::from_string(const std::string& str)
{
    if(str.size() < 4 || str.size() > 5) return;

    this->from.x = charToInt(str[0], letters);

    this->from.y = charToInt(str[1], numbersRev);

    this->to.x = charToInt(str[2], letters);

    this->to.y = charToInt(str[3], numbersRev);

    if(str.size() == 5) this->promotion = str[4];
}

Move::Move(const std::string& str)
{
    this->from_string(str);
}

bool Move::IsInvalid()
{
    return this->from.x == -1 || this->from.y == -1 ||
             this->to.x == -1 || this->to.x == -1;
}
