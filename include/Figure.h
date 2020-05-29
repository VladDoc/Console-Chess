#ifndef FIGURE_H
#define FIGURE_H

#include <cstdint>

#include "GameObject.h"
#include "Field.h"

class Field;

enum class Color : uint8_t
{
    BLACK,
    WHITE,
    UNKNOWN,
    COUNT
};

// Figure does not do anything besides move and storing state
// So I decided that it won't inherit gameobject
class Figure
{
    public:
        Figure() : x_(-1), y_(-1) {}
        virtual ~Figure();
        virtual bool Move(int x, int y, Field& f);
        virtual bool ValidateMove(int x1, int y1,
                                  int x2, int y2, const Field& f) const = 0;
        virtual void Draw(const UpdateData& data) const;

        // I've no idea why these are virtual
        // For overriding perhaps
        virtual bool IsWhite() const;
        virtual bool IsBlack() const;
        virtual Color GetColor() const;
        virtual bool IsEmpty() const;
        virtual void SetEmpty();
        virtual void SetCoords(int x, int y);
        virtual void GetCoords(int& x, int& y) const;
        virtual bool GetFirstMove() const { return firstMove;}
        virtual const char* GetName() const;

        virtual void output(std::ostream& where) const;
    protected:
        int x_, y_;
        std::string name = "  ";
        bool firstMove = true;

        Color color = Color::UNKNOWN;

    private:
};

#endif // FIGURE_H
