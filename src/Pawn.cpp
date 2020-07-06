#include "Pawn.h"

#include <iostream>
#include <cmath>
#include <cstring>

#include "FigureFactory.h"
#include "Move.h"

#ifndef MOVE_H_INCLUDED
#error "Kakogo"
#endif // MOVE_H_INCLUDED

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

            if(this->firstMove && (y2 == y1 + checkStep * 2) &&
               f.get(x1, y1 + checkStep).IsEmpty()) return true;

        } else {
            if(enPassantCheck == y1 &&
               (enPassantCheck + checkStep) == y2 && std::abs(x2 - x1) == 1) {

                Figure* enPassant = &f.get(x1+1, y1);
                if(enPassant->IsEmpty()) {
                   enPassant = &f.get(x1-1, y1);
                    if(enPassant->IsEmpty()) return false;
                }

                const char* enName = enPassant->GetName();

                // somehow I got a collision hence this ugly 'struct' hack
                struct Move move = f.GetLastMove();

                Vector2D<int>& src = move.from;
                Vector2D<int>& dst = move.to;

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
