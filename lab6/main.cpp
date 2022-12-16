#include <iostream>
#include <climits>
#include <iomanip>
#include <x86intrin.h>

const unsigned int N = 200000000;
const unsigned int RUN_TIMES = 3;
const unsigned int FRAGMENTS_COUNT = 32;

void initArray(unsigned int *array, unsigned int fragments, size_t offset, size_t size);
unsigned long run(unsigned int const *array);
void countTime(unsigned int *array, unsigned int fragments, int offset, int size);

int main(void){

    auto *array = new unsigned int[N];

    //unsigned int blockSize = 32 * 1024; // 32Kb
    //unsigned int blockSize = 256 * 1024; // 256Kb
    unsigned int blockSize = 8 * 1024 * 1024; // 8 MB
    unsigned int offset = blockSize;


    for(int fragments = 1; fragments <= FRAGMENTS_COUNT; fragments++)
    {
        countTime(array, fragments, offset / sizeof(int), blockSize / sizeof(int));
    }

    delete[] array;
    return 0;
}

void initArray(unsigned int *array, unsigned int fragments, size_t offset, size_t size)
{
    size_t j = 1;
    size_t i = 0;

    for(; i < size; i++)
    {
        for(j = 1; j < fragments; j++)
            array[i + (j - 1) * offset] = i + j * offset;

        array[i + (j - 1) * offset] = i + 1;
    }

    array[i - 1 + (j - 1) * offset] = 0;
}

unsigned long run(unsigned int const *array)
{
    unsigned long long minTime = ULLONG_MAX;

    for(size_t j = 0; j < RUN_TIMES; j++)
    {
        unsigned long long startTime = __rdtsc();
        for(size_t k = 0, i = 0; i < N; i++){
            k = array[k];
        }
        unsigned long long endTime = __rdtsc();

        if (minTime > endTime - startTime) minTime = endTime - startTime;
    }

    return minTime / N;
}

void countTime(unsigned int *array, unsigned int fragments, int offset, int size)
{
    initArray(array, fragments, offset, size);
    std::cout << std::left << std::setw(4)
                           << fragments
                           << std::setw(4)
                           << run(array)
                           << std::endl;
}
