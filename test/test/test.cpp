#include <string>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "progress_bar.h"
#include "heap.h"
#include "time_ruler.h"
#include "binary_file.h"
#include "log.h"

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


void benchmark_heap_test()
{
    printf("\n*** benchmark heap test begin ***\n");
    size_t size  = 100000;
    size_t top_n = 100000;
    int* parray = new int[size];
    printf("rand max: %d\n", RAND_MAX);
    //std::srand(std::time(0));
    for (size_t i = 0; i < size; ++i)
    {
        parray[i] = std::rand() + std::rand();
    }
    {
        // heap sort
        TimeRuler t;
        Heap<int> hp(top_n);
        for (size_t i = 0; i < size; ++i)
        {
            hp.insert(parray[i]);
        }
        hp.sort();
//        std::cout << hp[0];
//        for (size_t i = 1; i < top_n; ++i)
//        {
//            std::cout << ", " << hp[i];
//        }
//        std::cout << std::endl;

    }
    
    {
        // qsort
        TimeRuler t;
        std::sort(parray, parray + size);
//        std::cout << parray[size - 1];
//        for (size_t i = 1; i < top_n; ++i)
//        {
//            std::cout << ", " << parray[size - i - 1];
//        }
//        std::cout << std::endl;
    }

    delete [] parray;
    printf("*** benchmark heap test finish ***\n");
}

void buffer_writer_test()
{
    printf("\n****** %s ********\n", __FUNCTION__);
    BinaryFile f("d:/test.bin");

}

void time_ruler_test()
{
    {
        TimeRuler t;
        Sleep(500);
    }
    {
        TimeRuler t;
        Sleep(8000);
    }
    {
        TimeRuler t;
        Sleep(1000 * 70);
    }
}

int main(int argc, char* argv[])
{
    //benchmark_heap_test();
    //progress_bar_test();
    //buffer_writer_test();
//    time_ruler_test();
    //size_t elapsed = 1 * 60 * 1000;
    //if (elapsed >= 1000 * 60)
    //{
    //    elapsed /= 1000;
    //    printf("%s = %d min + %d s\n", "test", elapsed / 60, elapsed % 60);
    //}
    for (int i = 0; i < 5; ++i) {
        log_cyan("day %d", i);
        log_error("day %d", i);
        log_info("day %d", i);
        log_warning("day %d", i);
        log_msg("day %d", i);
    }
    printf("%s helo %d", __FUNCTION__, 3);
    return 0;
}