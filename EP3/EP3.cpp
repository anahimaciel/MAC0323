#include <bits/stdc++.h>
using namespace std;

//Utiliy functions
bool isValid(string u, string v,int k){             //checks whether two strings have a certain k
    if(u.length()<k || v.length()<k){
        return false;
    }

    for (int i = 0; i < k; i++)
    {
        if(v[i]!=u[u.length()-k+i]) return false;
    }
    return true;
}

int check_K(string u ,string v){                    //calculates max k of two strings
    int k = std::min(u.length(), v.length());       //max k possible

    while (k > 0) {
        if (u.substr(u.length() - k) == v.substr(0, k)) return k;
        k--;
    }

    return k;
}


//EDGE AND GRAPH
struct Edge {
    string start;
    string end;
    int k;

    Edge(const string& s, const string& e, int k_value) : start(s), end(e), k(k_value) {}
};

class Graph{
    private:
    void _findMaxPath(string vertex, unordered_map<string, int>& lengths, unordered_map<string, string>& previous){
        if (lengths.find(vertex) != lengths.end()) return;  //if length is already calculated return

        int maxLength = 0;
        string maxW;                
        for ( Edge& e : adjList[vertex]) {                //loop through all neighbours of the vertex
            string w=e.end;
            _findMaxPath(w, lengths, previous);
            if (lengths[w] > maxLength) {
                maxLength = lengths[w];
                maxW = w;
            }
        }

        lengths[vertex] = maxLength + 1;
        previous[vertex] = maxW;
    }

     bool _hasCircuit(string u, unordered_map<string,int>& pre,unordered_map<string,int>& pos,unordered_map<string,string>& pred,int& time){
        pre[u]=time++;
        for (auto& i: adjList[u])
        {
            string v=i.end;
            if(pre[v]==-1) {
                pred[v]=u;
                if(_hasCircuit(v,pre,pos,pred,time)) return true;
            }
            if(pos[v]==-1) return true;
        }
        pos[u]=time++;
        return false;
    }
    
    public:
    int k;                              //k parameter the graph is built with
    int edges;                          //number of edges
    unordered_map<string, vector<Edge>> adjList;

    Graph(int _k){
        k=_k;
        edges=0;
    }

    //builds graph from input file
    void buildGraph(string filename){
        int v;                                   //number of vertices                                 
        int cnt=0;
        string vertex;                 

        ifstream input(filename);

        if (!input) {                                   //if file is not in the same folder  
            cerr << "Arquivo nao encontrado: " << filename << endl;
            exit(1);
        }

        input>>v;

        //building the graph
        while (cnt<v){                           //reads vertices one by one
            input>>vertex;
            adjList[vertex]={};
            cnt++;
        }
        for (auto& u :adjList)
        {
            for(auto& v: adjList){
                if(isValid(u.first,v.first,k)&& u.first!=v.first){
                    adjList[u.first].push_back(Edge(u.first, v.first, check_K(u.first,v.first)));
                    edges++;
                }
            }
        }

        input.close();
    }; 

    bool isInCircuit(string u, string v){           //dfs
        unordered_set<string> visited;              //set to keep the vertices we have visited
        stack<string> stack;
        stack.push(v);

        while(!stack.empty()){
            string cur=stack.top();
            stack.pop(); 
            visited.insert(cur);

            if (cur==u) return true;            //if we find string u, that means the edge is in a circuit

            for(auto& i : adjList[cur]){
                string w=i.end;
                if(visited.find(w)==visited.end()) stack.push(w);   //avoid revisiting vertices
            }
        }

        return false;
    };

    //checks if graph has circuit
    bool hasCircuit(){
        unordered_map<string,int> pre;
        unordered_map<string,int> pos;
        unordered_map<string,string> pred;
        int time=0;
        for(auto&u:adjList){
            pre[u.first]=-1;
            pos[u.first]=-1;
        }
        for (auto& u: adjList)
        {
            if(pre[u.first]==-1){
                pred[u.first]=u.first;
                if(_hasCircuit(u.first,pre,pos,pred,time)) return true;
            }
        }
        return false;
    }

    //removes edges according to k until graph is acyclic
    void removeEdges() {
        while (hasCircuit() && edges>0) {                                //while the graph is cyclic 
            Edge minEdge("", "", numeric_limits<int>::max());       
            for (auto& entry : adjList) {                 //find edge with minimum k
                for (Edge& edge : entry.second) {
                    if (edge.k < minEdge.k) {
                        minEdge = edge;
                    }
                }
            }
        
           adjList[minEdge.start].erase(                //remove it 
                remove_if(adjList[minEdge.start].begin(),
                         adjList[minEdge.start].end(),
                          [minEdge](Edge& edge) {
                              return edge.end == minEdge.end && edge.k == minEdge.k;
                          }),
                adjList[minEdge.start].end());

            edges--;
        }
    }

    //finds max path and crops repetitions before printing the reconstructed sequence
    void findMaxPath(){
        int maxLength=0;
        string start;
        unordered_map<string,int> lengths;          //to keep the lengths of the max paths from each vertice
        unordered_map<string,string> previous;      //keeps track of the previous vertice in the longest path

        for(auto& w: adjList){                      //find maxPath from every vertice
            _findMaxPath(w.first,lengths,previous);
            if(lengths[w.first]>maxLength){
                start=w.first;
                maxLength=lengths[w.first];
            }
        }

        string cur = start;
        string next=start;
        cout<<cur;
        next=previous[cur];
        while (!next.empty()) {         
            int k_parameter=check_K(cur,next);
            cout<<next.substr(k_parameter);
            cur=next;
            next=previous[cur];
        }
    };
    
    // Print the adjacency list of the graph
    void printGraph() {
        for (const auto& entry : adjList) {
            cout << entry.first << ": ";
            for (const Edge& neighbor : entry.second) {
                cout << neighbor.end << " ";
            }
            cout << endl;
        }
        cout<<endl;
    }
};

int main(){ 
    string filename;
    int k;
    
    cout << "Nome do arquivo: "<<endl;           //input comes from file
    cin >> filename;

    cout << "Valor de k: "<<endl;
    cin >> k;
   
    Graph graph = Graph(k);
    cout<<"GRAFO ORIGINAL: "<<endl;
    graph.buildGraph(filename);                  //calls functions that builds graph according to the information on the file
    graph.printGraph();

    if(graph.hasCircuit()){
        cout<<"GRAFO CICLICO: "<<endl;
        graph.removeEdges();
        graph.printGraph();
    }

    cout<<"RECONSTRUCAO DA SEQUENCIA: "<<endl;
    graph.findMaxPath();
}
