#include "./_find.h"

int lrf_prepoll(lrf_cmd lrf_cmd){
 
  int i;               //for文用
  int step_cnt = 0;         //step counter
  int obj_cnt_local = 0;           //object_deg[], object_data[] counter && find flag
  double width[2];        //width of the object
  int first_step = 0;  //first step of the object
  int last_step = 0;
  long first_data = 0; //first distance of the object
  long last_data = 0;  //last distance of the object

  /* カメラの画角分だけLRFデータを取得し、
     15[m]以内に物体が有ればその幅を検知し、
     物体の存在する位置をグローバル変数 object_deg[]に返す */

  for(i=440; i<=640; i++){
    if(lrf_cmd.lrf_data[i] < 15000){
      width[0] = sqrt(pow(lrf_cmd.lrf_data[i],2)+pow(lrf_cmd.lrf_data[i-1],2) - 2*(double)lrf_cmd.lrf_data[i]*(double)lrf_cmd.lrf_data[i-1]*cos(0.25*(double)PI/180));
      width[1] = sqrt(pow(lrf_cmd.lrf_data[i],2)+pow(lrf_cmd.lrf_data[i+1],2) - 2*(double)lrf_cmd.lrf_data[i]*(double)lrf_cmd.lrf_data[i+1]*cos(0.25*(double)PI/180));
      if(width[0] > 150){
	first_data = lrf_cmd.lrf_data[i];
	first_step = i;
	for(;i<=640;i++){
	  width[0] = sqrt(pow(lrf_cmd.lrf_data[i],2) + pow(lrf_cmd.lrf_data[i+1],2) - 2*(double)lrf_cmd.lrf_data[i]*(double)lrf_cmd.lrf_data[i+1]*cos(0.25*(double)PI/180));
	  if(width[0] > 150){
	    last_data = lrf_cmd.lrf_data[i];
	    last_step = i;
	    break;
	  }
	  else  step_cnt++;
	}
	width[0] = sqrt(pow(first_data,2) + pow(last_data,2) - 2*(double)first_data*(double)last_data*cos((double)step_cnt*(double)PI/4/180));
	if(200 <width[0] && width[0] < 500){
	  obj_stp[obj_cnt_local] = (float)first_step + (float)step_cnt/2;
	  obj_first_stp[obj_cnt_local] = first_step;
	  obj_last_stp[obj_cnt_local] = last_step;
	  obj_data[obj_cnt_local] = lrf_cmd.lrf_data[first_step + step_cnt/2];
	  obj_first_data[obj_cnt_local] = lrf_cmd.lrf_data[first_step];
	  obj_last_data[obj_cnt_local] = lrf_cmd.lrf_data[last_step];	  
	  obj_cnt_local++;
	}
	else ;
	first_step = 0;
	last_step = 0;
	first_data = 0;
	last_data = 0;
	step_cnt = 0;
      }
    }
    else ;
  }

  free(obj_x);
  free(obj_y);
 
  obj_x=(cutPoint*)calloc(obj_cnt_local,sizeof(cutPoint));
  if (obj_x == NULL) {
    perror("calloc : at cut_vision()->obj_x");
    exit(-1);
  }
  obj_y=(cutPoint*)calloc(obj_cnt_local,sizeof(cutPoint));
  if (obj_y == NULL) {
    perror("calloc : at cut_vision()->obj_y");
    exit(-1);
  }

  return obj_cnt_local;
}

