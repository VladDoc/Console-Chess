#include "Knight.h"

#include <cmath>

Knight::Knight()
{
    this->SetEmpty();
}

Knight::~Knight()
{
    //dtor
}

Knight::Knight(int x, int y, bool col)
{
    this->x_ = x;
    this->y_ = y;

    if(col) {
        this->name = "WN";
        this->color = Color::WHITE;
    } else {
        this->name = "BN";
        this->color = Color::BLACK;
    }
}

bool Knight::ValidateMove(int x1, int y1,
                          int x2, int y2, const Field& f) const
{
    bool empty = f.get(x2, y2).IsEmpty();
    bool diff_color = f.get(x2, y2).GetColor() != this->GetColor();

    if(empty || diff_color) {
        if(std::abs(y2 - y1) == 2 && std::abs(x2 - x1) == 1) {
            return true;
        }

        if(std::abs(y2 - y1) == 1 && std::abs(x2 - x1) == 2) {
            return true;
        }
    }
    return false;
}
