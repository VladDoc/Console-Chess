#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "UpdateData.h"

class GameObject
{
    public:
        GameObject();
        virtual ~GameObject();
        virtual void Init() = 0;
        virtual void Update(const UpdateData& data) = 0;
        virtual void Draw(const UpdateData& data) const = 0;
        virtual void Close() = 0;
    protected:

    private:
};

#endif // GAMEOBJECT_H
