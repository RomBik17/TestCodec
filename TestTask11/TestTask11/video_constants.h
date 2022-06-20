#pragma once

#define PI		3.14159265359
#define E		2.71828

#define Q		3
#define D		1.5
#define IR		4

#define Y420    1.5
#define Y444    3

//video resolution
#define HEIGHT  288
#define LENGTH  352

const int pixels_on_video = HEIGHT * LENGTH;
const int y420_FrameSize = pixels_on_video * Y420;
const int y444_FrameSize = pixels_on_video * Y444;