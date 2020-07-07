#include <iostream>
#include <sstream>
#include <iterator>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <cassert>

#include "Field.h"

#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include "UtilityBS.h"
#include "FigureFactory.h"
#include "StockFish.h"

Field::Field()
{
}

Field::~Field()
{
    //dtor
}

Figure& Field::get(int x, int y) const
{
    return *this->field_[y][x];
}

std::unique_ptr<Figure> Field::set(int x, int y, std::unique_ptr<Figure>&& f)
{
    this->field_[y][x].swap(f);

    return std::move(f);
}

void Field::swap(int x1, int y1, int x2, int y2)
{
    field_[y1][x1]->SetCoords(x2, y2);
    field_[y2][x2]->SetCoords(x1, y1);
    std::swap(field_[y1][x1], field_[y2][x2]);
}

void Field::Init()
{
    field_.reserve(8);
    for(int i = 0; i < 1; ++i) {
        field_.emplace_back(
                    std::vector<std::unique_ptr<Figure>>{}
                );
        field_[i].reserve(8);
        for(int j = 0; j < 8; ++j) {
            switch(j)
            {
                case 0:
                case 7:
                    field_[i].emplace_back(std::make_unique<Rook>(Rook{j, i, true}));
                break;
                case 1:
                case 6:
                    field_[i].emplace_back(std::make_unique<Knight>(Knight{j, i, true}));
                break;
                case 2:
                case 5:
                    field_[i].emplace_back(std::make_unique<Bishop>(Bishop{j, i, true}));
                break;
                case 3:
                    field_[i].emplace_back(std::make_unique<Queen>(Queen{j, i, true}));
                break;
                case 4:
                    field_[i].emplace_back(std::make_unique<King>(King{j, i, true}));
                    this->whiteKing = &this->get(j, i);
                break;
            }
        }
    }
    for(int i = 1; i < 2; ++i) {
        field_.emplace_back(
                    std::vector<std::unique_ptr<Figure>>{}
                );
        field_[i].reserve(8);
        for(int j = 0; j < 8; ++j) {
            field_[i].emplace_back(std::make_unique<Pawn>(Pawn{j, i, true}));
        }
    }

    for(int i = 2; i < 6; ++i) {
        field_.emplace_back(
                    std::vector<std::unique_ptr<Figure>>{}
                );
        field_[i].reserve(8);
        for(int j = 0; j < 8; ++j) {
            field_[i].emplace_back(std::make_unique<Pawn>(Pawn{}));
        }
    }

    for(int i = 6; i < 7; ++i) {
        field_.emplace_back(
                    std::vector<std::unique_ptr<Figure>>{}
                );
        field_[i].reserve(8);
        for(int j = 0; j < 8; ++j) {
            field_[i].emplace_back(std::make_unique<Pawn>(Pawn{j, i, false}));
        }
    }
    for(int i = 7; i < 8; ++i) {
        field_.emplace_back(
                    std::vector<std::unique_ptr<Figure>>{}
                );
        field_[i].reserve(8);
        for(int j = 0; j < 8; ++j) {
            switch(j)
            {
                case 0:
                case 7:
                    field_[i].emplace_back(std::make_unique<Rook>(Rook{j, i, false}));
                break;
                case 1:
                case 6:
                    field_[i].emplace_back(std::make_unique<Knight>(Knight{j, i, false}));
                break;
                case 2:
                case 5:
                    field_[i].emplace_back(std::make_unique<Bishop>(Bishop{j, i, false}));
                break;
                case 3:
                    field_[i].emplace_back(std::make_unique<Queen>(Queen{j, i, false}));
                break;
                case 4:
                    field_[i].emplace_back(std::make_unique<King>(King{j, i, false}));
                    this->blackKing = &this->get(j, i);
                break;
            }
        }
    }

    this->SuggestAI();

    static bool testsRan;
    // So calling Init() again won't cause an assertion fail
    if(!testsRan) this->Tests();
    testsRan = true;
}
void Field::Update(const UpdateData&)
{
    if(check) {
        std::cout << "CHECK!\n";
        currentMove ? std::cout << "White King must defend himself\n" :
                      std::cout << "Black King must defend himself\n";
        check = false;
    }

    std::cout << "M to move\n"
              << "S to save\n"
              << "L to load\n"
              << "R to restart\n"
              << "RM to restart this move\n"
              << "Q to call it quits\n";
    do {
        std::string input_;

        std::cout << "> ";

        std::cin >> input_;


        if(input_.empty()) continue;

        std::istringstream iss(input_);

        using is_str_it = std::istream_iterator<std::string>;

        std::vector<std::string> parsed(is_str_it{iss}, is_str_it());

        UtilityBS::StringToLower(parsed[0]);

        if(parsed.size() == 0) {
            std::cout << "Wrong input. Try again\n";
            UtilityBS::Pause();
            continue;
        }

        if(parsed[0].compare("m") == 0) {
            std::string str;

            Vector2D<int> src;
            Vector2D<int> dst;
            char promote = '\0';


            if(!againstAI || !currentMove) {
                std::cout << "src: ";
                std::cin >> str;

                src = UtilityBS::ChessStringToCoods(str);
                if(src.x < 0) {
                    std::cout << "Input string must be in such format:\n"
                              << "E2 or e2\n";
                    UtilityBS::Pause();
                    continue;
                }

                if(field_[src.y][src.x]->IsEmpty() ||
                   field_[src.y][src.x]->IsWhite() != currentMove) {
                    std::cout << "Illegal Piece.\n";
                    UtilityBS::Pause();
                    continue;
                }
                std::cout << "dst: ";
                std::cin >> str;

                dst = UtilityBS::ChessStringToCoods(str);
                if(dst.x < 0) {
                    std::cout << "Input string must be in such format:\n"
                              << "E2 or e2\n";
                    UtilityBS::Pause();
                    continue;
                }
            } else {
                Move move = StockFish::GenMove(this->moves);
                if(move.IsInvalid()) {
                    std::cout << "Error: Could not generate a move\n";
                    UtilityBS::Pause();
                } else {
                    src = move.from;
                    dst = move.to;
                    promote = move.promotion;
                }
            }

            Figure* piece = &this->get(src.x, src.y);
            Figure* enemy = &this->get(dst.x, dst.y);



            bool wasntEmpty = !enemy->IsEmpty();
            bool wasntSameColor = this->get(src.x, src.y).GetColor() !=
                                  enemy->GetColor();


            // In most cases dynamic cast won't be executed
            bool promotionFlag = ((dst.y == 0 || dst.y == 7) &&
                                  enemy->IsEmpty() &&
                                  dynamic_cast<Pawn*>(piece) != nullptr);


            if(!field_[src.y][src.x]->Move(dst.x, dst.y, *this)) {
                std::cout << "Invalid Move\n";
                UtilityBS::Pause();
                continue;
            }

            if(promote != '\0') {
                std::string fig = piece->IsWhite() ? "W" : "B";
                fig += promote;
                this->set(dst.x, dst.y,
                          FigureFactory::create(fig.c_str(), dst.x, dst.y));
            } else if(promotionFlag) {
                promote = this->Promote(dst.x, dst.y);
            }

            if(currentMove) {
                if(this->KingInDanger(*whiteKing)) {
                    dynamic_cast<King*>(whiteKing)->setThreatened(true);
                    if(CheckMate(*whiteKing) || checkMateCounter == 5) {
                        std::cout << "Checkmate. Black won!\n";
                        this->done = true;
                        UtilityBS::Pause();
                        return;
                    }
                    std::cout << "Illegal Move: King is Under Attack\n";
                    this->swap(src.x, src.y, dst.x, dst.y);
                    UtilityBS::Pause();
                    checkMateCounter += 1;
                    continue;
                } else if(this->KingInDanger(*blackKing)) {
                    dynamic_cast<King*>(blackKing)->setThreatened(true);
                    this->check = true;
                }
            } else {
                if(this->KingInDanger(*blackKing)) {
                dynamic_cast<King*>(blackKing)->setThreatened(true);
                    if(CheckMate(*blackKing) || checkMateCounter == 5) {
                        std::cout << "Checkmate. White won!\n";
                        this->done = true;
                        UtilityBS::Pause();
                        return;
                    }
                    std::cout << "Illegal Move: King is Under Attack\n";
                    this->swap(src.x, src.y, dst.x, dst.y);
                    UtilityBS::Pause();
                    checkMateCounter += 1;
                    continue;
                } else if(this->KingInDanger(*whiteKing)) {
                    dynamic_cast<King*>(whiteKing)->setThreatened(true);
                    this->check = true;
                }
            }

            if(wasntEmpty && wasntSameColor) {
                currentMove ? ++capturedBlack : ++capturedWhite;
            }


            this->lastMove.from = src;
            this->lastMove.to = dst;
            this->lastMove.promotion = promote;

            this->moves.push_back(lastMove);

            currentMove = !currentMove;
            checkMateCounter = 0;

        } else if(parsed[0].compare("s") == 0) {
            std::string path;

            std::cout << "Input filename: ", std::cin >> path;
            std::ofstream out(path.c_str());

            this->output(out);

            if(!out) std::cout << "Failed to save.\n";
            else std::cout << "Saved.\n";

            std::cin.ignore();
            UtilityBS::Pause();

            out.close();
        } else if(parsed[0].compare("l") == 0) {
            std::string path;

            std::cout << "Input filename: ", std::cin >> path;
            std::ifstream in(path.c_str());

            bool success = this->input(in);

            if(!in.good() && !success) std::cout << "Failed to load.\n";
            else std::cout << "Loaded.\n";

            this->SuggestAI();

            std::cin.ignore();
            UtilityBS::Pause();

            in.close();
        } else if(parsed[0].compare("r") == 0) {
            this->restart = true;
            return;
        } else if(parsed[0].compare("rm") == 0) {
            return;
        } else if(parsed[0].compare("q") == 0) {
            std::string answer;
            std::cout << "Are you sure?(y, n): ", std::cin >> answer;

            UtilityBS::StringToLower(answer);

            if(answer == "y"   ||
               answer == "yes" ||
               answer == "yez" ||
               answer == "yea" ||
               answer == "yep" ||
               answer == "yeah") {
                    this->done = true;
                    return;
            } else {
                UtilityBS::Pause();
                continue;
            }
        } else {
            std::cout << "Wrong input. Try again\n";
            UtilityBS::Pause();
            continue;
        }


        break;
    } while(true);
}
void Field::Draw(const UpdateData& data) const
{
    this->output(std::cout);
}
void Field::Close()
{
    this->field_.clear();
}

