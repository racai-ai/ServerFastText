#include "serverfasttext.h"

namespace fasttext {

ServerFastText serverFastText;

DenseMatrix * ServerFastText::getWordVectors(){
   return wordVectors_.get();
}

void ServerFastText::callLazyComputeWordVectors(){
    lazyComputeWordVectors();
}


}