#include "StockFish.h"

#include "Defines.h"

#include "tinyproc/process.hpp"

#include <sstream>
#include <fstream>

#include <mutex>
#include <condition_variable>

#ifdef DEBUG
static int outCount = 0;

static int errCount = 0;

static std::ofstream out("out.txt");

static std::ofstream err("err.txt");
#endif // DEBUG
static std::mutex moveMtx;

static Move lastMove;

static bool moveRead = false;

static bool checkMate = false;

static TinyProcessLib::Process stockfish{
        "stockfish_20011801_x64.exe", "",
        [](const char* bytes, size_t n) {
            // todo read stdout

            std::string input(bytes);

            if(input.find("bestmove") != std::string::npos) {
                std::stringstream moveData(input);
                std::string move;

                // First string will be 'bestmove' which I dont care about
                moveData >> move;

                #ifdef DEBUG
                out << "\n<DEBUG>\n";
                out << move << " ";
                #endif // DEBUG

                // So two subsequent reads have to be made
                moveData >> move;

                #ifdef DEBUG
                out << move << " ";
                #endif // DEBUG

                {
                    const std::lock_guard<std::mutex> lock(moveMtx);

                    lastMove.from_string(move);
                    #ifdef DEBUG
                    out << lastMove.to_string() << " ";
                    #endif // DEBUG
                    moveRead = true;
                }
                #ifdef DEBUG
                out << "\n</DEBUG>\n";
                #endif // DEBUG
            }

            #ifdef DEBUG
                for(size_t i = 0; i < n; ++i) {
                    out << bytes[i];
                }
                out << std::endl;
                out << ++outCount << std::endl;
            #endif // DEBUG

        },
        [](const char* bytes, size_t n) {
            // TODO: process stderr
            #ifdef DEBUG
                for(size_t i = 0; i < n; ++i) {
                    err << bytes[i];
                }
                err << std::endl;
                err << ++errCount << std::endl;
            #endif // DEBUG
        },
        true
};


void StockFish::Init()
{
    stockfish.write("uci\n");

    stockfish.write("setoption name Hash value 32\n");

    stockfish.write("ucinewgame\n");

    stockfish.write("isready\n");

}

Move StockFish::GenMove(const std::vector<Move>& moves)
{
    std::string strmoves = "position startpos moves ";

    for(auto& a : moves) strmoves += a.to_string() + " ";

    strmoves += '\n';

    stockfish.write(strmoves);

    stockfish.write("go movetime 1000\n");

    #ifdef DEBUG
    out << "\n<DEBUG>\n";
    out << strmoves;
    out << "\n</DEBUG>\n";
    #endif // DEBUG

    std::this_thread::sleep_for(std::chrono::milliseconds(1010));

    {
        const std::lock_guard<std::mutex> lock(moveMtx);

        if(moveRead) {
            #ifdef DEBUG
            out << "\n<DEBUG>\n";
            out << lastMove.to_string();
            out << "\n</DEBUG>\n";
            #endif // DEBUG
            moveRead = false;
            return lastMove;
        } else {
            return {};
        }
    }
}

bool StockFish::CheckMate(const std::vector<Move>& moves)
{
    // TODO: Make engine check for a checkmate or a stallmate
}

int StockFish::Close()
{
    stockfish.write("quit");

    stockfish.kill();

    #ifdef DEBUG
    out.close();
    err.close();
    #endif // DEBUG

    return stockfish.get_exit_status();
}


