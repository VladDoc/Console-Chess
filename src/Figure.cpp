#include "Figure.h"

#include <cstring>
#include <iostream>


Figure::~Figure()
{

}

bool Figure::IsWhite() const
{
    return this->color == Color::WHITE;
}

bool Figure::IsBlack() const
{
    return this->color == Color::BLACK;
}

bool Figure::IsEmpty() const
{
    return (this->name == "  ") ||
           this->color == Color::UNKNOWN ||
           this->x_ == -1 ||
           this->y_ == -1;
}

void Figure::SetEmpty()
{
    this->name = "  ";
    this->color = Color::UNKNOWN;
    this->x_ = -1;
    this->y_ = -1;
}

void Figure::SetCoords(int x, int y)
{
    this->x_ = x;
    this->y_ = y;
}

bool Figure::Move(int x, int y, Field& f)
{
    if(x == this->x_ && y == this->y_) return false;
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

Color Figure::GetColor() const
{
    return this->color;
}

const char* Figure::GetName() const
{
    return this->name.c_str();
}

void Figure::Draw(const UpdateData& data) const
{
    this->output(std::cout);
}

void Figure::GetCoords(int& x, int& y) const
{
    x = this->x_;
    y = this->y_;
}


void Figure::output(std::ostream& where) const
{
    where << this->name;
}
