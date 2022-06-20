
#include "encoder.h"
using namespace std;

void intraReading(char* reconstructedFrame, int* modeUpBlock, int* modeLeftBlock, int blockSize, int y, int x)
{
    int modeY;
    int tempY, tempX;
    //read up-forward block
    if (y >= blockSize)
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            if (y - blockSize + i >= HEIGHT) tempY = (HEIGHT - 1) * LENGTH;
            tempY = (y - blockSize + i) * LENGTH;
            for (int j = 0; j < blockSize; ++j)
            {
                if (x + j >= LENGTH) tempX = LENGTH - 1;
                else tempX = x + j;
                modeUpBlock[modeY + j] = (unsigned char)reconstructedFrame[tempY + tempX];
            }
        }
    }
    else
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            for (int j = 0; j < blockSize; ++j)
            {
                modeUpBlock[modeY + j] = 128;
            }
        }
    }

    //read left-forward block
    if (x >= blockSize)
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            if (y + i >= HEIGHT) tempY = (HEIGHT - 1) * LENGTH;
            else tempY = (y + i) * LENGTH;
            for (int j = 0; j < blockSize; ++j)
            {
                if (x - blockSize + j >= LENGTH) tempX = LENGTH - 1;
                else tempX = x - blockSize + j;
                modeLeftBlock[modeY + j] = (unsigned char)reconstructedFrame[tempY + tempX];
            }
        }
    }
    else
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            for (int j = 0; j < blockSize; ++j)
            {
                modeLeftBlock[modeY + j] = 128;
            }
        }
    }
}

void interReading(char* reconstructedFrame, char* previousFrame, int** reconstructedBlocks, int blockSize, int y, int x)
{
    intraReading(reconstructedFrame, reconstructedBlocks[1], reconstructedBlocks[0], blockSize, y, x);

    //read blocks frpm previous frame
    int modeY;
    int tempY, tempX;
    for (int i = 0; i < blockSize; ++i)
    {
        modeY = i * blockSize;
        if (y + i >= HEIGHT) tempY = (HEIGHT - 1) * LENGTH;
        else tempY = (y + i) * LENGTH;
        for (int j = 0; j < blockSize; ++j)
        {
            if (x + j >= LENGTH) tempX = LENGTH - 1;
            else tempX = x + j;
            reconstructedBlocks[2][modeY + j] = (unsigned char)previousFrame[tempY + tempX];
        }
    }

    if (x >= blockSize)
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            if (y + i >= HEIGHT) tempY = (HEIGHT - 1) * LENGTH;
            else tempY = (y + i) * LENGTH;
            for (int j = 0; j < blockSize; ++j)
            {
                if (x - blockSize + j >= LENGTH) tempX = LENGTH - 1;
                else tempX = x - blockSize + j;
                reconstructedBlocks[3][modeY + j] = (unsigned char)previousFrame[tempY + tempX];
            }
        }
    }
    else
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            for (int j = 0; j < blockSize; ++j)
            {
                reconstructedBlocks[3][modeY + j] = 128;
            }
        }
    }

    if (x < LENGTH - blockSize)
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            if (y + i >= HEIGHT) tempY = (HEIGHT - 1) * LENGTH;
            else tempY = (y + i) * LENGTH;
            for (int j = 0; j < blockSize; ++j)
            {
                if (x + blockSize + j >= LENGTH) tempX = LENGTH - 1;
                else tempX = x + blockSize + j;
                reconstructedBlocks[4][modeY + j] = (unsigned char)previousFrame[tempY + tempX];
            }
        }
    }
    else
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            for (int j = 0; j < blockSize; ++j)
            {
                reconstructedBlocks[4][modeY + j] = 128;
            }
        }
    }

    if (y >= blockSize)
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            if (y - blockSize + i >= HEIGHT) tempY = (HEIGHT - 1) * LENGTH;
            else tempY = (y - blockSize + i) * LENGTH;
            for (int j = 0; j < blockSize; ++j)
            {
                if (x + j >= LENGTH) tempX = LENGTH - 1;
                else tempX = x + j;
                reconstructedBlocks[5][modeY + j] = (unsigned char)previousFrame[tempY + tempX];
            }
        }
    }
    else
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            for (int j = 0; j < blockSize; ++j)
            {
                reconstructedBlocks[5][modeY + j] = 128;
            }
        }
    }

    if (y < HEIGHT - blockSize)
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            if (y + blockSize + i >= HEIGHT) tempY = (HEIGHT - 1) * LENGTH;
            else tempY = (y + blockSize + i) * LENGTH;
            for (int j = 0; j < blockSize; ++j)
            {
                if (x + j >= LENGTH) tempX = LENGTH - 1;
                else tempX = x + j;
                reconstructedBlocks[6][modeY + j] = (unsigned char)previousFrame[tempY + tempX];
            }
        }
    }
    else
    {
        for (int i = 0; i < blockSize; ++i)
        {
            modeY = i * blockSize;
            for (int j = 0; j < blockSize; ++j)
            {
                reconstructedBlocks[6][modeY + j] = 128;
            }
        }
    }
}

