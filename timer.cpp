#ifndef DHK_LIB_TIMER
#define DHK_LIB_TIMER

#include "template.cpp"

namespace Timer
{
    struct timer
    {
        long double beginning = time(NULL);

        long double checkpoint = beginning;

        void set_checkpoint()
        {
            checkpoint = time(NULL);
        }

        long double since_beginning()
        {
            return time(NULL) - beginning;
        }

        long double since_last_checkpoint()
        {
            return time(NULL) - checkpoint;
        }
    };
}

#endif
