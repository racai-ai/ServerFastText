#ifndef _PVF_ENCODING_H_
#define _PVF_ENCODING_H_

typedef int (*encoding_get_fn)(const unsigned char *data,unsigned long data_sz,unsigned long *c,int *sz);
typedef int (*encoding_set_fn)(unsigned long c,unsigned char *data,unsigned long data_sz,int *sz);

typedef enum {
	UNKNOWN,
	ASCII,
	UTF8,
	UTF16
}EncodingType;

typedef struct {
	EncodingType type;
	encoding_get_fn get;
	encoding_set_fn set;
}Encoding;

Encoding *getEncoding(EncodingType etype);
Encoding *detectEncoding(const unsigned char *data,unsigned long datasz);

int getUTF8(const unsigned char *data, unsigned long data_sz, unsigned long *c, int *sz);
int setUTF8(unsigned long c,unsigned char *data,unsigned long data_sz,int *sz);

int getASCII(const unsigned char *data, unsigned long data_sz, unsigned long *c, int *sz);
int setASCII(unsigned long c,unsigned char *data,unsigned long data_sz,int *sz);

int getUTF16(const unsigned char *data, unsigned long data_sz, unsigned long *c, int *sz);
int setUTF16(unsigned long c,unsigned char *data,unsigned long data_sz,int *sz);


#endif
