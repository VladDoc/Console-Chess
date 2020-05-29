#include "Bishop.h"


#include <cmath>

Bishop::Bishop()
{
    this->SetEmpty();
}

Bishop::~Bishop()
{
    //dtor
}

Bishop::Bishop(int x, int y, bool col)
{
    this->x_ = x;
    this->y_ = y;

    if(col) {
        this->name = "WB";
        this->color = Color::WHITE;
    } else {
        this->name = "BB";
        this->color = Color::BLACK;
    }
}

bool Bishop::ValidateMove(int x1, int y1,
                          int x2, int y2, const Field& f) const
{
    bool empty = f.get(x2, y2).IsEmpty();
    bool diff_color = f.get(x2, y2).GetColor() != this->GetColor();

    Vector2D<int> diff;

    diff.x = x2 - x1;
    diff.y = y2 - y1;

    if((empty || diff_color) && diff.x != 0 && diff.y != 0) {
        if(f.IsPathClear({x1, y1}, {x2, y2})) return true;
    }
    return false;
}
