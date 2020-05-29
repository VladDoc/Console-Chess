#ifndef BOILER_PLATE
#define BOILER_PLATE

class FrameWork
{
    public:
        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Close() = 0;
        virtual ~FrameWork(){}
        bool done = false;

        static int run(FrameWork& fw);
};

#endif // BOILER_PLATE
