#include <iostream>

#include "BoilerPlate.h"
#include "Field.h"
#include "UtilityBS.h"
#include "StockFish.h"

class Game : public FrameWork
{
    std::unique_ptr<Field> field;
    UpdateData data;
    bool clrscreen = true;

    virtual void Init() override
    {
        StockFish::Init();


        field = std::make_unique<Field>();
        field->Init();
    }

    virtual void Update() override
    {
        if(clrscreen) UtilityBS::ClearScreen();
        field->Draw(data);
        field->Update(data);

        if(field->GameOver()) {
            this->done = true;
            std::cout << "See ya then\n";
        }

        if(field->Restart()) {
            field->Close();
            field = std::make_unique<Field>();
            field->Init();
        }
    }

    virtual void Close() override
    {
        field->Close();
        StockFish::Close();
    }
};


int main(int argc, char** argv)
{
    Game game;
    return FrameWork::run(game);
}
