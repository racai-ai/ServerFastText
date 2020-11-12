#include <stdio.h>

int getASCII(const unsigned char *data, unsigned long data_sz, unsigned long *c, int *sz){
	if(data_sz==0 || data==NULL || c==NULL || sz==NULL)return -1;

	unsigned char c1=data[0];
	*sz=1;
	*c=c1;

	return 0;
}

int setASCII(unsigned long c,unsigned char *data,unsigned long data_sz,int *sz){
	if(data_sz==0 || data==NULL || sz==NULL)return -1;

	unsigned char c1=(unsigned char)(c&0xFF);
	data[0]=c1;
	*sz=1;

	return 0;
}
