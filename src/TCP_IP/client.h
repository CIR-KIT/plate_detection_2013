#ifndef __CLIENT__
#define __CLIENT__

#define NUM_OF_TCP 3
#define THF_CMD_FUCTOR 8
#define NUM_OF_MAX_TARGET 4

typedef struct{
  int use;            //use target_human_finder or not
  int area_number;    //area number
  int plate;          //find plate flag
  int human;          //find human flag
  int num_of_plate;   //number of found plate
  int num_of_human;   //number of found human
  int plate_dir[4];   //degree(direction) of the plate 
  int human_dir[4];   //degree(direction) of the human
} thf_cmd;

#ifndef __FIND__
typedef struct{
  long lrf_data_max;
  long lrf_data[1081];
} lrf_cmd;
#endif

extern int sock[NUM_OF_TCP];

int client_setup(void);
void tcp_retry(const char* message);
void tcp_error(const char* message);
int SplitString(char *buf[], const char *str, const char *delimiter);
int char2lrf_cmd(lrf_cmd* lrf_cmd_data, char* trans_message[], const char* message);

#endif