const Move& Field::GetLastMove() const
{
    return this->lastMove;
}

bool Field::IsPathClear(const Vector2D<int>& src,
                        const Vector2D<int>& dst) const
{
    Vector2D<int> dist;
    dist.x = dst.x - src.x;
    dist.y = dst.y - src.y;

    // if aspect ratio isn't 1:0, 1:1 or 0:1
    // Then it must be an illegal move
    if(dist.x != 0 && dist.y != 0 &&
       !(std::abs(dist.x) == std::abs(dist.y))) return false;

    Vector2D<int> counter = src;
    Vector2D<int> step;

    if(dist.x > 0) step.x = 1;
    else if(dist.x < 0) step.x = -1;
    else step.x = 0;

    if(dist.y > 0) step.y = 1;
    else if(dist.y < 0) step.y = -1;
    else step.y = 0;

    counter += step;

    while(counter != dst) {
        if(!this->get(counter.x, counter.y).IsEmpty()) {
            return false;
        }
       counter += step;
    }

    return true;
}

void Field::Tests()
{
    // Check path traversal
    assert(this->IsPathClear({4,3}, {3,3}) == true);
    assert(this->IsPathClear({3,4}, {3,3}) == true);
    assert(this->IsPathClear({3,3}, {3,3}) == true);
    assert(this->IsPathClear({3,3}, {5,2}) == false);
    assert(this->IsPathClear({3,3}, {5,5}) == true);

    // Check basic Pawn behavior
    assert(this->get(1, 1).ValidateMove(1, 1, 1, 3, *this) == true);
    assert(this->get(1, 1).ValidateMove(1, 1, 1, 2, *this) == true);
    assert(this->get(1, 1).ValidateMove(1, 1, 2, 5, *this) == false);

    // Check basic Knight behavior
    assert(this->get(1, 0).ValidateMove(1, 0, 2, 2, *this) == true);
    assert(this->get(1, 0).ValidateMove(1, 0, 0, 2, *this) == true);
    assert(this->get(1, 0).ValidateMove(1, 0, 0, 1, *this) == false);
    assert(this->get(1, 0).ValidateMove(1, 0, 3, 1, *this) == false);

    // Check basic Bishop behavior
    this->get(3, 1).SetEmpty();
    this->get(3, 6).SetEmpty();
    assert(this->get(2, 0).ValidateMove(2, 0, 3, 1, *this) == true);
    assert(this->get(2, 0).ValidateMove(2, 0, 4, 2, *this) == true);
    assert(this->get(2, 0).ValidateMove(2, 0, 5, 3, *this) == true);
    assert(this->get(2, 0).ValidateMove(2, 0, 6, 4, *this) == true);

    assert(this->get(2, 0).ValidateMove(2, 0, 3, 3, *this) == false);
    assert(this->get(2, 0).ValidateMove(2, 0, 4, 1, *this) == false);

    assert(this->get(2, 7).ValidateMove(2, 7, 3, 6, *this) == true);

    this->get(3, 6) = Pawn{3, 6, false};
    this->get(3, 1) = Pawn{3, 1, true};

    // Check KingInDanger
    field_[3][3] = std::make_unique<King>(King{3, 3, false});
    field_[3][5] = std::make_unique<Queen>(Queen{5, 3, true});

    assert(KingInDanger(this->get(3, 3)) == true);

    this->get(3, 3).SetEmpty();
    this->get(5, 3).SetEmpty();


    Move move;

    move.from_string("e8g8");

    assert(move.from.x == 4);
    assert(move.from.y == 0);

    assert(move.to.x == 6);
    assert(move.to.y == 0);

    assert(move.promotion == '\0');
}


