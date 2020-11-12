#include "encoding.h"
#include <memory.h>
#include <stdlib.h>

Encoding *getEncoding(EncodingType etype){
	Encoding *ret=(Encoding*)malloc(sizeof(Encoding));
	memset(ret,0,sizeof(Encoding));

	ret->type=etype;

	switch(etype){
	case UTF8:
		ret->get=&getUTF8;
		ret->set=&setUTF8;
		break;
	case ASCII:
		ret->get=&getASCII;
		ret->set=&setASCII;
		break;
	case UTF16:
		ret->get=&getUTF16;
		ret->set=&setUTF16;
		break;
	}

	return ret;
}

Encoding *detectEncoding(const unsigned char *data,unsigned long datasz){
	unsigned long c;
	int sz;

	bool isUTF8=true;
	for(unsigned long i=0;i<datasz;){
		if(getUTF8(data,datasz,&c,&sz)){
			isUTF8=false;
			break;
		}
		i+=sz;
	}

	if(isUTF8)return getEncoding(UTF8);

	bool isUTF16=true;
	for(unsigned long i=0;i<datasz;){
		if(getUTF16(data+i,datasz-i,&c,&sz)){
			isUTF16=false;
			break;
		}
		i+=sz;
	}

	if(isUTF16)return getEncoding(UTF16);

	return getEncoding(ASCII);
}
