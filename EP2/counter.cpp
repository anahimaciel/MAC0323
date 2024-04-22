#include <bits/stdc++.h>
using namespace std;

int main(){
    string filename = "dickens.txt";                       //text comes from file
    ifstream input(filename);
    int cnt=0;
    string word;

    while(input>>word){
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end()); //remove punctuation
            transform(word.begin(), word.end(), word.begin(), ::tolower);           //to lower case

            if(!word.empty()){
                cnt++;
            }
    }
    cout<<cnt;                                        //subtract 9 for the rest of the input (ARN, numWords,numQuery.queries)
}