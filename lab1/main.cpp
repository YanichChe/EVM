#include <iostream>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <ctime>
#include "check.h"
#include "logarithm.h"

using namespace std;

bool CommandLineWork(int argc, char *argv[]);
void PrintTimeProgramWork(long long int n);

int main(int argc, char *argv[])
{

    /*if (!CommandLineWork(argc, argv))
    {
        return EXIT_FAILURE;
    }
    */
    PrintTimeProgramWork(atoll(argv[1]));

    return EXIT_SUCCESS;
}

void PrintTimeProgramWork(long long int n)
{
    struct timespec start, end;

    double totalTime = 0;
    int runs = 1;

    double x = 0.2;

    for (int i = 0; i < runs; i++)
    {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        cout << CountLogarithm(0.2, n) << endl;
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        cout << "Run №" << i + 1 << " x = " << x << " n = " << n << ". Time taken ";
        double time = end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
        cout << time << endl;
        totalTime += time;
    }

    //cout << "Average time: " << totalTime / runs << " sec" << endl;
}

bool CommandLineWork(int argc, char *argv[])
{
    CheckArgc(argc);

    double x = atof(argv[1]);
    int n = atoll(argv[2]);

    try
    {
        cout << fixed << setprecision(10) << CountLogarithm(x, n) << endl;
    }
    catch (const logic_error &e)
    {
        cout << e.what() << endl;
        return false;
    }
    return true;
}