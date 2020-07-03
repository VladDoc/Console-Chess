#ifndef COMMANDLINEPARAMS_H
#define COMMANDLINEPARAMS_H


class CommandLineParams final
{
    public:
    bool debug = false;
    static void Init(int argc, char** argv);
    static CommandLineParams& get();
};

#endif // COMMANDLINEPARAMS_H