int cut_vision(IplImage* input_img, IplImage* output_img, lrf_cmd lrf_cmd)
{
  int i;
  double denominator, numerator, alpha;
  
  cvZero(output_img);
 
  obj_cnt = lrf_prepoll(lrf_cmd);
 
  for(i=0; i<obj_cnt; i++){
    //x.last
    obj_x[i].last  = WIDTH_PX - (float)WIDTH_PX*((float)(obj_first_stp[i]-440)/200);

    //x.first
    obj_x[i].first  = WIDTH_PX - (float)WIDTH_PX*((float)(obj_last_stp[i]-440)/200);

    //y.last
    denominator = (double)obj_data[i]*sin((double)(obj_stp[i]/4-45)*(double)PI/180) + (double)CAM_LRF_DIST;
    numerator = (double)CAM_HEIGHT - 540;
    alpha = ((double)PI/12) - atan(numerator/denominator);
    obj_y[i].last = HEIGHT_PX*(1 - alpha/(PI/6));
   
    //y.first
    denominator = (double)obj_data[i]*sin((double)(obj_stp[i]/4-45)*(double)PI/180) + (double)CAM_LRF_DIST;
    numerator = 1340 - (double)CAM_HEIGHT;
    alpha = ((double)PI/12) + atan(numerator/denominator);
    obj_y[i].first = HEIGHT_PX*(1 - alpha/(PI/6));
 
  }

  for(y=0; y<input_img->height; y++){
    for(i=obj_cnt-1; i>=0; i--){
      for(x=0; x<input_img->width-1; x++){

	tmp = input_img->widthStep*y+(x*3);

	hue[x][y] = input_img->imageData[tmp+0];
	saturation[x][y] = input_img->imageData[tmp+1];
	value[x][y] = input_img->imageData[tmp+2];
  
	if(hue[x][y] < 0)
	  hue[x][y] += 180;
	if(saturation[x][y]< 0)
	  saturation[x][y] += 255;
	if(value[x][y] < 0)
	  value[x][y] += 255;
           
	if((obj_x[i].first <= x) && (x <=obj_x[i].last)){
	  if((obj_y[i].first <= y) && (y <= obj_y[i].last)){
	    output_img->imageData[tmp+0] = hue[x][y];
	    output_img->imageData[tmp+1] = saturation[x][y];
	    output_img->imageData[tmp+2] = value[x][y];
	  }
	}
      }
    }
  }

  return 1;
}

int color_poll(IplImage* input_img)
{
  long *color_cnt = 0; 
  int i = 0, j = 0;

  color_cnt = (long*)calloc(obj_cnt,sizeof(long));
  if(color_cnt == NULL){
    perror("calloc : at color_vote()->color_cnt");
    exit(-1);
  }

  for(y=0; y<input_img->height; y++){
    for(i=obj_cnt-1; i>=0; i--){
      for(x=0; x<input_img->width; x++){
	tmp = input_img->widthStep*y+(x*3);

	hue[x][y] = input_img->imageData[tmp+0];
	saturation[x][y] = input_img->imageData[tmp+1];
	value[x][y] = input_img->imageData[tmp+2];

	if(hue[x][y] < 0)
	  hue[x][y] += 180;
	if(saturation[x][y]< 0)
	  saturation[x][y] += 255;
	if( value[x][y] < 0)
	  value[x][y] += 255;
       
	if((obj_x[i].first <= x) && (x <= obj_x[i].last)){
	  if((obj_y[i].first <= y) && (y <= obj_y[i].last)){
	    if((10 < hue[x][y]) && (hue[x][y] < 30)){
	      color_cnt[i]++;
	    }
	  }
	}
      }
    }
  }

  for(i=0; i<obj_cnt; i++){
    if(color_cnt[i] > ((obj_x[i].last-obj_x[i].first)*(obj_y[i].last-obj_y[i].first)*DECIDE)){
      for(j=0; j<25; j++){
	if( ((65+j*2)<(double)(obj_stp[i]/4-45)) && ((double)(obj_stp[i]/4-45)<(65+(j+1)*2)) )
	  plate_deg[j]++;
      }
    }
  }

  free(color_cnt);
 
  return 1;
}

void vote_compiler(IplImage* input_img)
{
  int i;

  for(i=0; i<5; i++){
    color_poll(input_img);
  } 
  for(i=0; i<25; i++){
    if(plate_deg[i] > 3){
      printf("Target is %d[deg]\t", 65+i*2);
    }
    plate_deg[i] = 0;
  }
  puts("");
}
 

