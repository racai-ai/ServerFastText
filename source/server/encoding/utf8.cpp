#include <stdio.h>

int getUTF8(const unsigned char *data, unsigned long data_sz, unsigned long *c, int *sz){
	if(data_sz==0 || data==NULL || c==NULL || sz==NULL)return -1;

	*c=0;
	*sz=0;
	unsigned char c1=data[0];
	if((c1 & 128)==0){ // single byte UNICODE
		*sz=1;
		*c=c1;

	}else if((c1 & 224)==192){ // two byte UNICODE
		if(data_sz<2)return -1; // invalid data size
		*sz=2;
		unsigned char c2=((unsigned char*)data)[1];
		if((c2&192)!=128)return -1; // invalid encoding
		*c= (((unsigned long)c1&31)<<6) | ((unsigned long)c2&63);

	}else if((c1 & 240)==224){ // three byte UNICODE
		if(data_sz<3)return -1;
		*sz=3;
		unsigned char c2=((unsigned char*)data)[1];
		if((c2&192)!=128)return -1; // invalid encoding
		unsigned char c3=((unsigned char*)data)[2];
		if((c3&192)!=128)return -1; // invalid encoding
		*c= (((unsigned long)c1&15)<<12) | (((unsigned long)c2&63)<<6) | ((unsigned long)c3&63);

	}else if((c1 & 248)==240){ // four byte UNICODE
		if(data_sz<4)return -1;
		*sz=4;
		unsigned char c2=((unsigned char*)data)[1];
		if((c2&192)!=128)return -1; // invalid encoding
		unsigned char c3=((unsigned char*)data)[2];
		if((c3&192)!=128)return -1; // invalid encoding
		unsigned char c4=((unsigned char*)data)[3];
		if((c4&192)!=128)return -1; // invalid encoding
		*c= (((unsigned long)c1&7)<<18) | (((unsigned long)c2&63)<<12) | (((unsigned long)c3&63)<<6) | ((unsigned long)c3&63);

	}else if((c1 & 252)==248){ // five byte UNICODE
		if(data_sz<5)return -1;
		*sz=5;
		unsigned char c2=((unsigned char*)data)[1];
		if((c2&192)!=128)return -1; // invalid encoding
		unsigned char c3=((unsigned char*)data)[2];
		if((c3&192)!=128)return -1; // invalid encoding
		unsigned char c4=((unsigned char*)data)[3];
		if((c4&192)!=128)return -1; // invalid encoding
		unsigned char c5=((unsigned char*)data)[4];
		if((c5&192)!=128)return -1; // invalid encoding
		*c= (((unsigned long)c1&3)<<24) | (((unsigned long)c2&63)<<18) | (((unsigned long)c3&63)<<12) | (((unsigned long)c3&63)<<6) | ((unsigned long)c4&63);

	}else if((c1 & 254)==252){ // six byte UNICODE
		if(data_sz<6)return -1;
		*sz=6;
		unsigned char c2=((unsigned char*)data)[1];
		if((c2&192)!=128)return -1; // invalid encoding
		unsigned char c3=((unsigned char*)data)[2];
		if((c3&192)!=128)return -1; // invalid encoding
		unsigned char c4=((unsigned char*)data)[3];
		if((c4&192)!=128)return -1; // invalid encoding
		unsigned char c5=((unsigned char*)data)[4];
		if((c5&192)!=128)return -1; // invalid encoding
		unsigned char c6=((unsigned char*)data)[5];
		if((c6&192)!=128)return -1; // invalid encoding
		*c= (((unsigned long)c1&1)<<30) | (((unsigned long)c2&63)<<24) | (((unsigned long)c3&63)<<18) | (((unsigned long)c3&63)<<12) | (((unsigned long)c4&63)<<6) | ((unsigned long)c5&63);

	}else return -1; // invalid encoding

	return 0;
}

int setUTF8(unsigned long c,unsigned char *data,unsigned long data_sz,int *sz){
	if(data_sz==0 || data==NULL || sz==NULL)return -1;

	if(c<128){
		*sz=1;
		data[0]=(unsigned char)(c&0xFF);
	}else if(c<2048){
		if(data_sz<2)return -1; // Insufficient space
		*sz=2;
		data[0]=192 | (unsigned char)((c>>6)&0xFF);
		data[1]=128 | (unsigned char)(c& 63);
	}else if(c<65536){
		if(data_sz<3)return -1; // Insufficient space
		*sz=3;
		data[0]=224 | (unsigned char)((c>>12)&0xFF);
		data[1]=128 | (unsigned char)((c>>6)& 63);
		data[2]=128 | (unsigned char)((c)& 63);
	}else if(c<2097152){
		if(data_sz<4)return -1; // Insufficient space
		*sz=4;
		data[0]=240 | (unsigned char)((c>>18)&0xFF);
		data[1]=128 | (unsigned char)((c>>12)&63);
		data[2]=128 | (unsigned char)((c>>6)&63);
		data[3]=128 | (unsigned char)((c)&63);
	}else if(c<67108864){
		if(data_sz<5)return -1; // Insufficient space
		*sz=5;
		data[0]=248 | (unsigned char)((c>>24)&0xFF);
		data[1]=128 | (unsigned char)((c>>18)&63);
		data[2]=128 | (unsigned char)((c>>12)&63);
		data[3]=128 | (unsigned char)((c>>6)&63);
		data[4]=128 | (unsigned char)((c)&63);
	}else{
		if(data_sz<6)return -1; // Insufficient space
		*sz=6;
		data[0]=252 | (unsigned char)((c>>30)&0xFF);
		data[1]=128 | (unsigned char)((c>>24)&63);
		data[2]=128 | (unsigned char)((c>>18)&63);
		data[3]=128 | (unsigned char)((c>>12)&63);
		data[4]=128 | (unsigned char)((c>>6)&63);
		data[5]=128 | (unsigned char)((c)&63);
	}

	return 0;
}
