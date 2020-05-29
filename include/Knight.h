#ifndef KNIGHT_H
#define KNIGHT_H

#include "Figure.h"

class Knight : public Figure
{
    public:
        Knight();
        Knight(int x, int y, bool col);
        virtual ~Knight();
        virtual bool ValidateMove(int x1, int y1,
                                  int x2, int y2, const Field& f) const override;
    protected:

    private:
};

#endif // KNIGHT_H
