#include <iostream>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <ctime>

using namespace std;

double CountLogarithm(double x, long long int n)
{
    double result = 0;

    for (long long int i = 1; i <= n; i++)
    {
        if (i % 2 == 1)
        {
            result += x / i;
        }
        else
        {
            result -= x / i;
        }
        x *= x;
    }

    return result;
}

int main(int argc, char *argv[])
{
    struct timespec start, end;
    double totalTime = 0;

    double x = atof(argv[1]);
    long long int n = atoll(argv[2]);

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    cout << CountLogarithm(x, n) << endl;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    double time = end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
    cout << time << endl;

    return EXIT_SUCCESS;
}



