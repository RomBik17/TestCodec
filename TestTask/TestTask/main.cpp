
#include "frame_cut.h"
#include "grayscale.h"
using namespace std;


int main()
{
    cout << "Choose:\n    1) y420_To_PNG\n    2) y444_To_y420\n    3) greyscale video\n    4) split into blocks and count PSNR" << endl;
    int variant;
    cin >> variant;

    switch (variant)
    {
    case 1:
    {
        cout << "How much frames do you want to cut?" << endl;
        int n_frames;
        cin >> n_frames;
        y420_To_PNG(n_frames);
        break;
    }
    case 2:
    {
        y444_To_y420();
        break;
    }
    case 3:
    {
        extractY();
        break;
    }
    case 4:
    {
        cout << "Input the size of block from 2 to 64:" << endl;
        int blockSize;
        cin >> blockSize;
        framesplit(blockSize);
        break;
    }
    default:
        break;
    }

    cout << "Finished";
    return 0;
}