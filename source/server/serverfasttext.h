#ifndef SERVER_FASTTEXT_H_
#define SERVER_FASTTEXT_H_

#include "../src/fasttext.h"

namespace fasttext {

class ServerFastText:public FastText {
public:
   DenseMatrix* getWordVectors();
   void callLazyComputeWordVectors();
};

extern ServerFastText serverFastText;

}

#endif
