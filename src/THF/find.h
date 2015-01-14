#ifndef __FIND__
#define __FIND__

#define CAM_HEIGHT 931 //height of camera [mm]
#define LRF_HEIGHT 860 //height of LRF [mm]
#define CAM_LRF_DIST 20 //distance between cam & lrf [mm]
#define CAM_DEG_HEIGHT 30 //height degree of camera 
#define CAM_DEG_WIDTH 50 //width degree of camera
#define HEIGHT_PX 480 //height pixs of camera
#define WIDTH_PX 640 //widrh pixs of camera

typedef struct{
  int first;
  int last;
} cutPoint;
 
typedef struct{
  long lrf_data_max;
  long lrf_data[1081];
} lrf_cmd;

extern double obj_stp[50];
extern double obj_first_stp[50];
extern double obj_last_stp[50];
extern long obj_data[50];
extern long obj_first_data[50];
extern long obj_last_data[50];
extern cutPoint *obj_x; 
extern cutPoint *obj_y;
extern int obj_cnt;
extern int plate_deg[25];


/* int find_orange(IplImage* input_img, IplImage* output_img); */
int find_yellow(IplImage* input_img, IplImage* output_img);
int find_satuation(IplImage* input_img, IplImage* output_img);
int find_value(IplImage* input_img, IplImage* output_img);

//int lrf_prepoll(lrf_cmd lrf_cmd);
int cut_vision(IplImage* input_img, IplImage* output_img, lrf_cmd lrf_cmd);
//int color_poll(IplImage* input_img);
void vote_compiler(IplImage* input_img);
//void test_vision(IplImage* input_img, IplImage* output_img);

#endif
