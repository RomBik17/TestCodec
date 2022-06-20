#pragma once

#define Y420    1.5
#define Y444    3

//video resolution
#define HEIGHT  288
#define LENGTH  352

const int pixels_on_video = HEIGHT * LENGTH;
const int y420_FrameSize = pixels_on_video * Y420;
const int y444_FrameSize = pixels_on_video * Y444;