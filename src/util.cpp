#include"util.h"

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

void getMap(ifstream & infile, map<string, vector<string>> & mapping)
{
    string line;
    while (getline(infile, line)) { 
        int space_pos =0;
        while (space_pos != -1) {   // remove all space in the input line
            space_pos = line.find(' ');
            if (space_pos != -1)
                line.erase(space_pos, 1);
        }

        string ChuYin = line.substr(0, 2);
        for(int i=2;i<line.size();i+=2)
        {
            word=line.substr(i,2);
            mapping[ChuYin].push_back(word);
        }
    }
}

string Viterbi(string &line,Vocab &voc,Ngram &lm,map<string, vector<string>> &mapping)
{
    vector<string> possible_line;
    vector<double> probability;
    string first_word=line.substr(0,2);
    for(int i=0;i<mapping[first_word].size();++i)
    {
        possible_line.push_back(mapping[first_word][j]);
        probability.push_back(getBigramProb("<s>",mapping[first_word][j].c_str(),lm,voc));
    }

    vector<string> temp_line;
    vector<double> temp_prob;
    for (int i = 2; i < line.size(); i += 2) 
    {
        string word=line.substr(i,2);
        double max_prob=-999;
        int index;
        for(int j=0;j<mapping[word].size();++j)
        {
            for(int k=0;k<possible_line.size();++k)
            {
                string last_word=possible_line[k].substr(possible_line[k].length()-2,2);
                if(probability[k]+getBigramProb(last_word.c_str(),mapping[word][j].c_str(),lm,voc)>max_prob)
                {
                    max_prob=probability[k]+getBigramProb(last_word.c_str(),mapping[word][j].c_str(),lm,voc);
                    index=k;
                }
            }
            temp_line.push_back(possible_line[index]+mapping[word][j]);
            temp_prob.push_back(max_prob);
        }
        possible_line.clear();
        probability.clear();
        possible_line.assign(temp_line.begin(),temp_line.end());
        probability.assign(temp_prob.begin(),temp_prob.end());
        temp_line.clear();
        temp_prob.clear();
    }

    for(int i=0;i<possible_line.size();++i)
        probability[i]+=getBigramProb(possible_line[i].substr(possible_line[i].length()-2,2).c_str(),"</s>",lm,voc);

    string result;
    double max_prob=-999;
    for(int i=0;i<possible_line.size();++i)
    {
        if(probability[i]>max_prob)
        {
            result=possible_line[i];
            max_prob=probability[i];
        }
    }
    return string;
}

void outputResult(ofstream &out,string line)
{   
    out<<"<s>";
    for(int i=0;i<line.length();i+=2)
    {
        out<<line.substr(i,2)<<" ";
    }
    out<<"</s>"<<endl;
}

