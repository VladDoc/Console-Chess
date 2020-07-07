#include "StockFish.h"

#include "Defines.h"

#include "tinyproc/process.hpp"

#include <sstream>
#include <fstream>

#include <atomic>

#ifdef DEBUG
static int outCount = 0;

static int errCount = 0;

static std::ofstream out("out.txt");

static std::ofstream err("err.txt");
#endif // DEBUG

static Move lastMove;

static std::atomic<bool> moveRead{false};

static std::atomic<bool> checkMate{false};

static TinyProcessLib::Process stockfish{
        "stockfish_20011801_x64.exe", "",
        [](const char* bytes, size_t n) {
            // todo read stdout

            std::string input(bytes);

            size_t mate = input.find("mate 1");

            if(mate != std::string::npos)
            {
                checkMate = true;
            }

            size_t data_start = input.find("bestmove");

            if(data_start != std::string::npos) {
                std::stringstream moveData(input.substr(data_start));
                std::string move;

                // First string will be 'bestmove' which I dont care about
                moveData >> move;

                #ifdef DEBUG
                out << "\n<ReadStdout>\n";
                out << move << " ";
                #endif // DEBUG

                // So two subsequent reads have to be made
                moveData >> move;

                #ifdef DEBUG
                out << move << " ";
                #endif // DEBUG


                lastMove.from_string(move);
                #ifdef DEBUG
                out << lastMove.to_string() << " ";
                out << lastMove.from.x << " "
                    << lastMove.from.y << " "
                    << lastMove.to.x << " "
                    << lastMove.to.y << " "
                    << lastMove.promotion << " ";

                #endif // DEBUG
                moveRead = true;

                #ifdef DEBUG
                out << "\n</ReadStdout>\n";
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

    if(moveRead) {
        #ifdef DEBUG
        out << "\n<GenMove>\n";
        out << lastMove.to_string();
        out << lastMove.from.x << " "
            << lastMove.from.y << " "
            << lastMove.to.x << " "
            << lastMove.to.y << " "
            << lastMove.promotion << " ";
        out << "\n</GenMove>\n";
        #endif // DEBUG
        moveRead = false;
        return lastMove;
    } else {
        return {};
    }
}

bool StockFish::CheckMate()
{
    return checkMate;
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


