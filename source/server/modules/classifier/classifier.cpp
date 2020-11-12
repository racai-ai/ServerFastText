#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "module.h"
#include "../../serverfasttext.h"

int CLASSIFIER_predict_handler(StrMap *args, const unsigned char *data,unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz){
	if(args==NULL || ret_data==NULL)return -1;

	char w1[1001];
	long num=1;
	if(sm_get(args,"num",w1))num=atol(w1);
	
	float threshold=0.0;
	if(sm_get(args,"threshold",w1))threshold=atof(w1);

	std::string dataString=std::string("");
	dataString.assign((const char*)data,data_sz);
	
	/*std::cerr << num << std::endl;
	std::cerr << threshold << std::endl;
	std::cerr << dataString << std::endl;*/
	
	std::istringstream in(dataString);

	std::vector<std::pair<float,std::string>> results;
	fasttext::serverFastText.predictLine(in,results,num,threshold);
	
	std::string resultString=std::string("");
	for(std::pair<float,std::string> p:results){
	    if(resultString.size()>0)resultString.append(" ");
	    resultString.append(p.second);
	}
	*ret_data = (unsigned char*)malloc(resultString.size()+1);
	std::copy(resultString.begin(), resultString.end(), *ret_data);
	(*ret_data)[resultString.size()] = '\0';
	*ret_sz=resultString.size();

	return 0;
}
