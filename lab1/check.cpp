#include <cstdlib>
#include "check.h"
#include <iostream>
#include <exception>

void CheckInput(double x, int n)
{
    if (x <= -1 || x > 1)
    {
        throw std::logic_error("Wrong x value");
    }

    if (n < 1)
    {
        throw std::logic_error("Wrong n value");
    }
}

void CheckArgc(int argc)
{
    if (argc != 3)
    {
        std::cout << "Not enough arguments" << std::endl;
        exit(EXIT_SUCCESS);
    }
}
