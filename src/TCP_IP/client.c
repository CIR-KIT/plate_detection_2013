#include "./_client.h"


void tcp_retry(const char* message)
{
  perror(message);
  fprintf(stderr,"Retry\n");
}
/*==========================================================================================
  Name     : tcp_error
  Argument : char* message  //error message
  Return   : -1
  About    : TCP/IP通信においてエラーが発生した時に呼び出される
             エラー時には引き数のメッセージを表示し、引き数-1を返してプログラムを終了する
  Author   : Ryodo Tanaka
  Date     : 2013/09/07
 =========================================================================================*/   
void tcp_error(const char* message)
{
  perror(message);
  exit(-1);
}

/*==========================================================================================
  Name     : cliant_setup
  Argument : void
  Return   : 1 (SUCCEED)
  About    : TCP/IP接続の初期設定とconnect処理を行う
             接続先Serverが複数あるときは、
             ./_cliant.hの設定を変更すること
  Author   : Ryodo Tanaka
  Date     : 2013/09/07
==========================================================================================*/
int client_setup(void)
{
  short i;
 
  for(i=0;i<NUM_OF_TCP;i++){
    if( (sock[i] = socket(PF_INET,SOCK_STREAM, IPPROTO_TCP)) < 0 )
      tcp_error("socket()");

    memset(&ServAddr[i], 0, sizeof(ServAddr[i]));
    ServAddr[i].sin_family = AF_INET;
    ServAddr[i].sin_addr.s_addr = inet_addr(ServIP[i]);
    ServAddr[i].sin_port = htons(ServPort[i]);

    if(connect(sock[i], (struct sockaddr*)&ServAddr[i], sizeof(ServAddr[i])) < 0)
      tcp_error("connect()");
  }

  printf("Connect succeed\n");
  return 1;
}


/*++++++++++++++++++++++++++++++++++++++++++++++*/
/*    SplitString...文字列を指定文字で分割する       */
/*    引数 :                                     */
/*      char *buf[] : 代入先                     */
/*      const char *str : 分割元の文字列          */
/*      const char *delimiter : 分割する文字      */
/*    返り値 : 分割した個数                        */
/*++++++++++++++++++++++++++++++++++++++++++++++*/
int SplitString(char *buf[], const char *str, const char *delimiter)
{
  int i = 0;
  int len = strlen(str);
  char *before, *temp;
  before = (char*)malloc(len + 1);
  strcpy(before, str);
  temp = strtok(before, delimiter);
  if(temp == NULL){
    free(before);
    return 0;
  }else{
    strcpy(buf[0], temp);
    for(i = 1; i < len; i++){
      temp = strtok(NULL, delimiter);
      if(temp != NULL){
	strcpy(buf[i], temp);
      }else{
	break;
      }
    }
  }
  free(before);
  return i;
}

int char2lrf_cmd(lrf_cmd* lrf_cmd_data, char* trans_message[], const char* message)
{
  int i;
  int data_num = 0; 

  data_num = SplitString(trans_message, message, ",");
 
  if((strcmp(trans_message[0],"$LRF") == 0) && (data_num >= 1000)){
    for(i=0; i<1083; i++){
      if(i == 0)
	;
      if(i==1){
	lrf_cmd_data->lrf_data_max = atol(trans_message[i]);
      }
      else {
	lrf_cmd_data->lrf_data[i-2] = atol(trans_message[i]);
      }
    }
  }
  else {
    fprintf(stderr, "lrf_cmd is not correct.\n");
    return -1;
  }
  return 1;
}

/* int client_communication(const int* sock, void *cmd, const int send_flg, const int recv_flg) */
/* { */

/*   unsigned int thf_Send_Bytes = 0; */
/*   unsigned int thf_Recv_Bytes = 0; */
/*   int Rcvd_Bytes = 0; */

/*   if(send_flg){ */
/*     thf_Send_Bytes = sizeof(*cmd); */
/*     if(send(*sock, cmd, sizeof(*cmd), 0) != thf_Send_Bytes) */
/*       tcp_error("send()"); */
/*     printf("send thf_cmd\n"); */
/*   } */

/*   if(recv_flg){ */
/*     thf_Recv_Bytes = sizeof(*cmd); */
/*     do{ */
/*       if( (Rcvd_Bytes = recv(*sock, cmd, thf_Recv_Bytes , 0)) < 0 ) */
/*   	tcp_error("recv()"); */
/*       //  Total_Rcvd_Bytes += Rcvd_Bytes; */
/*     } while(Rcvd_Bytes != 0); */
/*     printf("recv thf_cmd\n"); */
/*   } */

/*   return 1; */
/* } */
