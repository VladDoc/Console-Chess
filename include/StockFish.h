#ifndef STOCKFISH_H
#define STOCKFISH_H

#include <vector>

#include "Move.h"

class StockFish
{
    public:
        static void Init();

        static Move GenMove(const std::vector<Move>& moves);

        static bool CheckMate();

        static int Close();

};

#endif // STOCKFISH_H