int find_satuation(IplImage* input_img, IplImage* output_img)
{

  for(y=0; y<input_img->height; y++){
    for(x=0; x<input_img->width-1; x++){
      tmp = input_img->widthStep*y+(x*3);
      next_tmp = input_img->widthStep*y+((x+1)*3);

      hue[x][y] = input_img->imageData[tmp+0];
      saturation[x][y] = input_img->imageData[tmp+1];     
      value[x][y] = input_img->imageData[tmp+2];
      hue[x+1][y] = input_img->imageData[next_tmp+0];
      saturation[x+1][y] = input_img->imageData[next_tmp+1];     
      value[x+1][y] = input_img->imageData[next_tmp+2];

      if(hue[x][y] < 0)
	hue[x][y] += 180;
      if(saturation[x][y]< 0)
	saturation[x][y] += 255;
      if( value[x][y] < 0)
	value[x][y] += 255;

      if(saturation[x][y] > 123){
	output_img->imageData[tmp+0] = input_img->imageData[tmp+0];
	output_img->imageData[tmp+1] = input_img->imageData[tmp+1];
	output_img->imageData[tmp+2] = input_img->imageData[tmp+2];
      }
      else {
	output_img->imageData[tmp+0] = 0;
	output_img->imageData[tmp+1] = 0;
	output_img->imageData[tmp+2] = 0;
      }
    }
  }

  return 1;
}
 

int find_value(IplImage* input_img, IplImage* output_img)
{
  for(y=0; y<input_img->height; y++){
    for(x=0; x<input_img->width-1; x++){
      tmp = input_img->widthStep*y+(x*3);
      next_tmp = input_img->widthStep*y+((x+1)*3);

      hue[x][y] = input_img->imageData[tmp+0];
      saturation[x][y] = input_img->imageData[tmp+1];     
      value[x][y] = input_img->imageData[tmp+2];
      hue[x+1][y] = input_img->imageData[next_tmp+0];
      saturation[x+1][y] = input_img->imageData[next_tmp+1];     
      value[x+1][y] = input_img->imageData[next_tmp+2];

      if(hue[x][y] < 0)
	hue[x][y] += 180;
      if(saturation[x][y]< 0)
	saturation[x][y] += 255;
      if( value[x][y] < 0)
	value[x][y] += 255;

      if(value[x][y] > 123){
	output_img->imageData[tmp+0] = input_img->imageData[tmp+0];
	output_img->imageData[tmp+1] = input_img->imageData[tmp+1];
	output_img->imageData[tmp+2] = input_img->imageData[tmp+2];
      }
      else {
	output_img->imageData[tmp+0] = 0;
	output_img->imageData[tmp+1] = 0;
	output_img->imageData[tmp+2] = 0;
      }
    }
  }

  return 1;
}

int find_yellow(IplImage* input_img,IplImage* output_img)
{
 
  for(y=0; y<input_img->height; y++){
    for(x=0; x<input_img->width; x++){
      tmp = input_img->widthStep*y+(x*3);

      hue[x][y] = input_img->imageData[tmp+0];
      saturation[x][y] = input_img->imageData[tmp+1];
      value[x][y] = input_img->imageData[tmp+2];

      if(hue[x][y] < 0)
	hue[x][y] += 180;
      if(saturation[x][y]< 0)
	saturation[x][y] += 255;
      if( value[x][y] < 0)
	value[x][y] += 255;
          
      if(5<hue[x][y] && hue[x][y]<40){
	output_img->imageData[tmp+0] = input_img->imageData[tmp+0];
	output_img->imageData[tmp+1] = input_img->imageData[tmp+1];
	output_img->imageData[tmp+2] = input_img->imageData[tmp+2];
      }
      else {
	output_img->imageData[tmp+0] = 0;
	output_img->imageData[tmp+1] = 0;
	output_img->imageData[tmp+2] = 0; 
      }
    }
  }
  return 1;
}


