#include "sleep.h"

void wait(int milliseconds)
{
#ifdef _WIN32
    win::Sleep(milliseconds);
#else
    sleep(milliseconds / 1000.0);
#endif // _WIN32

}
