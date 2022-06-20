
#include "video_constants.h"
#include <math.h>

#define CONST_BITS 13
#define ONE	((int) 1)
#define CONST_SCALE (ONE << CONST_BITS)
#define FIX(x)	((int) ((x) * CONST_SCALE))
#define DESCALE(x) ((int)((x) / CONST_SCALE))

double variance(int* array, int N);
double variance(char* array, int N);
int VAQ(double blockVAR, double frameVAR);
double PSNR(int* startFrame, int* dumpedFrame);
void quanting(int* block, int blockSize, int QP);
void dequanting(int* block, int blockSize, int QP);
void IDCT(int* block, int blockSize);
void DCT(int* block, int blockSize);
void SAD(int* currentBlock, int* predictedBlock, int* sadBlock, int blockSize);
void reverseSAD(int* currentBlock, int* predictedBlock, int* sadBlock, int blockSize);
