#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "module.h"
#include <iostream>
#include <sstream>
#include <queue>

#include "../../fasttext.h"
#include "../../../src/vector.h"

int SIMILARITYCOMPUTE_get_handler(StrMap *args, const unsigned char *data,unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz){
	if(args==NULL || ret_data==NULL)return -1;

	char w1[1001];
	char w2[1001];
    if(!sm_get(args,"w1",w1))return -1;
	std::string queryWord1=std::string(w1);

    if(!sm_get(args,"w2",w2))return -1;
	std::string queryWord2=std::string(w2);


  	fasttext::Vector queryVec1(fasttext::serverFastText.getDimension());
  	fasttext::Vector queryVec2(fasttext::serverFastText.getDimension());
	std::stringstream ret;
	fasttext::serverFastText.getWordVector(queryVec1, queryWord1);
	fasttext::serverFastText.getWordVector(queryVec2, queryWord2);

  fasttext::real queryNorm1 = queryVec1.norm();
  if (std::abs(queryNorm1) < 1e-8) {
    queryNorm1 = 1;
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
