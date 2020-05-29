#include "Queen.h"

#include <cmath>

Queen::Queen()
{
    this->SetEmpty();
}

Queen::~Queen()
{
    //dtor
}

Queen::Queen(int x, int y, bool col)
{
    this->x_ = x;
    this->y_ = y;

    if(col) {
        this->name = "WQ";
        this->color = Color::WHITE;
    } else {
        this->name = "BQ";
        this->color = Color::BLACK;
    }
}

bool Queen::ValidateMove(int x1, int y1,
                         int x2, int y2, const Field& f) const
{
    bool empty = f.get(x2, y2).IsEmpty();
    bool diff_color = f.get(x2, y2).GetColor() != this->GetColor();

    if((empty || diff_color)) {
        if(f.IsPathClear({x1, y1}, {x2, y2})) return true;
    }
    return false;
}

