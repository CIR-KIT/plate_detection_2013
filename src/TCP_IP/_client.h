#include <stdio.h>	/* printf()、fprintf()に必要 */
#include <string.h>        /* strtok(), atoi()に必要 */
#include <sys/socket.h>	/* socket()、connect()、send()、recv()に必要 */
#include <arpa/inet.h>	/* sockaddr_in、inet_addr()に必要 */
#include <stdlib.h>	/* atoi()に必要 */
#include <string.h>	/* memset()に必要 */
#include <unistd.h>	/* close()に必要 */
#include "./client.h"

const char* ServIP[NUM_OF_TCP] = {"192.168.0.10","192.168.0.10","192.168.0.10"};
const unsigned short ServPort[NUM_OF_TCP] = {5000,5100,5200};//,6000};
struct sockaddr_in ServAddr[NUM_OF_TCP];
