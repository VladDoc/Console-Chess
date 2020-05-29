#ifndef ROOK_H
#define ROOK_H


#include "Figure.h"

class Rook : public Figure
{
    public:
        Rook();
        Rook(int x, int y, bool col);
        virtual ~Rook();
        virtual bool ValidateMove(int x1, int y1,
                                  int x2, int y2, const Field& f) const override;
    protected:

    private:
};

#endif // ROOK_H
