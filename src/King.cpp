#include "King.h"

#include <cmath>
#include <cstring>
#include <iostream>

King::King()
{
    this->SetEmpty();
}

King::~King()
{
    //dtor
}

King::King(int x, int y, bool col)
{
    this->x_ = x;
    this->y_ = y;

    if(col) {
        this->name = "WK";
        this->color = Color::WHITE;
    } else {
        this->name = "BK";
        this->color = Color::BLACK;
    }
}

bool King::ValidateMove(int x1, int y1,
                        int x2, int y2, const Field& f) const
{
    bool empty = f.get(x2, y2).IsEmpty();
    bool diff_color = f.get(x2, y2).GetColor() != this->GetColor();

    Vector2D<int> diff;
    diff.x = std::abs(x2 - x1);
    diff.y = std::abs(y2 - y1);

    if((empty || diff_color) && (diff.x <= 1 && diff.y <= 1)) {
        if(f.IsPathClear({x1, y1}, {x2, y2})) return true;
    }
    return false;
}

bool King::Move(int x, int y, Field& f)
{
    if(x == this->x_ && y == this->y_) return false;

    bool col = this->IsWhite();
    const char* rook = col ? "WR" : "BR";

    Vector2D<int> oldCoords{this->x_, this->y_};
    Vector2D<int> dist{x - this->x_, y - this->y_};
    if(std::strcmp(f.get(x, y).GetName(), rook) == 0 &&
       !f.get(x, y).IsEmpty() &&  this->firstMove &&
        f.get(x, y).GetFirstMove() && !this->wasThreatened &&
        f.IsPathClear(oldCoords, {x, y}))
    {
        // You thought this is it?
        // No, now you've got to check that the king won't get under attack
        // on the cells that he's passing through.

        int step = dist.x > 0 ? 1 : -1;

        this->SetCoords(this->x_ + step, this->y_);
        if(!f.KingInDanger(*this)) {
            this->SetCoords(this->x_ + step, this->y_);
            if(!f.KingInDanger(*this)) {
                std::cout << "Castling successful\n";
                this->SetCoords(oldCoords.x, oldCoords.y);
                f.swap(oldCoords.x, oldCoords.y, oldCoords.x + step * 2, oldCoords.y);
                if(step < 0) f.swap(0, y, 3, y);
                else f.swap(7, y, 5, y);
                return true;
            }
        }

        this->SetCoords(oldCoords.x, oldCoords.y);
        std::cout << "Castling failed. King would have been threatened.\n";
        return false;
    }

    if(ValidateMove(this->x_, this->y_, x, y, f))
    {
        f.get(x, y).SetEmpty();
        f.swap(this->x_, this->y_, x, y);

        this->firstMove = false;

        return true;
    } else {
        return false;
    }
}

