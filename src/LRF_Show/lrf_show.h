#ifndef __LRF_SHOW__
#define __LRF_SHOW__

#define LRF_WINDOW_SIZE 300

#ifndef __FIND__
typedef struct{
  long lrf_data_max;
  long lrf_data[1081];
} lrf_cmd;
#endif

void draw_fundamental(IplImage* img);
void draw_object(IplImage* img, lrf_cmd lrf_cmd_data, const long max_dist);
void lrf_show(IplImage* img, lrf_cmd lrf_cmd_data, const long max_dist, const char* lrf_window_name);

#endif
