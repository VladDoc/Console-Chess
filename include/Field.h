#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <memory>
#include <deque>

#include "GameObject.h"
#include "Vector2D.h"

class Figure;

enum class Direction : uint8_t
{
    HORISONTAL,
    VERTICAL,
    DIAGONAL,
    COUNT
};


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
        virtual std::unique_ptr<Figure>&& set(int x, int y, std::unique_ptr<Figure>&& f);
        virtual void swap(int x1, int y1, int x2, int y2);

        virtual void GetLastMove(Vector2D<int>& src, Vector2D<int>& dst) const;

        virtual bool IsPathClear(const Vector2D<int>& src,
                                 const Vector2D<int>& dst) const;
        virtual bool GameOver() { return this->done; }
        virtual bool Restart() { return this->restart;}
        virtual bool KingInDanger(const Figure* king) const;

        virtual void output(std::ostream& where) const;
        virtual void input(std::istream& from);

    protected:
        std::vector<std::vector<std::unique_ptr<Figure>>> field_; // Lengthy ain't it?

        bool currentMove = false;

        Vector2D<int> lastMoveSrc;
        Vector2D<int> lastMoveDst;

        std::deque<Figure*> whiteFigures;
        std::deque<Figure*> blackFigures;

        Figure* whiteKing;
        Figure* blackKing;

        int checkMateCounter = 0;
        bool done = false;
        bool restart = false;
    private:
        void Tests();
};

#endif // FIELD_H
