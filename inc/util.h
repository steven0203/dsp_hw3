#ifndef UTIL_H
#define UTIL_H

#include<map>
#include<string>
#include<iostream>
#include<fstream>
#include <cstring>
#include <vector>

#include"Ngram.h"

double getBigramProb(const char *w1, const char *w2, Ngram &lm, Vocab &voc);
void getMap(ifstream & infile, map<string, vector<string>> & mapping);
string Viterbi(string &line,Vocab &voc,Ngram &lm,map<string, vector<string>> &mapping);
void outputResult(ofstream &out,string line);

#endif