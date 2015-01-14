#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>

#include "lrf_show.h"

#define FIRST_RANGE -45
#define LAST_RANGE 225
#define PI 3.141592

CvSize ellipse;
CvPoint
lo_pt1, lo_pt2, 
  la_pt1, la_pt2, 
  slant_pt1, slant_pt2,
  limit_pt1, limit_pt2,
  object_pt, 
  center_pt;
