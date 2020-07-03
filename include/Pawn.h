#ifndef PAWN_H
#define PAWN_H

#include "Figure.h"

class Pawn : public Figure
{
    public:
        Pawn();
        Pawn(int x, int y, bool col);
        virtual ~Pawn();
        virtual bool ValidateMove(int x1, int y1,
                                  int x2, int y2, const Field& f) const override;
    protected:

    private:
};

#endif // PAWN_H
