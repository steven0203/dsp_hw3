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
#include "util.h"

using namespace std;


int main(int argc, char *argv[]){
    ifstream infile(argv[1]);
    ifstream infile_map(argv[2]);

    string line;
    map<string, vector<string>> ChuYin_to_word;

    Vocab voc;
    Ngram lm( voc, 2 );

    File lmFile(argv[3], "r" );
    lm.read(lmFile);
    lmFile.close();

    ofstream outfile(argv[4]);
    getMap(infile_map,ChuYin_to_word);

    while (getline(infile, line)) {
        int space_pos =0;
        while (space_pos != -1) {   // remove all space in the input line
            space_pos = line.find(' ');
            if (space_pos != -1)
                line.erase(space_pos, 1);
        }

        string result=Viterbi(line,voc,lm,ChuYin_to_word);
        outputResult(outfile,result);
    }

    outfile.close();


    return 0;
}

