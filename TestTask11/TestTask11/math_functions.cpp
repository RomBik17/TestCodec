
#include "math_functions.h"
#include <iostream>
using namespace std;

double variance(int* array, int N)
{
    double mu;
    double sum = 0;
    for (int i = 0; i < N; ++i)
    {
        sum += array[i];
    }
    mu = sum / N;
    double var = 0;
    for (int i = 0; i < N; ++i)
    {
        var += pow((array[i] - mu), 2);
    }
    var /= (1.0 / (N - 1));
    if (var == 0) var++;
    return var;
}

double variance(char* array, int N)
{
    double mu;
    double sum = 0;
    for (int i = 0; i < N; ++i)
    {
        sum += (unsigned char)array[i];
    }
    mu = sum / N;
    double var = 0;
    for (int i = 0; i < N; ++i)
    {
        var += pow(((unsigned char)array[i] - mu), 2);
    }
    var /= (1.0 / (N - 1));
    if (var == 0) var++;
    return var;
}

int VAQ(double blockVAR, double frameVAR)
{
    int PQ;
    PQ = (int)(D * log10(blockVAR) - log10(frameVAR));
    return PQ;
}

double PSNR(int* startFrame, int* dumpedFrame)
{
    double sum = 0;
    for (int i = 0; i < pixels_on_video; ++i)
    {
        int delta = startFrame[i] - dumpedFrame[i];
        sum += pow(delta, 2);
    }
    double MSE = sum / pixels_on_video;
    const int MAX = 255;
    double psnr = 10 * log10(pow(MAX, 2) / MSE);
    return psnr;
}

void quanting(int* block, int blockSize, int QP)
{
    int quant = 1 << QP;

    for (int i = 0; i < blockSize; ++i)
    {
        int k = i * blockSize;
        for (int j = 0; j < blockSize; ++j)
        {
            bool quantization = true;
            int q = 1;
            if (i == 0 && j == 0)
            {
                quantization = false;
            }
            else if (i < (blockSize / 2) && j < (blockSize / 2))
            {
                q = quant / 2;
            }
            else if (i >= (blockSize / 2) && j >= (blockSize / 2))
            {
                q = quant * 2;
            }

            if (quantization)
            {
                //calculate the nearest multiple of Q
                block[k + j] /= q;
            }
        }
    }
}

void dequanting(int* block, int blockSize, int QP)
{
    int quant = 1 << QP;

    for (int i = 0; i < blockSize; ++i)
    {
        int k = i * blockSize;
        for (int j = 0; j < blockSize; ++j)
        {
            bool quantization = true;
            int q = 1;
            if (i == 0 && j == 0)
            {
                quantization = false;
            }
            else if (i < (blockSize / 2) && j < (blockSize / 2))
            {
                q = quant / 2;
            }
            else if (i >= (blockSize / 2) && j >= (blockSize / 2))
            {
                q = quant * 2;
            }

            if (quantization)
            {
                //calculate the nearest multiple of Q
                block[k + j] *= q;
            }
        }
    }
}

double cosTransformFunction(int m, int p, int blockSize)
{
    double argument = PI * (2.0 * m + 1) * p;
    argument /= (2.0 * blockSize);
    return cos(argument);
}

void IDCT(int* block, int blockSize)
{
    //create new block for IDCT data
    int* idctBlock = new int[blockSize * blockSize];

    int Cm;
    int Cn;
    for (int y = 0; y < blockSize; ++y)
    {
        int r = y * blockSize;
        for (int x = 0; x < blockSize; ++x)
        {
            long sum = 0;
            for (int m = 0; m < blockSize; ++m)
            {
                int k = m * blockSize;
                for (int n = 0; n < blockSize; ++n)
                {
                    if (m == 0) Cm = FIX(sqrt(1.0 / 2.0));
                    else Cm = FIX(1);
                    if (n == 0) Cn = FIX(sqrt(1.0 / 2.0));
                    else Cn = FIX(1);
                    int coef = DESCALE(Cm * Cn) * block[k + n];
                    coef = DESCALE(coef);
                    int cos1 = FIX(cosTransformFunction(x, n, blockSize));
                    int cos2 = FIX(cosTransformFunction(y, m, blockSize));
                    int a = coef * DESCALE(cos1 * cos2);
                    sum += DESCALE(a);
                }
            }
            idctBlock[r + x] = 2 * sum / blockSize;
        }
    }

    //copy IDCT data to original block
    for (int i = 0; i < blockSize; ++i)
    {
        int r = i * blockSize;
        for (int j = 0; j < blockSize; ++j)
        {
            block[r + j] = idctBlock[r + j];
        }
    }

    delete[] idctBlock;
}

void DCT(int* block, int blockSize)
{
    //create new block for DCT data
    int* dctBlock = new int[blockSize * blockSize];

    int Ap;
    int Aq;
    for (int p = 0; p < blockSize; ++p)
    {
        int r = p * blockSize;
        for (int q = 0; q < blockSize; ++q)
        {
            if (p == 0) Ap = FIX(sqrt(1.0 / blockSize));
            else Ap = FIX(sqrt(2.0 / blockSize));
            if (q == 0) Aq = FIX(sqrt(1.0 / blockSize));
            else Aq = FIX(sqrt(2.0 / blockSize));

            long sum = 0;
            for (int m = 0; m < blockSize; ++m)
            {
                int k = m * blockSize;
                for (int n = 0; n < blockSize; ++n)
                {
                    int cos1 = FIX(cosTransformFunction(m, p, blockSize));
                    int cos2 = FIX(cosTransformFunction(n, q, blockSize));
                    int a = block[k + n] * DESCALE(cos1 * cos2);
                    a = DESCALE(a);
                    sum += a;
                }
            }
            dctBlock[r + q] = DESCALE(Ap * Aq) * sum;
            dctBlock[r + q] = DESCALE(dctBlock[r + q]);
        }
    }

    //copy DCT data to original block 
    for (int i = 0; i < blockSize; ++i)
    {
        int r = i * blockSize;
        for (int j = 0; j < blockSize; ++j)
        {
            block[r + j] = dctBlock[r + j];
        }
    }

    delete[] dctBlock;
}

void SAD(int* currentBlock, int* predictedBlock, int* sadBlock, int blockSize)
{
    for (int i = 0; i < blockSize; ++i)
    {
        int r = i * blockSize;
        for (int j = 0; j < blockSize; ++j)
        {
            sadBlock[r + j] = currentBlock[r + j] - predictedBlock[r + j];
        }
    }
}

void reverseSAD(int* currentBlock, int* predictedBlock, int* sadBlock, int blockSize)
{
    for (int i = 0; i < blockSize; ++i)
    {
        int r = i * blockSize;
        for (int j = 0; j < blockSize; ++j)
        {
            currentBlock[r + j] = predictedBlock[r + j] + sadBlock[r + j];
        }
    }
}
