#include <iostream>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include "check.h"
#include "logarithm.h"

using namespace std;

int main(int argc, char *argv[])
{
    CheckArgc(argc);
    
    double x = atof(argv[1]);
    int n = atoi(argv[2]);

    try
    {
        cout << fixed << setprecision(5) << CountLogarithm(x, n) << endl;
    }
    catch (const logic_error &e)
    {
        cout << e.what() << endl;
    }

    return EXIT_SUCCESS;
}
