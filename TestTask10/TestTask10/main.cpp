
#include "encoder.h"
#include "decoder.h"
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
        encoder(blockSize);
        break;
    }
    case 2:
    {
        decoder(blockSize);
        break;
    }
    default:
        break;
    }

    cout << "Finished";
    return 0;
}