#include <string>
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
    printf("\n*** progressbar test begin ***\n");
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

typedef std::pair<std::string, size_t> Elem;

bool benchmark_heap_cmp(Elem const& a, Elem const& b)
{
    return a.second > b.second;
}

void benchmark_heap_test()
{
    printf("\n*** benchmark heap test begin ***\n");
    std::string prefix("abc");
    size_t size = 100000;
    size_t top_n = 5;
    Elem* parray = new Elem[size];
    for (size_t i = 0; i < size; ++i)
    {
        parray[i] = std::make_pair(prefix + std::to_string(i), i);
    }
    for (size_t i = 0; i < size; ++i)
    {
        std::swap(parray[i], parray[rand() % (size - i) + i]);
    }
    {
        // heap sort
        TimeRuler t;
        Heap<Elem, benchmark_heap_cmp> hp(top_n);
        for (size_t i = 0; i < size; ++i)
        {
            hp.insert(parray[i]);
        }
        hp.sort();
        std::cout << hp[0].first;
        for (size_t i = 1; i < top_n; ++i)
        {
            std::cout << ", " << hp[i].first;
        }
        std::cout << std::endl;

    }
    
    {
        // qsort
        TimeRuler t;
        std::sort(parray, parray + size, benchmark_heap_cmp);
        std::cout << parray[0].first;
        for (size_t i = 1; i < top_n; ++i)
        {
            std::cout << ", " << parray[i].first;
        }
        std::cout << std::endl;
    }

    delete [] parray;
    printf("*** benchmark heap test finish ***\n");
}

int main(int argc, char* argv[])
{
    benchmark_heap_test();
    progress_bar_test();
    return 0;
}