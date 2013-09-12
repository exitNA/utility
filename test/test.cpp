#include "../progress_bar.h"
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void progress_bar_test()
{
    printf("*** progressbar test begin ***\n");
    ProgressBar bar;
    size_t range = 100;
    bar.init(range);

    for (size_t i = 0; i < range; ++i)
    {
        bar.update(i);
#ifdef _WIN32
        Sleep(50);
#else
        sleep(1);
#endif
    }
    bar.finish();
    printf("*** progressbar test finish ***");
}

int main(int argc, char* argv[])
{
    progress_bar_test();
    return 0;
}