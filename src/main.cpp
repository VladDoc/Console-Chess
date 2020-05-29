#include "BoilerPlate.h"
#include "Field.h"
#include "UtilityBS.h"

class Game : public FrameWork
{
    std::unique_ptr<Field> field;
    UpdateData data;
    bool clrscreen = true;

    virtual void Init() override
    {
        field = std::make_unique<Field>();
        field->Init();
    }

    virtual void Update() override
    {
        if(clrscreen) UtilityBS::ClearScreen();
        field->Draw(data);
        field->Update(data);

        if(field->GameOver()) this->done = true;

        if(field->Restart()) {
            field->Close();
            field = std::make_unique<Field>();
            field->Init();
        }
    }

    virtual void Close() override
    {
        field->Close();
    }
};


int main()
{
    Game game;
    return FrameWork::run(game);
}
