#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "module.h"
#include "../../serverfasttext.h"

int ANALOGIES_handler(StrMap *args, const unsigned char *data,unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz){
	if(args==NULL || ret_data==NULL)return -1;

	char w1[1001];
	char w2[1001];
	char w3[1001];
	if(!sm_get(args,"w1",w1))return -1;
	if(!sm_get(args,"w2",w2))return -1;
	if(!sm_get(args,"w3",w3))return -1;

	std::string sw1=std::string(w1);
	std::string sw2=std::string(w2);
	std::string sw3=std::string(w3);

	std::vector<std::pair<float,std::string>> results=fasttext::serverFastText.getAnalogies(10,sw1,sw2,sw3);
	
	std::string word=std::string("");
	if(results.size()>0)word=results[0].second;

	*ret_data = (unsigned char*)malloc(word.size()+1);
	std::copy(word.begin(), word.end(), *ret_data);
	(*ret_data)[word.size()] = '\0';
	*ret_sz=word.size();

	return 0;
}
