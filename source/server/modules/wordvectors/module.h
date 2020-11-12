#ifndef _PVF_MOD_WORDVECTORS_
#define _PVF_MOD_WORDVECTORS_

#include "../../strmap.h"
#include "../../encoding/encoding.h"

int WORDVECTORS_get_handler(StrMap *args, const unsigned char *data, unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz);


#endif
