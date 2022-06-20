#pragma once
#include<SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "video_settings.h"

void y420_To_PNG(int n_frames);
void y444_To_y420();