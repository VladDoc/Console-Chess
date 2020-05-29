#ifndef BISHOP_H
#define BISHOP_H


#include "Figure.h"

class Bishop : public Figure
{
    public:
        Bishop();
        Bishop(int x, int y, bool col);
        virtual ~Bishop();
        virtual bool ValidateMove(int x1, int y1,
                                  int x2, int y2, const Field& f) const override;
    protected:

    private:
};

#endif // BISHOP_H
