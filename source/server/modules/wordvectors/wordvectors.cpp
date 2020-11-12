#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "module.h"
#include <iostream>
#include <sstream>

#include "../../serverfasttext.h"
#include "../../../src/vector.h"

int WORDVECTORS_get_handler(StrMap *args, const unsigned char *data,unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz){
	if(args==NULL || ret_data==NULL)return -1;

	char w[1001];
	char name[10];
	int n=0;
	std::string words[10];
	for(n=0;n<10;n++){
	    sprintf(name,"w%d",(n+1));
	    if(!sm_get(args,name,w))break;
	    words[n]=std::string(w);
	}

	if(n==0)return -1;

  	fasttext::Vector vec(fasttext::serverFastText.getDimension());
	std::stringstream ret;
	for(int i=0;i<n;i++){
    		fasttext::serverFastText.getWordVector(vec, words[i]);
		ret << words[i] << " " << vec << std::endl;
	}


	std::string word=ret.str();

	*ret_data = (unsigned char*)malloc(word.size()+1);
	std::copy(word.begin(), word.end(), *ret_data);
	(*ret_data)[word.size()] = '\0';
	*ret_sz=word.size();

	return 0;
}