int* inter_frame(char* reconstructedFrame, char* previousFrame, int* modeMatrix,
    int* predictedBlock, int* reconstructedBlock, int blockSize, int y, int x)
{
    int** reconstructedBlocks = new int* [7];
    int** sadBlocks = new int* [7];
    for (int k = 0; k < 7; ++k)
    {
        reconstructedBlocks[k] = new int[blockSize * blockSize];
        sadBlocks[k] = new int[blockSize * blockSize];
    }

    interReading(reconstructedFrame, previousFrame, reconstructedBlocks, blockSize, y, x);

    for (int k = 0; k < 7; ++k)
    {
        SAD(predictedBlock, reconstructedBlocks[k], sadBlocks[k], blockSize);
    }

    int minSum = 0;
    int blockId = 0;
    for (int i = 0; i < blockSize; ++i)
    {
        int r = i * blockSize;
        for (int j = 0; j < blockSize; ++j)
        {
            minSum += abs(sadBlocks[0][r + j]);
        }
    }
    for (int k = 1; k < 7; ++k)
    {
        int tempSum = 0;
        for (int i = 0; i < blockSize; ++i)
        {
            int r = i * blockSize;
            for (int j = 0; j < blockSize; ++j)
            {
                tempSum += abs(sadBlocks[k][r + j]);
            }
        }
        if (tempSum < minSum)
        {
            minSum = tempSum;
            blockId = k;
        }
    }

    int* residualBlock = sadBlocks[blockId];
    sadBlocks[blockId] = nullptr;
    for (int i = 0; i < blockSize; ++i)
    {
        int r = i * blockSize;
        for (int j = 0; j < blockSize; ++j)
        {
            reconstructedBlock[r + j] = reconstructedBlocks[blockId][r + j];
        }
    }

    //write prediction mode for decoding
    int wBlock;
    if (((int)(LENGTH / blockSize)) * blockSize == LENGTH) wBlock = LENGTH / blockSize;
    else wBlock = LENGTH / blockSize + 1;
    modeMatrix[(y / blockSize) * wBlock + (x / blockSize)] = blockId;

    for (int k = 0; k < 7; ++k)
    {
        delete[] reconstructedBlocks[k];
        if (k != blockId) delete[] sadBlocks[k];
    }
    delete[] reconstructedBlocks;
    delete[] sadBlocks;

    return residualBlock;
}

