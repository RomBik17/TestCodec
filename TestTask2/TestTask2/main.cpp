
#include "grayscale.h"
using namespace std;


int main()
{
    cout << "Choose:\n    1) greyscale video\n    2) split into blocks and count PSNR\n    3) quant and count PSNR" << endl;
    int variant;
    cin >> variant;

    switch (variant)
    {
    case 1:
    {
        extractY();
        break;
    }
    case 2:
    {
        cout << "Input the size of block from 2 to 64:" << endl;
        int blockSize;
        cin >> blockSize;
        frameSplit(blockSize);
        break;
    }
    case 3:
    {
        cout << "Input the size of Q from 2 to 64:" << endl;
        int Q;
        cin >> Q;
        quantFrame(Q);
        break;
    }
    default:
        break;
    }

    cout << "Finished";
    return 0;
}