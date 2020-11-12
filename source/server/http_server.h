#ifndef _PVF_HTTP_SERVER_H_
#define _PVF_HTTP_SERVER_H_

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "strmap.h"
#include "encoding/encoding.h"

//#define DEBUG_PRINT(level,message,code) {{if(level<=debug_level){ int _debug_rank; MPI_Comm_rank(MPI_COMM_WORLD, &_debug_rank); fprintf(stderr,"%d:%d:%d:%s\n",_debug_rank,level,code,message);}}}
#define DEBUG_PRINT(level,message,code) {fprintf(stderr,"%d:%d:%s\n",level,code,message);}

typedef struct {
	int sock;
	char addr[20];
}HTTP_Client_Data;

typedef struct {
	char *data;
	int type;
	unsigned long len;
	int fd;
}HTTPResult;

extern pthread_spinlock_t slock_server;
#define SERVER_LOCK pthread_spin_lock(&slock_server);
#define SERVER_UNLOCK pthread_spin_unlock(&slock_server);

int http_server_init();

void* HTTPServer_Thread_Client(void* lpThreadData);
HTTPResult* HandleRequest(void *hcdv,const char *fname,const char *query,const char *soap_action,const char *data, int datasz);
void *HTTPServer_Thread_Server(void* lpThreadData);
int SENGINE_Server(int port);
int SENGINE_Server_Run(int port);
void* HTTPServer_Thread_Client(void* lpThreadData);

typedef int (*http_func)(StrMap *args, const unsigned char *data, unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz);

http_func getHttpFunc(const char *name);

#endif
