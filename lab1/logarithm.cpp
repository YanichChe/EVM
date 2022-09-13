#include "check.h"
#include "logarithm.h"
#include <cmath>

double CountLogarithm(double x, long long int n)
{
    CheckInput(x, n);

    double result = 0;

    for (long long int i = 1; i <= n; i++)
    {
        result += pow(-1, i + 1) * x / i;
        x *= x;
    }

    return result;
}