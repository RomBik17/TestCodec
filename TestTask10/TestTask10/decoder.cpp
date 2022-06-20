
#include "decoder.h"
using namespace std;

void reconstructedBlockRead(int* reconstructedBlock, int* decodedFrame, int* previousFrame,
    int mode, int blockSize, int y, int x)
{
    //by checking mode read the reconstructed block
    int recY;
    int tempX, tempY;
    if (mode == 0)
    {
        if (x >= blockSize)
        {
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                tempY = y + i;
                if (tempY >= HEIGHT) tempY = HEIGHT - 1;
                tempY *= LENGTH;
                for (int j = 0; j < blockSize; ++j)
                {
                    tempX = x - blockSize + j;
                    if (tempX >= LENGTH) tempX = LENGTH - 1;
                    reconstructedBlock[recY + j] = decodedFrame[tempY + tempX];
                }
            }
        }
        else
        {
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                for (int j = 0; j < blockSize; ++j)
                {
                    reconstructedBlock[recY + j] = 128;
                }
            }
        }
    }
    else if (mode == 1)
    {
        if (y >= blockSize)
        {
            int tempX, tempY;
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                tempY = y - blockSize + i;
                if (tempY >= HEIGHT) tempY = HEIGHT - 1;
                tempY *= LENGTH;
                for (int j = 0; j < blockSize; ++j)
                {
                    tempX = x + j;
                    if (tempX >= LENGTH) tempX = LENGTH - 1;
                    reconstructedBlock[recY + j] = decodedFrame[tempY + tempX];
                }
            }
        }
        else
        {
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                for (int j = 0; j < blockSize; ++j)
                {
                    reconstructedBlock[recY + j] = 128;
                }
            }
        }
    }
    else if (mode == 2)
    {
        for (int i = 0; i < blockSize; ++i)
        {
            recY = i * blockSize;
            tempY = y + i;
            if (tempY >= HEIGHT) tempY = HEIGHT - 1;
            tempY *= LENGTH;
            for (int j = 0; j < blockSize; ++j)
            {
                tempX = x + j;
                if (tempX >= LENGTH) tempX = LENGTH - 1;
                reconstructedBlock[recY + j] = previousFrame[tempY + tempX];
            }
        }
    }
    else if (mode == 3)
    {
        if (x >= blockSize)
        {
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                tempY = y + i;
                if (tempY >= HEIGHT) tempY = HEIGHT - 1;
                tempY *= LENGTH;
                for (int j = 0; j < blockSize; ++j)
                {
                    tempX = x - blockSize + j;
                    if (tempX >= LENGTH) tempX = LENGTH - 1;
                    reconstructedBlock[recY + j] = previousFrame[tempY + tempX];
                }
            }
        }
        else
        {
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                for (int j = 0; j < blockSize; ++j)
                {
                    reconstructedBlock[recY + j] = 128;
                }
            }
        }
    }
    else if (mode == 4)
    {
        if (x < LENGTH - blockSize)
        {
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                tempY = y + i;
                if (tempY >= HEIGHT) tempY = HEIGHT - 1;
                tempY *= LENGTH;
                for (int j = 0; j < blockSize; ++j)
                {
                    tempX = x + blockSize + j;
                    if (tempX >= LENGTH) tempX = LENGTH - 1;
                    reconstructedBlock[recY + j] = previousFrame[tempY + tempX];
                }
            }
        }
        else
        {
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                for (int j = 0; j < blockSize; ++j)
                {
                    reconstructedBlock[recY + j] = 128;
                }
            }
        }
    }
    else if (mode == 5)
    {
        if (y >= blockSize)
        {
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                tempY = y - blockSize + i;
                if (tempY >= HEIGHT) tempY = HEIGHT - 1;
                tempY *= LENGTH;
                for (int j = 0; j < blockSize; ++j)
                {
                    tempX = x + j;
                    if (tempX >= LENGTH) tempX = LENGTH - 1;
                    reconstructedBlock[recY + j] = previousFrame[tempY + tempX];
                }
            }
        }
        else
        {
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                for (int j = 0; j < blockSize; ++j)
                {
                    reconstructedBlock[recY + j] = 128;
                }
            }
        }
    }
    else if (mode == 6)
    {
        if (y < HEIGHT - blockSize)
        {
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                tempY = y + blockSize + i;
                if (tempY >= HEIGHT) tempY = HEIGHT - 1;
                tempY *=  LENGTH;
                for (int j = 0; j < blockSize; ++j)
                {
                    tempX = x + j;
                    if (tempX >= LENGTH) tempX = LENGTH - 1;
                    reconstructedBlock[recY + j] = previousFrame[tempY + tempX];
                }
            }
        }
        else
        {
            for (int i = 0; i < blockSize; ++i)
            {
                recY = i * blockSize;
                for (int j = 0; j < blockSize; ++j)
                {
                    reconstructedBlock[recY + j] = 128;
                }
            }
        }
    }
}

