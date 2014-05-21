#include "suffix_array.h"
#include <stdint.h>
#include <string>
#include <iostream>
#include "time_ruler.h"

void test_suffix_array()
{
    SuffixArray<> sa;
    char* str = "mississippi";
    TimeRuler t(TimeRuler::NoAutoShow);
    sa.build(str, strlen(str) + 1, 256);
    std::cout << "total build time: " << t.elapsed_ms() << " ms." << std::endl;
}