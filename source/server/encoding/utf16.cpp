#include <stdio.h>

int getUTF16(const unsigned char *data, unsigned long data_sz, unsigned long *c, int *sz){
	if(data_sz<2 || data==NULL || c==NULL || sz==NULL)return -1;

	*c=0;
	*sz=0;
	unsigned char c1=data[0];
	unsigned char c2=data[1];

	unsigned long w1=(((unsigned long)c1)<<8)|(unsigned long)c2;
	if(w1<0xD800 || w1>0xDFFF){
		*c=w1;
		*sz=2;
	}else{
		if(w1>0xD800 && w1<0xDBFF){
			if(data_sz<4)return -1;

			unsigned char c3=data[2];
			unsigned char c4=data[3];

			unsigned long w2=(((unsigned long)c3)<<8)|(unsigned long)c4;
			if(w2<=0xDC00 || w2>=0xDFFF)return -1;

			(*c)=((w1&0x3FF)<<10) | (w2&0x3FF);
			(*c) += 0x10000;
		}else return -1;
	}

	return 0;
}

int setUTF16(unsigned long c,unsigned char *data,unsigned long data_sz,int *sz){
	if(data_sz<2 || data==NULL || sz==NULL)return -1;

	if(c<=0xFFFF){
		*sz=2;
		data[0]=(unsigned char)(c&0xFF);
		data[1]=(unsigned char)((c>>8)&0xFF);
	}else{
		c-=0x10000;

		unsigned long vh=c>>10;
		unsigned long vl=c&0x3FF;
		unsigned long w1=0xD800+vh;
		unsigned long w2=0xDC00+vl;

		*sz=4;
		data[0]=(unsigned char)(w1&0xFF);
		data[1]=(unsigned char)((w1>>8)&0xFF);
		data[2]=(unsigned char)(w2&0xFF);
		data[3]=(unsigned char)((w2>>8)&0xFF);

	}

	return 0;
}
