# ServerFastText

In this repository is an addition to Facebook's FastText allowing pre-loading of a trained model and interacting with it via REST APIs.

# Compile

Run "make" or "make fasttext-static". Will create a "build" directory for the intermediate files.

# Run

Keeps all the FastText command line options and adds an additional parameter "server".

> ./fasttext server <model_file.bin> <port>

# APIs

## http://<localhost:port>/wordvectors_get?w1=...[&w2=.........&w10=...]

Returns the word embeddings associated with at most 10 words.

## http://<localhost:port>/similarity_get?word=...&n=...

Returns the n most similar words to a give word using word embeddings.

## http://<localhost:port>/analogies?w1=...&w2=...&w3=...

Given 3 words will compute the analogy: w1-w2+w3

## http://<localhost:port>/classifier_predict?num=...&threshold=...

Given a text will predict num labels considering a minimal threshold. The text will be sent via POST data.

# Use a newer version of FastText

The "src" folder currently holds the unchanged "src" folder from the FastText github repository ( https://github.com/facebookresearch/fastText ). As long as the C++ interface remains unchanged it should be possible to replace the "src" folder from our repo with the latest "src" folder from the official FastText repo. 
