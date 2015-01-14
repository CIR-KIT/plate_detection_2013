#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <cv.h>
#include <highgui.h>

#include "find.h"

#define PI 3.141592

#define MAX_RATE 1.3

#define DECIDE 0.7


static int hue[1500][1500];
static int saturation[1500][1500];
static int value[1500][1500];

static int x;
static int y;
static int tmp;
static int next_tmp;

int *color_vote;