int* intra_frame(char* reconstructedFrame, int* modeMatrix,
    int* predictedBlock, int* reconstructedBlock, int blockSize, int y, int x)
{
    int** reconstructedBlocks = new int* [2];
    int** sadBlocks = new int* [2];
    for (int k = 0; k < 2; ++k)
    {
        reconstructedBlocks[k] = new int[blockSize * blockSize];
        sadBlocks[k] = new int[blockSize * blockSize];
    }

    intraReading(reconstructedFrame, reconstructedBlocks[1], reconstructedBlocks[0], blockSize, y, x);
    //calculate SAD from up and left blocks
    for (int k = 0; k < 2; ++k)
    {
        SAD(predictedBlock, reconstructedBlocks[k], sadBlocks[k], blockSize);
    }

    int minSum = 0;
    int blockId = 0;
    for (int i = 0; i < blockSize; ++i)
    {
        int r = i * blockSize;
        for (int j = 0; j < blockSize; ++j)
        {
            minSum += abs(sadBlocks[0][r + j]);
        }
    }
    for (int k = 1; k < 2; ++k)
    {
        int tempSum = 0;
        for (int i = 0; i < blockSize; ++i)
        {
            int r = i * blockSize;
            for (int j = 0; j < blockSize; ++j)
            {
                tempSum += abs(sadBlocks[k][r + j]);
            }
        }
        if (tempSum < minSum)
        {
            minSum = tempSum;
            blockId = k;
        }
    }

    int* residualBlock = sadBlocks[blockId];
    sadBlocks[blockId] = nullptr;
    for (int i = 0; i < blockSize; ++i)
    {
        int r = i * blockSize;
        for (int j = 0; j < blockSize; ++j)
        {
            reconstructedBlock[r + j] = reconstructedBlocks[blockId][r + j];
        }
    }

    //write prediction mode for decoding
    int wBlock;
    if (((int)(LENGTH / blockSize)) * blockSize == LENGTH) wBlock = LENGTH / blockSize;
    else wBlock = LENGTH / blockSize + 1;
    modeMatrix[(y / blockSize) * wBlock + (x / blockSize)] = blockId;

    for (int k = 0; k < 2; ++k)
    {
        delete[] reconstructedBlocks[k];
        if (k != blockId) delete[] sadBlocks[k];
    }
    delete[] reconstructedBlocks;
    delete[] sadBlocks;

    return residualBlock;
}

int splitBlockPadding(char* tempFrame, char* previousFrame, char* reconstructedFrame,
    int* frameCoef, int* modeMatrix, int* qMatrix, int blockSize, int y, int x, int frameN, double frameVAR, int task)
{
    int* predictedBlock = new int[blockSize * blockSize];
    int* reconstructedBlock = new int[blockSize * blockSize];

    //read original block, what we want to predict
    int tempX, tempY;
    for (int i = y; i < (blockSize + y); ++i)
    {
        int r = (i - y) * blockSize - x;
        if (i >= HEIGHT) tempY = (HEIGHT - 1) * LENGTH;
        else tempY = i * LENGTH;
        for (int j = x; j < (blockSize + x); ++j)
        {
            if (j >= LENGTH) tempX = LENGTH - 1;
            else tempX = j;
            predictedBlock[r + j] = (unsigned char)tempFrame[tempY + tempX];
        }
    }

    double blockVAR = variance(predictedBlock, blockSize * blockSize);
    int QP;
    QP = Q + VAQ(blockVAR, frameVAR);
    if (QP < 1) QP = 1;
    else if (QP > 6) QP = 6;

    if (task == 2)
    {
        QP = 1;
    }

    //if it's a first frame, we have to do intra-frame prediction
    int* residualBlock = nullptr;
    if (frameN == 0 || (frameN + 1) % IR == 0)
    {
        residualBlock = intra_frame(reconstructedFrame, modeMatrix, predictedBlock, reconstructedBlock, blockSize, y, x);
    }
    else
    {
        residualBlock = inter_frame(tempFrame, previousFrame, modeMatrix, predictedBlock, reconstructedBlock, blockSize, y, x);
    }

    //transform and quantize
    DCT(residualBlock, blockSize);
    quanting(residualBlock, blockSize, QP);

    int wBlock;
    if (((int)(LENGTH / blockSize)) * blockSize == LENGTH) wBlock = LENGTH / blockSize;
    else wBlock = (LENGTH / blockSize + 1);
    qMatrix[(y / blockSize) * wBlock + (x / blockSize)] = QP;

    if (((int)(LENGTH / blockSize)) * blockSize == LENGTH) wBlock = LENGTH;
    else wBlock = (LENGTH / blockSize + 1) * blockSize;
    //writing coeficients
    for (int i = y; i < (blockSize + y); ++i)
    {
        int r = i * wBlock;
        int v = (i - y) * blockSize - x;
        for (int j = x; j < (blockSize + x); ++j)
        {
            frameCoef[r + j] = residualBlock[v + j];
        }
    }

    int dataCount = 0;
    for (int i = 0; i < blockSize * blockSize; ++i)
    {
        if (residualBlock[i] != 0) dataCount++;
    }

    if (frameN == 0 || (frameN + 1) % IR == 0)
    {
        //dequantize and retransform
        dequanting(residualBlock, blockSize, QP);
        IDCT(residualBlock, blockSize);

        reverseSAD(predictedBlock, reconstructedBlock, residualBlock, blockSize);

        for (int i = y; i < (blockSize + y); ++i)
        {
            int r = (i - y) * blockSize - x;
            int h = i * LENGTH;
            for (int j = x; j < (blockSize + x); ++j)
            {
                if (i < HEIGHT && j < LENGTH)
                {
                    //remember reconstructed data
                    if (predictedBlock[r + j] < 0) predictedBlock[(i - y) * blockSize + j - x] = 0;
                    else if (predictedBlock[r + j] > 255) predictedBlock[r + j] = 255;
                    reconstructedFrame[h + j] = (unsigned char)predictedBlock[r + j];
                }
            }
        }
    }

    delete[] reconstructedBlock;
    delete[] predictedBlock;
    delete[] residualBlock;

    return dataCount;
}