bool Field::CheckMate(const Figure& king) const
{
//    Color color = king.IsWhite() ? Color::BLACK : Color::WHITE;
//    Vector2D<int> dst;
//    king.GetCoords(dst.x, dst.y);
//
//    bool isAlive = true;
//
//    for(int i = -1; dst.y + i < 8; ++i) {
//        for(int j = -1; dst.x + j < 8; ++j) {
//            if(j == 0 && i == 0) continue;
//            if(dst.x - j < 0 || dst.y - i < 0) continue;
//            if(king.ValidateMove(dst.x, dst.y, dst.x + j, dst.y + i, *this))
//              isAlive &= !KingInDanger(color, {dst.x + j, dst.y + i});
//        }
//    }
//
//    return !isAlive;

     return StockFish::CheckMate();
}

bool Field::KingInDanger(Color color, const Vector2D<int>& dst) const
{
    for(size_t i = 0; i < field_.size(); ++i) {
        for(size_t j = 0; j < field_[0].size(); ++j) {
            auto& a = field_[i][j];
            if(a->GetColor() != color) continue;
            Vector2D<int> aCoords;
            a->GetCoords(aCoords.x, aCoords.y);
            if(a->ValidateMove(aCoords.x, aCoords.y, dst.x, dst.y, *this))
                return true;
        }
    }

    return false;
}

