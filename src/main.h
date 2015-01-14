//standard include
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//for OpenCv
#include <cv.h>
#include <highgui.h>

//THF include & define
#define REDUCTION 1
#include "./THF/find.h"

//TCP_IP include & define
#include "./TCP_IP/client.h"

//LRF_SHOW include &define
#include "./LRF_Show/lrf_show.h"

//Prototype
void* lrf_cmd_thread(void* arg);
void* cam_thread(void* arg);


//for THF
IplImage* src_img;
IplImage* src_img_out;
IplImage* hsv_img;
IplImage* cut_img;
IplImage* cut_img_out;
IplImage* lrf_img[3];

CvCapture* cap;
lrf_cmd lrf_cmd_data_cpy[3];


double obj_stp[50];
double obj_first_stp[50];
double obj_last_stp[50];
long obj_data[50];
long obj_first_data[50];
long obj_last_data[50];
cutPoint *obj_x;
cutPoint *obj_y;
int obj_cnt;
int plate_deg[25];

//for TCP_IP
int sock[NUM_OF_TCP];
lrf_cmd lrf_cmd_data[3];

//for LRF_Show
const char* lrf_window_name[3] = {"LRF_TOP", "LRF_BOTTOM", "LRF_REAR"};
const long max_dist[3] = {2000,5000,1500};


//for main


//pthread ID
pthread_t lrf_cmd_thread_t;

//pthread mutex ID
static pthread_mutex_t lrf_cmd_mtx = PTHREAD_MUTEX_INITIALIZER;


