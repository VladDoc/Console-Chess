#include "Pawn.h"

#include <iostream>
#include <cmath>
#include <cstring>

#include "FigureFactory.h"

Pawn::Pawn()
{
    //ctor
}

Pawn::Pawn(int x, int y, bool col)
{
    // You cannot access protected members in init list,
    // hence this hack
    this->x_ = x;
    this->y_ = y;

    if(col) {
        this->color = Color::WHITE;
        name = "WP";
    } else {
        this->color = Color::BLACK;
        name = "BP";
    }
}

Pawn::~Pawn()
{
    //dtor
}

bool Pawn::ValidateMove(int x1, int y1,
                        int x2, int y2, const Field& f) const
{
    if(this->IsEmpty()) return false;

    const Figure& enemy = f.get(x2, y2);

    bool isFree = enemy.IsEmpty();

    // Improves readability quite a bit
    int checkStep = this->IsWhite() ? 1 : -1;

    int enPassantCheck = this->IsWhite() ? 4 : 3;

    Color enemyCol = enemy.GetColor();

    if(isFree) {
        if(x1 == x2) {

            if(y2 == y1 + checkStep) return true;

            if(this->firstMove && (y2 == y1 + checkStep * 2)) return true;
        } else {
            if(enPassantCheck == y1 &&
               (enPassantCheck + checkStep) == y2 && std::abs(x2 - x1) == 1) {

                Figure* enPassant = &f.get(x1+1, y1);
                if(enPassant->IsEmpty()) {
                    enPassant = &f.get(x1-1, y1);
                    if(enPassant->IsEmpty()) return false;
                }

                const char* enName = enPassant->GetName();

                Vector2D<int> src;
                Vector2D<int> dst;

                f.GetLastMove(src, dst);

                src = dst - src;

                if(this->color == Color::WHITE) {
                    if(std::strcmp(enName, "BP") != 0) return false;
                    if(src.y == -2 && src.x == 0) {
                        enPassant->SetEmpty();
                        return true;
                    }
                } else {
                    if(std::strcmp(enName, "WP") != 0) return false;
                    if(src.y == 2 && src.x == 0) {
                        enPassant->SetEmpty();
                        return true;
                    }
                }
            }
        }
        return false;

    } else {
        // Then attack
        if( y2 == y1 + checkStep &&
           (x2 == x1 + checkStep || x2 == x1 - checkStep) &&
            enemyCol != this->color) {
                return true;
        }

    }
    return false;
}

bool Pawn::Move(int x, int y, Field& f)
{
    if(x == this->x_ && y == this->y_) return false;
    if(ValidateMove(this->x_, this->y_, x, y, f))
    {
        f.get(x, y).SetEmpty();
        f.swap(this->x_, this->y_, x, y);

        this->firstMove = false;

        if(this->y_ == 7 || this->y_ == 0)
        {
            bool col = this->IsWhite();
            char input;
            std::string res = col ? "W" : "B";
            do {
                std::cout << "Pawn gets promoted!\n"
                          << "Choose a piece(R, N, B, Q, K):\n";
                std::cin >> input;

                res += input;

                std::unique_ptr<Figure> newF =
                    FigureFactory::create(res.c_str(), this->x_, this->y_);
                if(newF->IsEmpty() || newF->GetName()[1] == 'P') {
                    std::cout << "You must have misspelled. Try again\n";
                    continue;
                }

                newF = f.set(this->x_, this->y_, std::move(newF));
                break;
            } while(true);
        }

        return true;
    } else {
        return false;
    }
}
