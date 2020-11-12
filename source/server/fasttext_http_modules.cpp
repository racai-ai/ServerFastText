#include "http_server.h"
#include "modules/all.h"


http_func getHttpFunc(const char *name){
	if(strcmp(name,"/analogies")==0)return ANALOGIES_handler;
	if(strcmp(name,"/wordvectors_get")==0)return WORDVECTORS_get_handler;
	if(strcmp(name,"/similarity_get")==0)return SIMILARITY_get_handler;
	if(strcmp(name,"/negsimilarity_get")==0)return NEGSIMILARITY_get_handler;
	if(strcmp(name,"/classifier_predict")==0)return CLASSIFIER_predict_handler;

	return NULL;
}
