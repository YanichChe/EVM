#include "mkl.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <float.h>

const int N = 2048;
const int M = 10;

///////////////////////////////////////////////////////////////////////

float GetMultMaxSums(float *matrix);
void DefineB(float *matrix, float *B);
void DefineI(float *I);

void Mult(float *matrix1, float *matrix2, float *result);
void Additional(float *matrix1, float *matrix2, float *result);
void Subtraction(float *matrix1, float *matrix2, float *result);

void Inverse(float *matrix, float *result);

//void PrintMatrix(const float *matrix);
void CopyMatrix(float *matrixInput, float *matrixOutput);

//////////////////////////////////////////////////////////////////////

int main()
{
    srand(time(0));

    float *matrix = (float *)calloc(N*N, sizeof(float));
    float *result = (float *)calloc(N*N, sizeof(float));

    for (int i = 0; i < N * N; ++i)
    {
        matrix[i] = rand() % 10;
        result[i] = 0;
    }

    // PrintMatrix(matrix);
    clock_t start = clock();
    Inverse(matrix, result);
    clock_t end = clock();
    // PrintMatrix(result);
    printf("TIME : %lf sec \n ", (double)(clock() - start) / CLOCKS_PER_SEC);

    return EXIT_SUCCESS;
}
//////////////////////////////////////////////////////////////////////

float GetMultMaxSums(float *matrix)
{
    float currentSumRow = 0;
    float currentSumColumn = 0;

    float maxSumRow = FLT_MIN;
    float maxSumColumn = FLT_MIN;

    for (int i = 0; i < N; ++i)
    {
        currentSumColumn = 0;
        currentSumRow = 0;

        for (int j = 0; j < N; ++j)
        {
            currentSumRow += matrix[N * i + j];
            currentSumColumn += matrix[j * N + i];
        }

        if (currentSumRow > maxSumRow) maxSumRow = currentSumRow;
        if (currentSumColumn > maxSumColumn) maxSumColumn = currentSumColumn;

    }

    return maxSumColumn * maxSumRow;
}

void DefineB(float *matrix, float *B)
{
    float divider = GetMultMaxSums(matrix);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            B[N * i + j] = matrix[j * N + i] / divider;
}

void DefineI(float *I)
{
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            (i == j) ? I[N * i + j] = 1 : I[N * i + j] = 0;
}

//////////////////////////////////////////////////////////////////////////////////////

void Mult(float *matrix1, float *matrix2, float *result)
{
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, matrix1, N, matrix2, N, 0.0, result, N);
}

//////////////////////////////////////////////////////////////////////////////////////

void Subtraction(float *matrix1, float *matrix2, float *result)
{
    cblas_saxpy(N*N, -1.0, matrix2, 1, matrix1, 1);
    cblas_scopy(N * N, matrix1, 1, result, 1);
}

void Additional(float *matrix1, float *matrix2, float *result)
{
    for (int i = 0; i < N * N; i++)
        result[i] = matrix1[i] + matrix2[i];
}

void Inverse(float *matrix, float *result)
{
    float *R = (float *)calloc(N*N, sizeof(float));
    float *B = (float *)calloc(N*N, sizeof(float));
    float *I = (float *)calloc(N*N, sizeof(float));
    float *tmp = (float *)calloc(N*N, sizeof(float));

    DefineB(matrix, B);
    DefineI(I);

    Mult(B, matrix, tmp);   // tmp = BA
    Subtraction(I, tmp, R); // R = I - BA
    Additional(I, R, tmp);  // tmp = I + R
    CopyMatrix(R, result);  // result = R

    for (int i = 2; i < M; ++i)
    {
        if (i % 2 == 0)
        {
            Mult(R, result, I);    // I = result * R
            Additional(tmp, I, tmp); // tmp = tmp + result
        }
        else
        {
            Mult(R, I, result);    // result = I * R
            Additional(tmp, result, tmp); // tmp = tmp + I
        }
    }

    Mult(tmp, B, result); // result = tmp * B

    free(I);
    free(R);
    free(B);
    free(tmp);
}

/*void PrintMatrix(const float *matrix)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            std::cout << matrix[N * i + j] << " ";

        std::cout << std::endl;
    }
}*/

void CopyMatrix(float *matrixInput, float *matrixOutput)
{
    for (int i = 0; i < N * N; i++)
        matrixOutput[i] = matrixInput[i];
}


