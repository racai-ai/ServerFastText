#include "http_server.h"
#include "urldecode.h"

pthread_spinlock_t slock_server;

int http_server_init(){
	pthread_spin_init(&slock_server, 0);

	return 0;
}


HTTPResult* HandleRequest(void *hcdv,const char *fname,const char *query,const char *soap_action,const char *data, unsigned long datasz){
	HTTP_Client_Data *hcd=(HTTP_Client_Data *)hcdv;

	StrMap *args=sm_new(20,1000);
	char name[1001];
	char *value=NULL;
	char value2[1001];
	char *ret_data=NULL;
	unsigned long ret_sz=0;
	if(query!=NULL){
		for(int start=0,end=0,pos=0;;end++){
			if(query[end]=='&' || query[end]==0){
				if(end>start+1){ // Found argument
					for(pos=start;pos<end && query[pos]!='=';pos++);
					if(pos<end && pos-start<1001 && end-pos<1000){ // Found =
						memcpy(name,query+start,pos-start);
						name[pos-start]=0;

						unsigned long sz=end-pos;
						value=(char*)malloc(sz*sizeof(char));
						memcpy(value,query+pos+1,end-(pos+1));
						value[end-(pos+1)]=0;

						char *dec_val=url_decode(value);
						free(value); value=NULL;
						sm_put(args,name,dec_val);
						free(dec_val);
					}
				}
				if(query[end]==0)break;
				start=end+1;
			}
		}
	}

	bool unknown=false;
	if(fname!=NULL){ // do query for fname
		http_func f=getHttpFunc(fname);
		if(f==NULL)unknown=true;
		else{
			Encoding *enc=NULL;
			if(sm_get(args,"encoding",value2)){
				if(strcasecmp(value2,"ASCII")==0)enc=getEncoding(ASCII);
				else if(strcasecmp(value2,"UTF8")==0 || strcasecmp(value2,"UTF-8")==0)enc=getEncoding(UTF8);
				else if(strcasecmp(value2,"UTF16")==0 || strcasecmp(value2,"UTF-16")==0)enc=getEncoding(UTF16);
			}
			if(enc==NULL)enc=detectEncoding((unsigned char*)data,datasz);

			if(f(args,(unsigned char*)data,datasz,enc,(unsigned char**)&ret_data,&ret_sz))unknown=true;
		}
	}else unknown=true;

	sm_delete(args);

	HTTPResult *ret=(HTTPResult *)malloc(sizeof(HTTPResult));
	memset(ret,0,sizeof(HTTPResult));

	if(!unknown){
		DEBUG_PRINT(5,"Finished processing",0);
		if(ret_data==NULL){
			ret->data=(char*)malloc(10);
			strcpy(ret->data,"OK");
			ret->len=strlen(ret->data);
		}else{
			ret->data=ret_data;
			ret->len=ret_sz;
		}
	}else{
		ret->data=(char*)malloc(10);
		strcpy(ret->data,"ERROR");
		ret->len=strlen(ret->data);
	}

	return ret;
}

void *HTTPServer_Thread_Server(void* lpThreadData){
	int port=*(int*)lpThreadData;
	int sock;
    struct sockaddr_in addr;

    if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
    	DEBUG_PRINT(0,"Can not get a socket",sock);
    	return NULL;
    }

    int optval=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int));

    addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	//addr.sin_addr.s_addr = INADDR_ANY;
	inet_aton("127.0.0.1", &addr.sin_addr);

	if ( bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0 ){
    	DEBUG_PRINT(0,"Can not bind",0);
    	return NULL;
	}

	if ( listen(sock, 200) != 0 ){
    	DEBUG_PRINT(0,"Can not listen",0);
    	return NULL;
	}

    while (1){
    	int client;
    	socklen_t addr_size = sizeof(addr);
	    pthread_t child;

        client = accept(sock, (struct sockaddr*)&addr, &addr_size);
        HTTP_Client_Data *hcd=(HTTP_Client_Data*)malloc(sizeof(HTTP_Client_Data));
        hcd->sock=client;
        strcpy(hcd->addr,inet_ntoa(addr.sin_addr));
        if ( pthread_create(&child, NULL, HTTPServer_Thread_Client, hcd) != 0 ){
        	DEBUG_PRINT(0,"Can not create client thread",0);
		}else
            pthread_detach(child);  /* disassociate from parent */
    }

	return NULL;
}

int SENGINE_Server(int port){
	pthread_t server;
	int p=port;
	if(pthread_create(&server, NULL, HTTPServer_Thread_Server, &p) != 0 ){
		DEBUG_PRINT(0,"Can not create server thread",0);
		return -1;
	}

    return 0;
}

int SENGINE_Server_Run(int port){
	int p=port;
	HTTPServer_Thread_Server(&p);

    return 0;
}

