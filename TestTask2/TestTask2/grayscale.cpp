
#include "grayscale.h"
#include <math.h>
using namespace std;

double PSNR(char** greyscaleFrame, char** splitFrame)
{
    double sum = 0;
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < LENGTH; ++j)
        {
            int delta = greyscaleFrame[i][j] - splitFrame[i][j];
            sum += pow(delta, 2);
        }
    }
    double MSE = sum / pixels_on_video;
    const int MAX = 255;
    double psnr = 10 * log10(pow(MAX, 2) / MSE);
    return psnr;
}

void splitBlockPadding(char** greyscaleFrame, char** splitFrame, int blockSize, int y, int x)
{
    int sum = 0;
    int tempX, tempY;
    for (int i = y; i < (blockSize + y); ++i)
    {
        for (int j = x; j < (blockSize + x); ++j)
        {
            if (blockSize + y <= HEIGHT && blockSize + x <= LENGTH)
            {
                //if block is inside of frame then read the data and calculate the sum
                sum += (unsigned char)greyscaleFrame[i][j];
            }
            else
            {
                //else we have to read the last data from raw or column
                if (i >= HEIGHT) tempY = HEIGHT - 1;
                else tempY = i;
                if (j >= LENGTH) tempX = LENGTH - 1;
                else tempX = j;
                sum += (unsigned char)greyscaleFrame[tempY][tempX];
            }
        }
    }

    int average = sum / (blockSize * blockSize);
    for (int i = y; i < (blockSize + y); ++i)
    {
        for (int j = x; j < (blockSize + x); ++j)
        {
            if (i < HEIGHT && j < LENGTH)
            {
                //if block is inside of frame then write the data 
                splitFrame[i][j] = (unsigned char)average;
            }

        }
    }
}

void split(char** greyscaleFrame, char** splitFrame, int blockSize)
{
    for (int i = 0; i < HEIGHT; i += blockSize)
    {
        for (int j = 0; j < LENGTH; j += blockSize)
        {
            splitBlockPadding(greyscaleFrame, splitFrame, blockSize, i, j);
        }
    }
}

void quanting(char** greyscaleFrame, char** splitFrame, int Q)
{
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < LENGTH; ++j)
        {
            //calculate the nearest multiple of Q
            int k = greyscaleFrame[i][j] / Q;
            splitFrame[i][j] = k * Q;
        }
    }
}

void quantFrame(int Q)
{
    const char* in_string = "y_file.yuv";
    const char* out_string = "quant_y_file.yuv";

    ofstream out_file(out_string, ios::out | ios::binary);
    ifstream in_file(in_string, ios::in | ios::binary);

    char** greyscaleFrame;
    char** quantFrame;
    greyscaleFrame = new char* [HEIGHT];
    quantFrame = new char* [HEIGHT];
    for (int i = 0; i < HEIGHT; ++i)
    {
        greyscaleFrame[i] = new char[LENGTH];
        quantFrame[i] = new char[LENGTH];
    }

    int frameCount = 0;
    double psnrSum = 0;
    while (true)
    {
        //read data
        for (int i = 0; i < HEIGHT; ++i)
        {
            for (int j = 0; j < LENGTH; ++j)
            {
                char a;
                in_file.read(&a, 1);
                greyscaleFrame[i][j] = a;
            }
        }

        if (in_file.eof()) break;

        //Quant frames and found PSNR
        quanting(greyscaleFrame, quantFrame, Q);
        psnrSum += PSNR(greyscaleFrame, quantFrame);

        //write new data
        for (int i = 0; i < HEIGHT; ++i)
        {
            for (int j = 0; j < LENGTH; ++j)
            {
                char a = quantFrame[i][j];
                out_file.write(&a, 1);
            }
        }
        ++frameCount;
    }

    double averagePSNR = psnrSum / frameCount;
    cout << "average PSNR among frames:  " << averagePSNR << " dB" << endl;

    for (int i = 0; i < HEIGHT; ++i)
    {
        delete[] greyscaleFrame[i];
        delete[] quantFrame[i];
    }

    delete[] greyscaleFrame;
    delete[] quantFrame;

    out_file.close();
    in_file.close();
}

void frameSplit(int blockSize)
{
    const char* in_string = "y_file.yuv";
    const char* out_string = "split_y_file.yuv";

    ofstream out_file(out_string, ios::out | ios::binary);
    ifstream in_file(in_string, ios::in | ios::binary);

    char** greyscaleFrame;
    char** splitFrame;
    greyscaleFrame = new char* [HEIGHT];
    splitFrame = new char* [HEIGHT];
    for (int i = 0; i < HEIGHT; ++i)
    {
        greyscaleFrame[i] = new char[LENGTH];
        splitFrame[i] = new char[LENGTH];
    }

    int frameCount = 0;
    double psnrSum = 0;
    while (true)
    {
        //read data
        for (int i = 0; i < HEIGHT; ++i)
        {
            for (int j = 0; j < LENGTH; ++j)
            {
                char a;
                in_file.read(&a, 1);
                greyscaleFrame[i][j] = a;
            }
        }

        if (in_file.eof()) break;

        //split video into blocks and calculate PSNR
        split(greyscaleFrame, splitFrame, blockSize);
        psnrSum += PSNR(greyscaleFrame, splitFrame);

        //write new data
        for (int i = 0; i < HEIGHT; ++i)
        {
            for (int j = 0; j < LENGTH; ++j)
            {
                char a = splitFrame[i][j];
                out_file.write(&a, 1);
            }
        }
        ++frameCount;
    }

    double averagePSNR = psnrSum / frameCount;
    cout << "average PSNR among frames:  " << averagePSNR << " dB" << endl;

    for (int i = 0; i < HEIGHT; ++i)
    {
        delete[] greyscaleFrame[i];
        delete[] splitFrame[i];
    }

    delete[] greyscaleFrame;
    delete[] splitFrame;

    out_file.close();
    in_file.close();
}

void extractY()
{
    const char* in_string = "coastguard_cif.y4m";
    const char* out_string = "y_file.yuv";

    ofstream out_file(out_string, ios::out | ios::binary);
    ifstream in_file(in_string, ios::in | ios::binary);

    char* y420_Frame = new char[y420_FrameSize];
    char* greyscaleFrame = new char[pixels_on_video];

    //read the first string of settings in start video
    char letter = NULL;
    while (letter != '\n' && letter != 'C')
    {
        in_file.read(&letter, 1);
    }

    if (letter == 'C')
    {
        while (letter != '\n')
        {
            in_file.read(&letter, 1);
        }
    }

    char* frame_indicator = new char[6];
    while (true)
    {
        //read frame indicator from y4m
        in_file.read(frame_indicator, 6);
        if (in_file.eof()) break;

        //read YUV data and copy Y-data from frame
        in_file.read(y420_Frame, y420_FrameSize);
        for (int i = 0; i < pixels_on_video; ++i)
        {
            greyscaleFrame[i] = y420_Frame[i];
        }
        //write only Y-data to yuv file
        out_file.write(greyscaleFrame, pixels_on_video);
    }

    delete[] frame_indicator;
    delete[] y420_Frame;
    delete[] greyscaleFrame;

    out_file.close();
    in_file.close();
}