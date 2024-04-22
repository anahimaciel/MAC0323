#include <bits/stdc++.h>
using namespace std;

bool isCharacter(char c){
    return (c!='('&& c !=')' && c != '*' && c != '|' && c != '^' && c != '[' && c != ']'&& c!='.' && c!= '+' && c !='-'); 
};

bool isLetter(char c){
    set <char> alphabet={ 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    return(alphabet.count(c)>0);
};

bool isNumber(char c){
    set <char> numbers = { '0','1','2','3','4','5','6','7','8','9'};
    return(numbers.count(c)>0);
};

bool bothNumbersOrLetters(char a, char b){
    return(isNumber(a) && isNumber(b) || isLetter(a) && isLetter(b));
};

class Graph{
    private:
    void dfs(int vertex, bool* visited){
        visited[vertex]=true;
        for (int i = 0; i <v ; i++)
        {
            if(adj[vertex][i]&& !visited[i]) dfs(i,visited);
        }
        
    }

    void addEdge(int u, int w){             //u->w
        adj[u][w]=true;
    }

    public:
    int v;
    vector<char> letters;
    vector<vector<bool>>adj;   

    Graph(int vertices): v(vertices), letters(vertices), adj(vertices,vector<bool>(vertices,false)){}

    void buildGraph(string exp){    
        stack<int> stack;
        int prev=0;                 
        for (int i = 0; i < exp.length(); i++)
        {
            letters[i]=exp[i];
            if(exp[i]=='(' || exp[i] =='|') {
                stack.push(i);
            }
            else{          
                if(isCharacter(exp[i])&& exp[i+1]!= '*' && exp[i+1]!= '+') {
                    continue;
                }
                if(exp[i]==')'){
                    int top=stack.top();
                    stack.pop();
                    if(exp[top]=='|'){
                        prev=stack.top();
                        stack.pop();
                        addEdge(prev,top+1);
                        addEdge(top,i+1);       
                    }
                    else prev=top;
                }
                if(i<exp.length()-1 && exp[i+1]=='*'){
                    addEdge(prev,i+1);
                    addEdge(i+1,prev);
                }
                if(i<exp.length()-1 && exp[i+1]=='+'){  
                    addEdge(i+1,prev);
                }                
            }
            if(i<exp.length()-1 && exp[i]=='(' || exp[i]=='*' || exp[i]==')' || exp[i]=='+') 
                addEdge(i,i+1);
        }
        letters[v-1]=' ';   
    }

    bool testWord(string word){ 
        bool* visited=new bool[v];
        for (int i = 0; i < v; i++)
        visited[i]=false;
        dfs(0,visited);
        for (int i = 0; i < word.length(); i++)
        {
            bool* prox=new bool[v];
            for (int j = 0; j < v; j++)  prox[j]=false;
            for (int j = 0; j < v; j++){
                if(visited[j]){
                        if(letters[j]== '\\' && letters[j+1]==word[i]){                        
                            prox[j+2]=true;
                            j=j+2;
                        }
                        if(letters[j]==word[i] || letters[j]=='.')   
                            prox[j+1]=true;
                        if(letters[j]=='['){
                            if(letters[j+1]=='^'){                      //complement  
                                if(letters[j+3]=='-'){                  //complement of interval
                                    if((tolower(word[i])<=letters[j+2]||tolower(word[i])>=letters[j+4]) && bothNumbersOrLetters(word[i],letters[j+2])){
                                        prox[j+6]=true;
                                    }
                                    j=j+6;                             //skip to the element after ]
                                }
                                set<char> set;
                                int k=j+2;
                                while(letters[k]!=']' ){
                                    if(letters[k]!=']' ){
                                        set.insert(letters[k]);
                                    }
                                    k++;
                                }
                                if(set.count(word[i])==0) {
                                    prox[k+1]=true;
                                }
                                j=k+1;
                            }
                            else if(letters[j+2]=='-'){                 //interval
                                if((tolower(word[i])>=letters[j+1]||tolower(word[i])<=letters[j+3])&& bothNumbersOrLetters(word[i],letters[j+1])){ 
                                        prox[j+5]=true;
                                }
                                j=j+5;
                            }
                            else{                                       //set
                                set<char> set;
                                int k=j+1;
                                while(letters[k]!=']'){
                                    if(letters[k]!=']'){
                                        set.insert(letters[k]);
                                    }
                                    k++;
                                }
                                if(set.count(word[i])>0) {
                                    prox[k+1]=true;
                                }
                                j=k+1;
                            }
                        }
                    }
            }
            bool* marked=new bool[v];
            for (int j = 0; j < v; j++)  visited[j]=false;
            for (int j = 0; j < v; j++) {
                if(prox[j]){
                    for (int k = 0; k < v; k++)  marked[k]=false;
                    dfs(j,marked);
                    for (int k = 0; k < v; k++)  
                        if(marked[k]) visited[k]=true;
                }
            }
        }
        return(visited[v-1]);
    }
};    

int main(){ 
    string filename;
    string exp;                                                     //regular expression
    string n;                                                       //number of test words
    string word;

    cout << "Nome do arquivo: "<<endl;                              //input comes from file
    cin >> filename;
       
    ifstream input(filename);

    if (!input) {                                                   //if file is not in the same folder  
        cerr << "Arquivo nao encontrado: " << filename << endl;
        exit(1);
    }                
    
    getline(input,exp);
    getline(input,n);

    exp.erase(remove(exp.begin(),exp.end(),' '),exp.end());         //removing spaces
    Graph graph=Graph(exp.length()+1);
    graph.buildGraph(exp);

    for (int i = 0; i <stoi(n); i++)
    {
        getline(input,word);
        if(graph.testWord(word)) cout<<"S"<<endl;
        else cout<<"N"<<endl;
    }
}
