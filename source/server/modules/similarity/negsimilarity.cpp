#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "module.h"
#include <iostream>
#include <sstream>
#include <queue>

#include "../../serverfasttext.h"
#include "../../../src/vector.h"

int NEGSIMILARITY_get_handler(StrMap *args, const unsigned char *data,unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz){
	if(args==NULL || ret_data==NULL)return -1;

	char w[1001];
    if(!sm_get(args,"word",w))return -1;
	std::string queryWord=std::string(w);

    if(!sm_get(args,"n",w))return -1;
	int n=atoi(w);
	if(n<1 || n>2000)return -1;

  	fasttext::Vector queryVec(fasttext::serverFastText.getDimension());
	std::stringstream ret;
	fasttext::serverFastText.getWordVector(queryVec, queryWord);

	for(int i=0;i<queryVec.size();i++)
	    queryVec.data()[i]*=-1.0;

  	std::set<std::string> banSet;
    banSet.clear();
    banSet.insert(queryWord);

    //findNN(wordVectors, queryVec, k, banSet);

  fasttext::real queryNorm = queryVec.norm();
  if (std::abs(queryNorm) < 1e-8) {
    queryNorm = 1;
  }
  std::priority_queue<std::pair<fasttext::real, std::string>> heap;
  fasttext::Vector vec(fasttext::serverFastText.getDimension());
  for (int32_t i = 0; i < fasttext::serverFastText.getDictionary()->nwords(); i++) {
    std::string word = fasttext::serverFastText.getDictionary()->getWord(i);
    fasttext::real dp = fasttext::serverFastText.getWordVectors()->dotRow(queryVec, i);
    heap.push(std::make_pair(dp / queryNorm, word));
  }
  int32_t i = 0;
  while (i < n && heap.size() > 0) {
    auto it = banSet.find(heap.top().second);
    if (it == banSet.end()) {
      ret << heap.top().second << " " << heap.top().first << std::endl;
      i++;
    }
    heap.pop();
  }



	//ret << words[i] << " " << vec << std::endl;


	std::string retWord=ret.str();

	*ret_data = (unsigned char*)malloc(retWord.size()+1);
	std::copy(retWord.begin(), retWord.end(), *ret_data);
	(*ret_data)[retWord.size()] = '\0';
	*ret_sz=retWord.size();

	return 0;
}
