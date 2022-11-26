#include <iostream>
#include <cmath>
#include <ctime>
#include <limits>

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

void PrintMatrix(const float *matrix);
void CopyMatrix(float *matrixInput, float *matrixOutput);
void ConvertToMatrix(float *matrix1, float *matrix2, float **matrix1_, float **matrix2_);

//////////////////////////////////////////////////////////////////////

struct Vector
{
    float x, y, z, w;
};

//////////////////////////////////////////////////////////////////////

int main()
{
    srand(time(0));

    auto *matrix = new float[N * N];
    auto *result = new float[N * N];

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
    std::cout << "TIME: " << (((double)end - start) / CLOCKS_PER_SEC) << std::endl;

    return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

void Add_SSE(Vector *a, Vector *b, Vector *result)
{
    __asm__ volatile("mov %0, %%rax" ::"m"(a));
    __asm__ volatile("mov %0, %%rbx" ::"m"(b));
    __asm__ volatile("movaps (%rax), %xmm0");
    __asm__ volatile("movaps (%rbx), %xmm1");
    __asm__ volatile("addps %xmm1, %xmm0");
    __asm__ volatile("mov %0, %%rax" ::"m"(result));
    __asm__ volatile("movaps %xmm0, (%rax)");
}

void Mul_SSE(Vector *a, Vector *b, Vector *result)
{
    __asm__ volatile("mov %0, %%rax" ::"m"(a));
    __asm__ volatile("mov %0, %%rbx" ::"m"(b));
    __asm__ volatile("movaps (%rax), %xmm0");
    __asm__ volatile("movaps (%rbx), %xmm1");
    __asm__ volatile("mulps %xmm1, %xmm0");
    __asm__ volatile("mov %0, %%rax" ::"m"(result));
    __asm__ volatile("movaps %xmm0, (%rax)");
}

float VectorSum(float *arr)
{
    Vector sumVector = {0, 0, 0, 0};
    Vector *vector;
    vector = (Vector *)arr;

    for (int i = 0; i < N / 4; i++, vector++)
        Add_SSE(vector, &sumVector, &sumVector);

    return (sumVector.x + sumVector.y + sumVector.z + sumVector.w);
}

//////////////////////////////////////////////////////////////////////

float GetMultMaxSums(float *matrix)
{
    float currentSumRow = 0;
    float currentSumColumn = 0;

    float maxSumRow = std::numeric_limits<float>::min();
    float maxSumColumn = std::numeric_limits<float>::min();

    for (int i = 0; i < N; ++i)
    {
        currentSumColumn = 0;
        currentSumRow = 0;

        for (int j = 0; j < N; ++j)
        {
            currentSumRow += matrix[N * i + j];
            currentSumColumn += matrix[j * N + i];
        }

        if (currentSumRow > maxSumRow)
            maxSumRow = currentSumRow;
        if (currentSumColumn > maxSumColumn)
            maxSumColumn = currentSumColumn;
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

void ConvertToMatrix(float *matrix1, float *matrix2, float **matrix1_, float **matrix2_)
{

    for (int i = 0; i < N; i++)
    {
        matrix1_[i] = new float[N];
        matrix2_[i] = new float[N];

        for (int j = 0; j < N; j++)
        {
            matrix1_[i][j] = matrix1[N * i + j];
            matrix2_[i][j] = matrix2[N * j + i]; // +transpose
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////

void Mult(float *matrix1, float *matrix2, float *result)
{
    for (int i = 0; i < N * N; ++i)
        result[i] = 0;

    float **matrix1_ = new float *[N];
    float **matrix2_ = new float *[N];
    ConvertToMatrix(matrix1, matrix2, matrix1_, matrix2_);

    auto *currentResult = new float[N * N];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            Vector *x, *y, *z;
            x = (Vector *)matrix1_[i];
            y = (Vector *)matrix2_[j];
            z = (Vector *)currentResult;

            for (int i = 0; i < N / 4; ++i, ++x, ++y, ++z)
            {
                Mul_SSE(x, y, z);
            }

            result[i * N + j] = VectorSum(currentResult);
        }
    }

    delete[] currentResult;
}

void Subtraction(float *matrix1, float *matrix2, float *result)
{
    for (int i = 0; i < N * N; i++)
        result[i] = matrix1[i] - matrix2[i];
}

void Additional(float *matrix1, float *matrix2, float *result)
{
    for (int i = 0; i < N * N; i++)
        result[i] = matrix1[i] + matrix2[i];
}

void Inverse(float *matrix, float *result)
{
    auto *R = new float[N * N];
    auto *B = new float[N * N];
    auto *I = new float[N * N];
    auto *tmp = new float[N * N];

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
            Mult(R, result, I);      // I = result * R
            Additional(tmp, I, tmp); // tmp = tmp + result
        }
        else
        {
            Mult(R, I, result);           // result = I * R
            Additional(tmp, result, tmp); // tmp = tmp + I
        }
    }

    Mult(tmp, B, result); // result = tmp * B

    delete[] I;
    delete[] R;
    delete[] B;
    delete[] tmp;
}

void PrintMatrix(const float *matrix)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            std::cout << matrix[N * i + j] << " ";

        std::cout << std::endl;
    }
}

void CopyMatrix(float *matrixInput, float *matrixOutput)
{
    for (int i = 0; i < N * N; i++)
        matrixOutput[i] = matrixInput[i];
}
