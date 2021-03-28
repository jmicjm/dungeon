#include <time.h>
#include <stdlib.h>
#include <fstream>


int rand(int min, int max)
{
    auto initSeed = []()
    {
        int s = time(0);
        std::ofstream("seed.txt") << s;
        return s;
    };
    static int seed = initSeed();
    srand(seed++);

    return min + rand() % abs(max + 1 - min);
}