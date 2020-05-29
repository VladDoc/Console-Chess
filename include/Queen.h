#ifndef QUEEN_H
#define QUEEN_H


#include "Figure.h"

class Queen : public Figure
{
    public:
        Queen();
        Queen(int x, int y, bool col);
        virtual ~Queen();
        virtual bool ValidateMove(int x1, int y1,
                                  int x2, int y2, const Field& f) const override;
    protected:

    private:
};

#endif // QUEEN_H
