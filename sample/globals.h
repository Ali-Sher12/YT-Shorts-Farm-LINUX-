#pragma once
#include <vector>
using namespace sf;

int cell_size = 64;
float PI = 3.14159;

float SCREEN_HEIGHT = 600;
float SCREEN_WIDTH = 800;

float render_distance = 1200; // (float)INT_MAX;
float render_distance_wall = (float)INT_MAX;
float FOV = 2*PI/9;

int total_wall_textures_per_level = 7;// for columns
//char tex_identifiers[8] = { '#', '@', '*', 'a', 'b', 'f', 'e', 'd' };
