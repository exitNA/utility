#include "../progress_bar.h"
#include "../heap.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void progress_bar_test()
{
    printf("*** progressbar test begin ***\n");
    ProgressBar bar;
    size_t range = 53;
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

void heap_test()
{
    printf("min heap test\n");
    Heap<int> hp(10);
    for (int i = 10; i > 0; --i)
    {
        hp.insert(i);
    }
    hp.insert(13);
    hp.print();
    printf("finish.\n");
}

int main(int argc, char* argv[])
{
    progress_bar_test();
    heap_test();
    return 0;
}