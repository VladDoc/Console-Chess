#include "CommandLineParams.h"

static CommandLineParams params_;


CommandLineParams& CommandLineParams::get()
{
    return params_;
}

void CommandLineParams::Init(int argc, char** argv)
{
    if(argc != 2) return;

    bool debug
}