int split(char* startFrame, char* previousFrame, char* reconstructedFrame,
    int* frameCoef, int* modeMatrix, int* qMatrix, int blockSize, int frameN, int task)
{
    int dataCount = 0;
    double frameVAR = variance(startFrame, pixels_on_video);
    for (int i = 0; i < HEIGHT; i += blockSize)
    {
        for (int j = 0; j < LENGTH; j += blockSize)
        {
            dataCount += splitBlockPadding(startFrame, previousFrame, reconstructedFrame,
                frameCoef, modeMatrix, qMatrix, blockSize, i, j, frameN, frameVAR, task);
        }
    }

    return dataCount;
}

void encoder(int blockSize, int task)
{
    const char* in_string   = "y_file.yuv";
    const char* mode_string = "mode_file.dat";
    const char* coef_string = "coef_file.dat";
    const char* q_string    = "q_file.dat";

    ofstream mode_file(mode_string, ios::out | ios::binary);
    ofstream q_file(q_string, ios::out | ios::binary);
    ofstream coef_file(coef_string, ios::out | ios::binary);
    ifstream in_file(in_string, ios::in | ios::binary);

    char* previousFrame      = new char[pixels_on_video];
    char* greyscaleFrame     = new char[pixels_on_video];
    char* reconstructedFrame = new char[pixels_on_video];

    int* modeMatrix;
    int* qMatrix;
    int hBlock, wBlock;
    if (((int)(HEIGHT / blockSize)) * blockSize == HEIGHT) hBlock = HEIGHT / blockSize;
    else hBlock = HEIGHT / blockSize + 1;

    if (((int)(LENGTH / blockSize)) * blockSize == LENGTH) wBlock = LENGTH / blockSize;
    else wBlock = LENGTH / blockSize + 1;

    modeMatrix  = new int[hBlock * wBlock];
    qMatrix     = new int[hBlock * wBlock];

    int* frameCoef = new int[hBlock * blockSize * wBlock * blockSize];

    int    frameCount = 0;
    double dataCount  = 0;

    while (frameCount < 1)
    {
        //read data
        for (int i = 0; i < pixels_on_video; ++i)
        {
            in_file.read(&greyscaleFrame[i], 1);
        }

        if (in_file.eof()) break;

        dataCount += split(greyscaleFrame, previousFrame, reconstructedFrame,
            frameCoef, modeMatrix, qMatrix, blockSize, frameCount, task);

        for (int i = 0; i < pixels_on_video; ++i)
        {
            previousFrame[i] = greyscaleFrame[i];
        }

        //write coficients
        for (int i = 0; i < hBlock * blockSize * wBlock * blockSize; ++i)
        {
            coef_file << frameCoef[i] << '\n';
        }

        //write prediction modes
        for (int i = 0; i < hBlock * wBlock; ++i)
        {
            mode_file << modeMatrix[i] << '\n';
            q_file << qMatrix[i] << '\n';
        }

        ++frameCount;
    }
    dataCount /= pixels_on_video;
    cout << "Data Compression: " << 1.0 - dataCount / frameCount << endl;

    delete[] previousFrame;
    delete[] frameCoef;
    delete[] modeMatrix;
    delete[] reconstructedFrame;
    delete[] greyscaleFrame;

    coef_file.close();
    mode_file.close();
    q_file.close();
    in_file.close();
}
