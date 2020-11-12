#ifndef _PVF_MOD_SIMILARITY_
#define _PVF_MOD_SIMILARITY_

#include "../../strmap.h"
#include "../../encoding/encoding.h"

int SIMILARITY_get_handler(StrMap *args, const unsigned char *data, unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz);
int SIMILARITYCOMPUTE_get_handler(StrMap *args, const unsigned char *data, unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz);
int NEGSIMILARITY_get_handler(StrMap *args, const unsigned char *data, unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz);


#endif
