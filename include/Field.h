#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <memory>
#include <deque>

#include "GameObject.h"
#include "Vector2D.h"
#include "Move.h"

class Figure;
enum class Color : uint8_t;


class Field : public GameObject
{
    public:
        Field();
        virtual ~Field();
        virtual void Init() override;
        virtual void Update(const UpdateData&) override;
        virtual void Draw(const UpdateData&) const override;
        virtual void Close() override;

        virtual Figure& get(int x, int y) const;
        virtual std::unique_ptr<Figure> set(int x, int y, std::unique_ptr<Figure>&& f);
        virtual void swap(int x1, int y1, int x2, int y2);

        virtual const Move& GetLastMove() const final;

        virtual bool IsPathClear(const Vector2D<int>& src,
                                 const Vector2D<int>& dst) const;
        virtual bool GameOver() { return this->done; }
        virtual bool Restart() { return this->restart;}
        virtual bool KingInDanger(const Figure& king) const;
        virtual bool CheckMate(const Figure& king) const;

        virtual void output(std::ostream& where) const;
        virtual bool input(std::istream& from);

    protected:
        std::vector<std::vector<std::unique_ptr<Figure>>> field_; // Lengthy ain't it?

        bool currentMove = false;

        Move lastMove;

        Figure* whiteKing;
        Figure* blackKing;

        std::vector<Move> moves;

        int checkMateCounter = 0;

        int capturedWhite = 0;
        int capturedBlack = 0;

        bool check = false;

        bool done = false;
        bool restart = false;

        bool againstAI = false;

        virtual bool KingInDanger(Color color, const Vector2D<int>& dst) const;

        char Promote(int x, int y);

        void SuggestAI();

    private:
        void Tests();
};

#endif // FIELD_H
