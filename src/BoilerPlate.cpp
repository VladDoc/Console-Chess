#include "BoilerPlate.h"
#include <iostream>
#include <locale>

int FrameWork::run(FrameWork& fw)
{
    fw.Init();
    setlocale(LC_ALL, "");
    while(true)
    {
        fw.Update();
        if(fw.done) break;
    }
    fw.Close();
    std::cin.get();

    return 0;
}
