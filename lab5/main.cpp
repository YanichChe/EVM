#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <set>
#include <string>
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#include <iomanip>
#include <vector>

enum MODE
{
    DIRECT,
    REVERSE,
    RANDOM
};

void directTraversal(unsigned int *array, const unsigned int n)
{
    array[n - 1] = 0;
    for (unsigned int i = 0; i < n - 1; i++)
    {
        array[i] = i + 1;
    }
}

void reverseTraversal(unsigned int *array, const unsigned int n)
{
    array[0] = n - 1;
    for (unsigned int i = n - 1; i > 0; --i)
        array[i] = i - 1;
}

void randomTraversal(unsigned int *array, const unsigned int arraySize)
{
    std::set<unsigned int> set;

    for(unsigned int i = 0; i < arraySize; ++i) set.insert(i);
    unsigned int index = rand() % set.size();
    unsigned int prev = *next(set.begin(), index);
    unsigned int start = prev;

    for(; set.size() > 1;)
    {
        set.erase(next(set.begin(), index));
        index = rand() % set.size();
        array[prev] = *next(set.begin(), index);
        prev = *next(set.begin(), index);

    }
    array[*set.begin()] = start;

}

void assign(const unsigned int *array, const unsigned int n, const unsigned int k)
{
    int m = 0;
    for (unsigned int j = 0; j < n * k; ++j)
        m = array[m];
}

void CalcTime(MODE modeName, const unsigned int minSize, const unsigned int maxSize,
              unsigned int *array, const int k)
{
    std::cout << std::setw(7)<< std::left << "KB"
              << std::setw(15)<< std::left << "size"
              << std::setw(7)<< std::left << "time" << std::endl;
    unsigned int currentSize = minSize;
    for (; currentSize < maxSize; currentSize = (unsigned int)(currentSize * 1.2))
    {
        array = new unsigned int[currentSize];

        if (modeName == DIRECT)             directTraversal(array, currentSize);
        else if (modeName == REVERSE)       reverseTraversal(array, currentSize);
        else                                randomTraversal(array, currentSize);

        unsigned long long minTime = LLONG_MAX;

        assign(array, currentSize, 3);

        for (int j = 0; j < 3; j++)
        {
            unsigned long long start = __rdtsc();
            assign(array, currentSize, k);
            unsigned long long end = __rdtsc();
            unsigned long long Time = end - start;
            if (Time < minTime) minTime = Time;
        }

        std::cout << std::setw(7) << std::left << currentSize / 256
                  << std::setw(15)<< std::left << currentSize
                  << std::setw(7) << std::left << minTime / (k * currentSize) << std::endl;

        free(array);
    }
}

int main()
{
    unsigned int minSize = 256;
    unsigned int maxSize = 32 * 1024 * 256;
    unsigned int k = 10;
    unsigned int *array;

    std::cout << "DIRECT TRAVERSAL" << std::endl;
    CalcTime(DIRECT, minSize, maxSize, array, k);

    std::cout << "REVERSE TRAVERSAL" << std::endl;
    CalcTime(REVERSE, minSize, maxSize, array, k);

    std::cout << "RANDOM TRAVERSAL" << std::endl;
    CalcTime(RANDOM, minSize, maxSize, array, k);

}
