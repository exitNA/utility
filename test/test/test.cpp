#include "progress_bar.h"
#include "heap.h"
#include "time_ruler.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void progress_bar_test()
{
    printf("*** progressbar test begin ***\n");
    ProgressBar bar;
    size_t range = 37;
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
    printf("*** progressbar test finish ***\n");
}

bool benchmark_heap_cmp(size_t a, size_t b)
{
    return a < b;
}

void benchmark_heap_test()
{
    printf("\n*** benchmark heap test begin ***\n");
    size_t size = 100000;
    size_t top_n = 5;
    size_t* parray = new size_t[size];
    for (size_t i = 0; i < size; ++i)
    {
        parray[i] = i;
    }
    for (size_t i = 0; i < size; ++i)
    {
        std::swap(parray[i], parray[rand() % (size - i) + i]);
    }
    {
        TimeRuler t;
        Heap<size_t, max_heap_cmp<size_t> > mhp(top_n);
        for (size_t i = 0; i < size; ++i)
        {
            mhp.insert(parray[i]);
        }
        mhp.print();
    }
    
    {
        TimeRuler t;
        std::sort(parray, parray + size, benchmark_heap_cmp);
        std::cout << parray[0];
        for (size_t i = 1; i < top_n; ++i)
        {
            std::cout << ", " << parray[i];
        }
        std::cout << std::endl;
    }

    delete [] parray;
    printf("*** benchmark heap test finish ***\n");
}

int main(int argc, char* argv[])
{
    progress_bar_test();
    benchmark_heap_test();
    return 0;
}