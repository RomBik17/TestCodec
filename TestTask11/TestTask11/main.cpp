
#include "encoder.h"
#include "decoder.h"
#include <chrono>
using namespace std;


int main()
{
    cout << "Choose:\n    1) encode\n    2) decode" << endl;
    int variant;
    cin >> variant;

    cout << "Input the size of block from 2 to 64:" << endl;
    int blockSize;
    cin >> blockSize;

    switch (variant)
    {
    case 1:
    {
        cout << "Input task:" << endl;
        int task;
        cin >> task;

        typedef chrono::seconds s;
        auto start = chrono::high_resolution_clock::now();
        encoder(blockSize, "y_file.yuv");
        decoder(blockSize, "decoded_y_file.yuv");
        if (task == 1) encoder(blockSize, "decoded_y_file.yuv");
        else if (task == 2) secondEncoder(blockSize, "decoded_y_file.yuv");
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<float> duration = end - start;
        cout << duration.count();
        break;
    }
    case 2:
    {
        decoder(blockSize, "decoded_y_file.yuv");
        break;
    }
    default:
        break;
    }

    cout << "Finished";
    return 0;
}