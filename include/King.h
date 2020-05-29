#ifndef KING_H
#define KING_H


#include "Figure.h"

class Field;

class King : public Figure
{
    public:
        King();
        King(int x, int y, bool col);
        virtual ~King();
        virtual bool ValidateMove(int x1, int y1,
                                  int x2, int y2, const Field& f) override;
    protected:

    private:
};

#endif // KING_H