void blockDecoding(int* frameCoef, int* decodedFrame, int* previousFrame, int* modeMatrix, int* qMatrix, int blockSize, int y, int x)
{
    int* block = new int[blockSize * blockSize];
    int* reconstructedBlock = new int[blockSize * blockSize];
    int* residualBlock = new int[blockSize * blockSize];

    int wBlock;
    if (((int)(LENGTH / blockSize)) * blockSize == LENGTH) wBlock = LENGTH / blockSize;
    else wBlock = LENGTH / blockSize + 1;

    int QP = qMatrix[(y / blockSize) * wBlock + (x / blockSize)];
    int mode = modeMatrix[(y / blockSize) * wBlock + (x / blockSize)];

    wBlock *= blockSize;
    //read coeficients
    int tempX, tempY;
    for (int i = y; i < (blockSize + y); ++i)
    {
        int r = i * wBlock;
        int v = (i - y) * blockSize - x;
        for (int j = x; j < (blockSize + x); ++j)
        {
            residualBlock[v + j] = frameCoef[r + j];
        }
    }

    //read reconstructed block
    reconstructedBlockRead(reconstructedBlock, decodedFrame, previousFrame, mode, blockSize, y, x);

    //dequantize, retransform and reconstruct block
    dequanting(residualBlock, blockSize, QP);
    IDCT(residualBlock, blockSize);
    reverseSAD(block, reconstructedBlock, residualBlock, blockSize);

    for (int i = y; i < (blockSize + y); ++i)
    {
        for (int j = x; j < (blockSize + x); ++j)
        {
            int r = (i - y) * blockSize - x;
            int h = i * LENGTH;
            if (i < HEIGHT && j < LENGTH)
            {
                //if block is inside of frame then write the data
                if (block[r + j] < 0) block[r + j] = 0;
                else if (block[r + j] > 255) block[r + j] = 255;
                decodedFrame[h + j] = block[r + j];
            }
        }
    }

    delete[] block;
    delete[] residualBlock;
    delete[] reconstructedBlock;
}

//return no-zero data in frame
void split(int* frameCoef, int* decodedFrame, int* previousFrame, int* modeMatrix, int* qMatrix, int blockSize)
{

    for (int i = 0; i < HEIGHT; i += blockSize)
    {
        for (int j = 0; j < LENGTH; j += blockSize)
        {
            blockDecoding(frameCoef, decodedFrame, previousFrame, modeMatrix, qMatrix, blockSize, i, j);
        }
    }
}

void decoder(int blockSize)
{
    const char* in_string   = "coef_file.dat";
    const char* out_string  = "decoded_y_file.yuv";
    const char* mode_string = "mode_file.dat";
    const char* q_string    = "q_file.dat";
    const char* y_string    = "y_file.yuv";

    ifstream y_file(y_string, ios::in | ios::binary);
    ifstream mode_file(mode_string, ios::in | ios::binary);
    ifstream q_file(q_string, ios::in | ios::binary);
    ofstream decoded_file(out_string, ios::out | ios::binary);
    ifstream in_file(in_string, ios::in | ios::binary);

    int* decodedFrame  = new int[pixels_on_video];
    int* previousFrame = new int[pixels_on_video];
    int* originalFrame = new int[pixels_on_video];


    int* modeMatrix;
    int* qMatrix;
    int hBlock, wBlock;
    if (((int)(HEIGHT / blockSize)) * blockSize == HEIGHT) hBlock = HEIGHT / blockSize;
    else hBlock = HEIGHT / blockSize + 1;

    if (((int)(LENGTH / blockSize)) * blockSize == LENGTH) wBlock = LENGTH / blockSize;
    else wBlock = LENGTH / blockSize + 1;

    modeMatrix = new int[hBlock * wBlock];
    qMatrix = new int[hBlock * wBlock];

    int* frameCoef;
    frameCoef = new int[hBlock * blockSize * wBlock * blockSize];

    int frameCount = 0;
    double psnrSum = 0.0;
    while (true)
    {
        for (int i = 0; i < pixels_on_video; ++i)
        {
            char a;
            y_file.read(&a, 1);
            originalFrame[i] = (unsigned char)a;
        }
        //read prediction modes
        for (int i = 0; i < hBlock * wBlock; ++i)
        {
            mode_file >> modeMatrix[i];
            q_file >> qMatrix[i];
        }

        if (mode_file.eof()) break;

        //read coeficients
        for (int i = 0; i < hBlock * blockSize * wBlock * blockSize; ++i)
        {
            in_file >> frameCoef[i];
        }

        split(frameCoef, decodedFrame, previousFrame, modeMatrix, qMatrix, blockSize);
        psnrSum += PSNR(originalFrame, decodedFrame);

        //write decoded frame
        for (int i = 0; i < pixels_on_video; ++i)
        {
            previousFrame[i] = decodedFrame[i];
            char a = (unsigned char)decodedFrame[i];
            decoded_file.write(&a, 1);
        }

        ++frameCount;
    }

    double averagePSNR = psnrSum / frameCount;
    cout << "average PSNR among frames:  " << averagePSNR << " dB" << endl;

    delete[] modeMatrix;
    delete[] decodedFrame;
    delete[] previousFrame;
    delete[] frameCoef;

    y_file.close();
    mode_file.close();
    q_file.close();
    decoded_file.close();
    in_file.close();
}
