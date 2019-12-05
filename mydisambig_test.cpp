#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <cstring>
#include <stdio.h>
#include <fstream>

#include "Ngram.h"

using namespace std;

double getBigramProb(const char *w1, const char *w2, Ngram &lm, Vocab &voc)
{
    VocabIndex wid1 = voc.getIndex(w1);
    VocabIndex wid2 = voc.getIndex(w2);

    if(wid1 == Vocab_None) 
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None) 
        wid2 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid1, Vocab_None };
    return lm.wordProb( wid2, context);
}

int main(int argc, char *argv[]){
    ifstream infile(argv[2]);
    ifstream infile_map("Big5-ZhuYin.map");

    string line;
    map<string, vector<string>> ChuYin_to_word;

    Vocab voc;
    Ngram lm( voc, 2 );

    File frLm("bigram.lm", "r" );
    lm.read(frLm);
    frLm.close();

    while (getline(infile_map, line)) { // Build up the table for ChuYin map to all posible chinese char
        int space_pos = line.find(' ');
        string ChuYin = line.substr(space_pos + 1, 2);
        string chinese_word = line.substr(0, 2);
        ChuYin_to_word[ChuYin].push_back(chinese_word);

        int slash_pos = line.find('/');
        while (slash_pos != -1) {
            ChuYin = line.substr(slash_pos + 1, 2);
            vector<string>::iterator it = find(ChuYin_to_word[ChuYin].begin(), ChuYin_to_word[ChuYin].end(), chinese_word);

            if (it == ChuYin_to_word[ChuYin].end()) {
                ChuYin_to_word[ChuYin].push_back(chinese_word);
            }

            line = line.substr(slash_pos + 1);
            slash_pos = line.find('/');
        }
    }


    while (getline(infile, line)) {
        vector<string> possible_line;
        vector<double> probability;
        map<string, vector<string>>::iterator iter;
        int space_pos = 0;

        while (space_pos != -1) {   // remove all space in the input line
            space_pos = line.find(' ');
            if (space_pos != -1)
                line.erase(space_pos, 1);
        }

        // take care of the first word in the input line
        string first_word = line.substr(0, 2);
        iter = ChuYin_to_word.find(first_word);
        if(iter != ChuYin_to_word.end()) {    // it is ChuYin
            for(int j = 0; j < ChuYin_to_word[first_word].size(); j++) {
                possible_line.push_back(ChuYin_to_word[first_word][j]);
                probability.push_back(getBigramProb("<s>", ChuYin_to_word[first_word][j].c_str(), lm, voc));
            }
        }
        else {            // it is chinese char
            possible_line.push_back(first_word);
            probability.push_back(getBigramProb("<s>", first_word.c_str(), lm, voc));
        }


        vector<string> temp_possible_line;
        vector<double> temp_probability;
        for (int i = 2; i < line.size(); i += 2) {  // for the rest of the word in the input line
            string word = line.substr(i, 2);
            //cout << endl << endl << i / 2 << " " << word << endl;
            
            iter = ChuYin_to_word.find(word);
            if(iter != ChuYin_to_word.end()) {  // it is ChuYin
                for (int j = 0; j < ChuYin_to_word[word].size(); j++) {
                    string possible_word = ChuYin_to_word[word][j];
                    double max_probability = -99999;
                    int choose_index = -1;

                    for (int k = 0; k < possible_line.size(); k++) {
                        int last_word_pos = possible_line[k].length() - 2;
                        string last_word = possible_line[k].substr(last_word_pos, 2);
                        //cout << last_word << " " << possible_line[k] + "_" + possible_word << " " << getBigramProb(last_word.c_str(), possible_word.c_str(), lm, voc) + probability[k] << endl;
                        if (getBigramProb(last_word.c_str(), possible_word.c_str(), lm, voc) + probability[k] > max_probability) {
                            max_probability = getBigramProb(last_word.c_str(), possible_word.c_str(), lm, voc) + probability[k];
                            choose_index = k;
                        }
                    }
                    temp_possible_line.push_back(possible_line[choose_index] + possible_word);
                    temp_probability.push_back(max_probability);
                }

            }
            else {  // is chinese word
                double max_probability = -99999;
                int choose_index = -1;

                for (int j = 0; j < possible_line.size(); j++) {
                    int last_word_pos = possible_line[j].length() - 2;
                    string last_word = possible_line[j].substr(last_word_pos, 2);
                    //cout << last_word << " " << possible_line[j] + "_" + word << " " << getBigramProb(last_word.c_str(), word.c_str(), lm, voc) + probability[j] << endl;
                    if (getBigramProb(last_word.c_str(), word.c_str(), lm, voc) + probability[j] > max_probability) {
                        max_probability = getBigramProb(last_word.c_str(), word.c_str(), lm, voc) + probability[j];
                        choose_index = j;
                    }
                }
                temp_possible_line.push_back(possible_line[choose_index] + word);
                temp_probability.push_back(max_probability);
            }
            possible_line.clear();
            probability.clear();
            possible_line.assign(temp_possible_line.begin(), temp_possible_line.end());
            probability.assign(temp_probability.begin(), temp_probability.end());
            temp_possible_line.clear();
            temp_probability.clear();
        }

        for (int i = 0; i < probability.size(); i++) {
            string last_word = possible_line[i].substr(possible_line[i].length() - 2, 2);
            probability[i] += getBigramProb(last_word.c_str(), "</s>", lm, voc);
        }

        double max = -9999;
        int index = -1;
        for (int i = 0; i < probability.size(); i++) {
            if (probability[i] > max) {
                max = probability[i];
                index = i;
            }
        }

        cout << "<s> ";
        for (int i = 0; i < possible_line[index].size(); i += 2) {
            cout << possible_line[index].substr(i, 2) << " ";
        }
        cout << "</s>" << endl;
    }

    


    return 0;
}

