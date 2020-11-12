#ifndef _PVF_MOD_CLASSIFY_
#define _PVF_MOD_CLASSIFY_

#include "../../strmap.h"
#include "../../encoding/encoding.h"

int CLASSIFIER_predict_handler(StrMap *args, const unsigned char *data, unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz);


#endif
