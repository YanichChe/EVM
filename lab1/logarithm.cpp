#include "check.h"
#include "logarithm.h"
#include <iostream>

double CountLogarithm(double x, long long int n)
{
    CheckInput(x, n);

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