void* HTTPServer_Thread_Client(void* lpThreadData){
	HTTP_Client_Data *hcd=(HTTP_Client_Data*)lpThreadData;
	time_t start=time(NULL);
	char buff[2048];
	int pos=0;
	int found_pos=0;
	while(true){
		ssize_t dim=recv(hcd->sock,buff+pos,2048-pos,MSG_DONTWAIT);
		if(dim==-1 && (errno==EAGAIN || errno==EWOULDBLOCK)){
			time_t current=time(NULL);
			if(current-start>5){
				DEBUG_PRINT(1,"RECV Timeout",0);
				close(hcd->sock);
				return NULL;
			}
			usleep(100000);
		}else if(dim>0){
			pos+=dim;
			buff[pos]=0;
			bool found=false;
			for(int i=0;i<pos-1 && !found;i++)
				if(((buff[i]==10) && (buff[i+1]==10)) ||
					((buff[i]==13) && (buff[i+1]==13)) ||
					(i<pos-3 && buff[i]==13 && buff[i+1]==10 && buff[i+2]==13 && buff[i+3]==10)){
						found_pos=i+2;
						if(i<pos-3 && buff[i]==13 && buff[i+1]==10 && buff[i+2]==13 && buff[i+3]==10)found_pos+=2;
						found=true;
				}
			if(found)break;
		}else{
			DEBUG_PRINT(1,"RECV Error",0);
			close(hcd->sock);
			return NULL;
		}
	}

	buff[pos]=0;

	char *rsa=strstr(buff,"SOAPAction:");
	char *soap_action=NULL;
	if(rsa!=NULL){
		char *startsa=rsa+11;
		while(startsa[0]==32 || startsa[0]=='"')startsa++;
		int sz=0;
		while(startsa[sz]!=13 && startsa[sz]!=10 && startsa[sz]!=0 && startsa[sz]!='"')sz++;
		if(sz>0){
			soap_action=(char*)malloc(sz+1);
			memcpy(soap_action,startsa,sz);
			soap_action[sz]=0;
		}
	}

	char *r1=strstr(buff," HTTP/");
	if(r1==NULL){
		DEBUG_PRINT(1,"Unknown request type (1)",0);
		close(hcd->sock);
		return NULL;
	}

	int reqsz=0;
	unsigned long datasz=0;
	bool isPost=false;
	char *data=NULL;


	if(strncasecmp(buff,"POST ",5)==0){
		reqsz=5;
		char *cl=strcasestr(buff,"Content-length");
		if(cl!=NULL){
			while(cl[0]!=0 && (cl[0]<'0' || cl[0]>'9'))cl++;
			datasz=(unsigned long)atol(cl);
		}
		if(datasz>0){
			data=(char*)malloc(datasz+1);
			memset(data,0,datasz+1);
			if(pos-found_pos>0){
				if(pos-found_pos<=datasz)memcpy(data,buff+found_pos,pos-found_pos);
				else memcpy(data,buff+found_pos,datasz);
			}
		}

		isPost=true;
	}else if(strncasecmp(buff,"GET ",4)==0){
		reqsz=4;
	}else{
		DEBUG_PRINT(1,"Unknown request type (2)",0);
		close(hcd->sock);
		return NULL;
	}

	memcpy(buff,buff+reqsz,r1-buff-reqsz);
	buff[r1-buff-reqsz]=0;

	char *file=buff;
	char *query=strstr(buff,"?");
	if(query!=NULL){
		query[0]=0;
		query++;
	}


	DEBUG_PRINT(5,file,0);
	DEBUG_PRINT(5,query,0);
	//DEBUG_PRINT(5,soap_action,0);

	if(datasz>0 && isPost){
		pos-=found_pos;
		start=time(NULL);
		ssize_t dim=0;
		time_t current=0;
		while(pos<datasz){
			dim=recv(hcd->sock,data+pos,datasz-pos,MSG_DONTWAIT);
			if(dim==-1 && (errno==EAGAIN || errno==EWOULDBLOCK)){
				current=time(NULL);
				if(current-start>5){
					DEBUG_PRINT(1,"RECV Timeout",0);
					close(hcd->sock);
					return NULL;
				}
				usleep(100000);
			}else if(dim>0){
				pos+=dim;
				data[pos]=0;
			}else{
				DEBUG_PRINT(1,"RECV Error",0);
				close(hcd->sock);
				return NULL;
			}
		}

		data[pos]=0;

	}

	HTTPResult *ret=HandleRequest(hcd,file,query,soap_action,data,datasz);
	if(soap_action!=NULL)free(soap_action);
	if(ret!=NULL){
		sprintf(buff,"HTTP/1.1 200 Ok\nPragma: no-cache\nContent-Type: text/xml\nContent-Length: %ld\nConnection: close\r\n\r\n",ret->len);
		send(hcd->sock,buff,strlen(buff),0);
		send(hcd->sock,ret->data,ret->len,0);
		free(ret->data);
		if(ret->fd!=0)close(ret->fd);

		free(ret);
	}

	close(hcd->sock);
	free(hcd);
	return NULL;
}


