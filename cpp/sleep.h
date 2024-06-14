#ifdef _WIN32
namespace win
{
#define NOMINMAX

#include <Windows.h>
}

#else
#include <unistd.h>
//#include <sys/sysinfo.h>
#endif

//function to wait for a number of milliseconds
void wait(int milliseconds);

