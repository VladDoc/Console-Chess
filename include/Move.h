#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include <string>

#include "Vector2D.h"

struct Move
{
    Vector2D<int> from{-1, -1};
    Vector2D<int> to{-1, -1};

    char promotion = '\0'; // '\0' is non promoting

    Move() = default;
    Move(const std::string&);

    std::string to_string() const;

    void from_string(const std::string&);

    bool IsInvalid();
};

#endif // MOVE_H_INCLUDED
