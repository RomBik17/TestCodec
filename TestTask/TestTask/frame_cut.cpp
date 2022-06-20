
#include "frame_cut.h"
using namespace std;

void YUV_to_RGB(char* y444_Frame, RGB_Triplet* rgb_frame)
{
    for (int i = 0; i < pixels_on_video; ++i)
    {
        int Y = (unsigned char)y444_Frame[i];
        int Cb = (unsigned char)y444_Frame[i + pixels_on_video];
        int Cr = (unsigned char)y444_Frame[i + 2 * pixels_on_video];

        //YCbCr to RGB
        rgb_frame[i].r = Y + 1.402 * (Cr - 128);
        rgb_frame[i].b = Y + 1.772 * (Cb - 128);
        rgb_frame[i].g = Y - 0.344 * (Cb - 128) - 0.714 * (Cr - 128);

        //Prevent colour distortions in rgb image
        if (rgb_frame[i].r < 0) rgb_frame[i].r = 0;
        else if (rgb_frame[i].r > 255) rgb_frame[i].r = 255;
        if (rgb_frame[i].g < 0) rgb_frame[i].g = 0;
        else if (rgb_frame[i].g > 255) rgb_frame[i].g = 255;
        if (rgb_frame[i].b < 0) rgb_frame[i].b = 0;
        else if (rgb_frame[i].b > 255) rgb_frame[i].b = 255;
    }
}

void saveRGB_to_PNG(std::string file_name, RGB_Triplet* rgb_frame)
{
    sf::Image img;
    img.create(LENGTH, HEIGHT, sf::Color::White);
    for (int i = 0; i < HEIGHT; ++i)
    {

        for (int j = 0; j < LENGTH; ++j)
        {
            sf::Uint16 x = j;
            sf::Uint16 y = i;
            sf::Color col(rgb_frame[LENGTH * i + j].r, rgb_frame[LENGTH * i + j].g, rgb_frame[LENGTH * i + j].b);
            img.setPixel(x, y, col);
        }
    }
    img.saveToFile(file_name);
}

void upscaleChroma(char* y420_Frame, int& y420_index, char* y444_Frame, int& y444_index)
{

    for (int k = 0; k < HEIGHT / 2; ++k)
    {
        for (int i = 0; i < LENGTH / 2; ++i)
        {
            y444_Frame[y444_index + 2 * i] = y420_Frame[y420_index + i];
        }
        for (int i = 1; i < LENGTH; i += 2)
        {
            y444_Frame[y444_index + i] = y444_Frame[y444_index + i - 1];
        }
        y444_index += LENGTH;
        y420_index += LENGTH / 2;
        for (int i = 0; i < LENGTH; ++i)
        {
            y444_Frame[y444_index + i] = y444_Frame[y444_index + i - LENGTH];
        }
        y444_index += LENGTH;
    }
}

void upscale(char* y420_Frame, char* y444_Frame)
{

    for (int i = 0; i < pixels_on_video; ++i)
    {
        y444_Frame[i] = y420_Frame[i];
    }

    int y444_index = pixels_on_video;
    int y420_index = pixels_on_video;
    upscaleChroma(y420_Frame, y420_index, y444_Frame, y444_index);
    upscaleChroma(y420_Frame, y420_index, y444_Frame, y444_index);

}

void downscaleChroma(char* y444_Frame, int& y444_index, char* y420_Frame, int& y420_index)
{
    for (int k = 0; k < HEIGHT / 2; ++k)
    {
        for (int i = 0; i < LENGTH / 2; ++i)
        {
            y420_Frame[y420_index + i] = y444_Frame[y444_index + i * 2];
        }
        y444_index += 2 * LENGTH;
        y420_index += LENGTH / 2;
    }
}

void downscale(char* y444_Frame, char* y420_Frame)
{
    for (int i = 0; i < pixels_on_video; ++i)
    {
        y420_Frame[i] = y444_Frame[i];
    }

    int y444_index = pixels_on_video;
    int y420_index = pixels_on_video;
    downscaleChroma(y444_Frame, y444_index, y420_Frame, y420_index);
    downscaleChroma(y444_Frame, y444_index, y420_Frame, y420_index);
}

void y420_To_PNG(int n_frames)
{
    const char* in_string = "akiyo_cif.y4m";
    ifstream in_file(in_string, ios::in | ios::binary);

    char* y420_Frame = new char[y420_FrameSize];
    char* y444_Frame = new char[y444_FrameSize];
    RGB_Triplet* rgb_frame = new RGB_Triplet[pixels_on_video];

    char letter = NULL;
    while (letter != '\n')
    {
        in_file.read(&letter, 1);
    }

    std::string png_filename = "a.png";

    char* frame_indicator = new char[6];
    for (int i = 0; i < n_frames; i++)
    {
        in_file.read(frame_indicator, 6);

        in_file.read(y420_Frame, y420_FrameSize);
        upscale(y420_Frame, y444_Frame);
        YUV_to_RGB(y444_Frame, rgb_frame);
        saveRGB_to_PNG(png_filename, rgb_frame);
        png_filename.insert(0, "a");
    }

    delete[] frame_indicator;
    delete[] y420_Frame;
    delete[] y444_Frame;
    delete[] rgb_frame;

    in_file.close();
}

void y444_To_y420()
{
    const char* in_string = "park_joy_444_720p50.y4m";
    const char* out_string = "out_big_420.y4m";

    ofstream out_file(out_string, ios::out | ios::binary);
    ifstream in_file(in_string, ios::in | ios::binary);

    char* y420_Frame = new char[y420_FrameSize];
    char* y444_Frame = new char[y444_FrameSize];

    char letter = NULL;
    while (letter != '\n' && letter != 'C')
    {
        in_file.read(&letter, 1);
        if (letter != '\n' && letter != 'C') out_file.write(&letter, 1);
    }

    if (letter == 'C')
    {
        while (letter != '\n')
        {
            in_file.read(&letter, 1);
        }
    }

    out_file.write(" C420\n", 6);

    char* frame_indicator = new char[6];
    while (true)
    {
        in_file.read(frame_indicator, 6);
        if (in_file.eof()) break;
        out_file.write(frame_indicator, 6);

        in_file.read(y444_Frame, y444_FrameSize);
        downscale(y444_Frame, y420_Frame);
        out_file.write(y420_Frame, y420_FrameSize);
    }

    delete[] frame_indicator;
    delete[] y420_Frame;
    delete[] y444_Frame;

    out_file.close();
    in_file.close();
}