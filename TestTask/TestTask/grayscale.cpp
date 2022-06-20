
#include "grayscale.h"
#include <math.h>
using namespace std;

double PSNR(char** greyscaleFrame, char** splitFrame)
{
    int sum = 0;
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

void blockPadding(char** greyscaleFrame, char** splitFrame, int blockSize, int x, int y)
{
    int sum = 0;
    int tempX, tempY;
    for (int i = x; i < (blockSize + x); ++i)
    {
        for (int j = y; j < (blockSize + y); ++j)
        {
            if (blockSize + x <= HEIGHT && blockSize + y <= LENGTH) 
            {
                sum += (unsigned char)greyscaleFrame[i][j];
            }
            else
            {
                if (i >= HEIGHT) tempY = HEIGHT - 1;
                else tempY = i;
                if (j >= LENGTH) tempX = LENGTH - 1;
                else tempX = j;
                sum += (unsigned char)greyscaleFrame[tempY][tempY];
            }
        }
    }

    int average = sum / (blockSize * blockSize);
    for (int i = x; i < (blockSize + x); ++i)
    {
        for (int j = y; j < (blockSize + y); ++j)
        {
            if (blockSize + x <= HEIGHT && blockSize + y <= LENGTH)
            {
                splitFrame[i][j] = (unsigned char)average;
            }
            else
            {
                if (i >= HEIGHT) tempY = HEIGHT - 1;
                else tempY = i;
                if (j >= LENGTH) tempX = LENGTH - 1;
                else tempX = j;
                splitFrame[tempY][tempX] = (unsigned char)average;
            }
            
        }
    }
}

void split(char** greyscaleFrame, char** splitFrame, int blockSize)
{
    for (int i = 0; i < HEIGHT; i+=blockSize)
    {
        for (int j = 0; j < LENGTH; j+=blockSize)
        {
            blockPadding(greyscaleFrame, splitFrame, blockSize, i, j);
        }
    }
}

void framesplit(int blockSize)
{
    const char* in_string = "y_file.yuv";
    const char* out_string = "split_y_file.yuv";

    ofstream out_file(out_string, ios::out | ios::binary);
    ifstream in_file(in_string, ios::in | ios::binary);

    char** greyscaleFrame;
    char** splitFrame;
    greyscaleFrame = new char* [HEIGHT];
    for (int i = 0; i < HEIGHT; ++i)
    {
        greyscaleFrame[i] = new char[LENGTH];
    }
    splitFrame = new char* [HEIGHT];
    for (int i = 0; i < HEIGHT; ++i)
    {
        splitFrame[i] = new char[LENGTH];
    }

    int frameCount = 0;
    double psnrSum = 0;
    while (true)
    {
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

        split(greyscaleFrame, splitFrame, blockSize);
        psnrSum += PSNR(greyscaleFrame, splitFrame);

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
    const char* in_string = "akiyo_cif.y4m";
    const char* out_string = "y_file.yuv";

    ofstream out_file(out_string, ios::out | ios::binary);
    ifstream in_file(in_string, ios::in | ios::binary);

    char* y420_Frame = new char[y420_FrameSize];
    char* greyscaleFrame = new char[pixels_on_video];

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
    while(true)
    {
        in_file.read(frame_indicator, 6);
        if (in_file.eof()) break;

        in_file.read(y420_Frame, y420_FrameSize);
        for (int i = 0; i < pixels_on_video; ++i)
        {
            greyscaleFrame[i] = y420_Frame[i];
        }
        out_file.write(greyscaleFrame, pixels_on_video);
    }

    delete[] frame_indicator;
    delete[] y420_Frame;
    delete[] greyscaleFrame;

    out_file.close();
    in_file.close();
}
