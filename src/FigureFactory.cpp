#include "FigureFactory.h"

#include <cctype>

#include "Pawn.h"
#include "Knight.h"
#include "King.h"
#include "Rook.h"
#include "Bishop.h"
#include "Queen.h"

std::unique_ptr<Figure> FigureFactory::create(const char* name, int x, int y)
{
    bool color;
    if(std::toupper(name[0]) == 'W') {
        color = true;
    } else if (std::toupper(name[0]) == 'B') {
        color = false;
    } else {
        return std::make_unique<Pawn>(Pawn{});
    }

    switch(std::toupper(name[1])) {
        case 'P':
            return std::make_unique<Pawn>(Pawn{x, y, color});
        case 'N':
            return std::make_unique<Knight>(Knight{x, y, color});
        case 'R':
            return std::make_unique<Rook>(Rook{x, y, color});
        case 'Q':
            return std::make_unique<Queen>(Queen{x, y, color});
        case 'K':
            return std::make_unique<King>(King{x, y, color});
        case 'B':
            return std::make_unique<Bishop>(Bishop{x, y, color});
        default:
            return std::make_unique<Pawn>(Pawn{});
    }
}