bool Field::KingInDanger(const Figure& king) const
{

    Color color = king.IsWhite() ? Color::BLACK : Color::WHITE;
    Vector2D<int> dst;
    king.GetCoords(dst.x, dst.y);

    return KingInDanger(color, dst);
}


void Field::output(std::ostream& where) const
{
    where << "    a   b   c   d   e   f   g   h \n";
    where << "  +---+---+---+---+---+---+---+---+\n";
    for(size_t i = 0; i < field_.size(); ++i) {
        where << " " << 8 - i << "|";
        for(size_t j = 0; j < field_[0].size(); ++j) {
            where << " ";
            field_[i][j]->output(where);
            where << "|";
        }
        where << 8 - i;
        where << "\n  +---+---+---+---+---+---+---+---+\n";
    }
    where << "    a   b   c   d   e   f   g   h \n\n";
    where << (currentMove ? "Move of white\n" : "Move of black\n");

    where << "White pieces taken: " << this->capturedWhite << '\n';
    where << "Black pieces taken: " << this->capturedBlack << '\n';

    where << "All moves:\n";

    for(auto& a : this->moves) {
        where << a.to_string();
    }
    where << "\n";
}


bool Field::input(std::istream& from)
{
    this->field_.clear();

    std::string line;
    std::string piece;

    std::getline(from, line, '\n');
    std::getline(from, line, '\n');
    char cbuf;
    int  ibuf;

    int whiteC = 0;
    int blackC = 0;

    this->field_.reserve(8);
    for(int i = 0; i < 8; ++i) {
        field_.emplace_back(
                    std::vector<std::unique_ptr<Figure>>{}
                );
        field_[i].reserve(8);
        from >> ibuf >> cbuf;
        for(int j = 0; j < 8; ++j) {
             std::getline(from, piece, '|');
             if(piece != "  ") {
                piece = UtilityBS::trim(piece);
             }
             field_[i].emplace_back(FigureFactory::create(piece.c_str(), j, i));
             if(field_[i][j]->IsWhite()) {
                ++whiteC;
                if(piece[1] == 'K') whiteKing = field_[i][j].get();
             }
             if(field_[i][j]->IsBlack()) {
                 ++blackC;
                if(piece[1] == 'K') blackKing = field_[i][j].get();
             }
        }
        from >> ibuf;
        std::getline(from, line, '\n');
        std::getline(from, line, '\n');
    }

    std::getline(from, line, '\n');
    std::getline(from, line, '\n');
    std::getline(from, line, '\n');

    if(line == "Move of white") {
        this->currentMove = true;
    }

    if(line == "Move of black") {
        this->currentMove = false;
    }

    std::getline(from, line, '\n');
    std::getline(from, line, '\n');

    this->capturedWhite = 16 - whiteC;
    this->capturedBlack = 16 - blackC;

    std::getline(from, line, '\n');
    this->moves.clear();

    if(whiteC == 0 || blackC == 0 || !from.good()) return false;

    while(!from.eof() && from >> line) moves.emplace_back(line);

    return true;
}

char Field::Promote(int x, int y)
{
    bool col = this->get(x, y).IsWhite();
    char input_;
    std::string res = col ? "W" : "B";
    do {
        std::cout << "Pawn gets promoted!\n"
                  << "Choose a piece(R, N, B, Q, K):\n";
        std::cin >> input_;

        res += input_;

        std::unique_ptr<Figure> newF =
            FigureFactory::create(res.c_str(), x, y);

        if(newF->IsEmpty() || newF->GetName()[1] == 'P') {
            std::cout << "You must have misspelled. Try again\n";
            continue;
        }

        newF = this->set(x, y, std::move(newF));
        break;
    } while(true);

    return std::tolower(input_);
}


void Field::SuggestAI()
{
    std::cout << "Would you like to play with an AI(y, n)?" << std::endl;
    std::string answer;

    std::cin >> answer;

    if(answer == "y"   ||
       answer == "yes" ||
       answer == "yez" ||
       answer == "yea" ||
       answer == "yep" ||
       answer == "yeah") {
        this->againstAI = true;
    } else {
        this->againstAI = false;
    }
}
