#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <iomanip>

using namespace std;

double CountLogarithm(double x, int n)
{
    double result = 0;

    for (int i = 1; i <= n; i++)
    {
        result += pow(-1, i + 1) * x / i;
        x *= x;
    }

    return result;
}

void CheckArgc(int argc)
{
    if (argc != 3)
    {
        cout << "Not enough arguments" << endl;
        exit(EXIT_FAILURE);
    }
}

void CheckX(double x)
{
    if (x <= -1 || x > 1)
    {
        cout << "Wrong x value" << endl;
        exit(EXIT_FAILURE);
    }
}

void CheckN(int n)
{
    if (n < 0)
    {
        cout << "Wrong n value" << endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    CheckArgc(argc);

    double x = atof(argv[1]);
    int n = atoi(argv[2]);

    CheckX(x);
    CheckN(n);

    cout << fixed << setprecision(5) << CountLogarithm(x, n) << endl;

    return EXIT_SUCCESS;
}
