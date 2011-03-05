#pragma once
#include <math.h>

double const PI = 2 * acos(0.0);

#define AF_LEVEL 16.0
#define NUM_MULTISAMPLES 4

#define WHEEL_ZOOM_FACTOR -0.1
//elevation im bogenmass
#define MIN_ELEVATION 0.1
#define MIN_DISTANCE 3.0
#define MAX_DISTANCE 50.0

#define MAP_DIRECTORY_TEXTURES "textures/"
#define MAP_DIRECTORY_MODELS   "models/"

#define EVENT_PAUSE 1
#define EVENT_INCREASE_SPEED 2
#define EVENT_NORMALIZE_SPEED 3
#define EVENT_DECREASE_SPEED 4