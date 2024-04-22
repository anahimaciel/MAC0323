#include <bits/stdc++.h>
using namespace std;

//Some utility functions used in the queries

bool hasRepeat(string word){      //returns true if the word has repeating letters
    unordered_set<char> pastLetters;
    for (char i : word) {
        if (pastLetters.count(i) > 0) {
            return true;  
        }
        pastLetters.insert(i);
    }
    return false; 
}

int difVowels(string word){       //returns number of different vowels in word
    unordered_set<char> vowels = {'a', 'e', 'i', 'o', 'u'};
    int n = 0;
    for (char i : word) {
        char lowerI = tolower(i);
        if (vowels.count(lowerI) > 0) {
            n++;
            vowels.erase(lowerI);  // removes vowel from set 
        }
    }
    return n;
}

//Key and item structs

struct Key {
    string k;
    Key(string s){
        k=s;
    }
    Key(){
        k="";
    }
};

struct Item { 
    int freq;
    int length;
    int vowels;

    Item(int f, int l, int v){
        freq=f;
        length=l;
        vowels=v;
    }
    Item(){
        freq=0;
        length=0;
        vowels=0;
    }
};

//Node classes

class Node {            //for BSTs and treaps
    public:
    Node* right;
    Node* left;
    struct Key key;
    struct Item value;
    int height;                                             //for treaps

    Node (Key k, Item v, Node* r, Node* l) {                //for BSTs              
        key = k;
        value = v;
        right=r;
        left=l;
        height=0;
    };

    Node (Key k, Item v, Node* r, Node* l, int h) {         //for treaps
        key = k;
        value = v;
        right=r;
        left=l;
        height=h;
    };    
};

class NodeARN {         //for RBTs
    public:
    NodeARN* right;
    NodeARN* left;
    NodeARN* parent;
    struct Key key;
    struct Item value;
    bool color;                 //true:red,false:black

    NodeARN (Key k, Item v, NodeARN* r, NodeARN* l, NodeARN* p, bool c) {                           
        key = k;
        value = v;
        right=r;
        left=l;
        parent=p;
        color=c;
    };
};

class NodeA23{          //for 2-3 trees
    public:
    struct Key key1;
    struct Item value1;
    struct Key key2;
    struct Item value2;
    NodeA23* leftPtr;
    NodeA23* midPtr;
    NodeA23* rightPtr;
    bool is_2node;     //whether the node is a 2 node or not. true: 1 element and 2 pointers, false: 2 elements and 3 pointers

    NodeA23(Key k1, Item v1, NodeA23* p1, NodeA23* p2){
        key1=k1;
        value1=v1;
        leftPtr=p1;
        midPtr=p2;
        rightPtr=nullptr;
        key2=Key();
        value2=Item();
        is_2node=true;
    }
};

class NodeVDO {         //for ordered dynamic vectors
    public:
    struct Key key;
    struct Item value;                                      
 
    NodeVDO(Key k, Item v){
        key=k;
        value=v;
    };
};

//Structure classes

class VDO {            //ordered dynamic vector
    private:
    vector<NodeVDO> *vec;

    public:
    VDO() {
        vec = new vector<NodeVDO>;
    }

    void add(Key key, Item value){
        auto it =lower_bound(vec->begin(),vec->end(),NodeVDO(key,value),[](const NodeVDO& a ,const NodeVDO& b) {
            return a.key.k<b.key.k;
        });
        if(it!=vec->end() && it->key.k==key.k) it->value=value;
        else vec->insert(it,NodeVDO(key,value));  
    }

    Item value(Key key) {
        if (vec->empty()) {
            return(Item());
        }
        auto l=vec->begin();
        auto r= vec->end()-1;
        while(l<=r){
            auto mid=l+(r-l)/2;
            if(mid->key.k==key.k)
            {
                return mid->value;
            }
            if(mid->key.k > key.k)
                r = mid-1;
            else
                l = mid+1;
        }
        return(Item());
    };


    vector<string> mostFrequent(){
        int maxFrequency=0;
        vector<string> words;
        for (auto it = vec->begin(); it < vec->end(); it++)
        {   
            if (it->value.freq > maxFrequency) {
                words.clear();
                words.push_back(it->key.k);
                maxFrequency=it->value.freq;
            } 
            else if (it->value.freq== maxFrequency) {
                words.push_back(it->key.k);
            }
        }
        return words;
    }

    vector<string> longestWords(){
        int maxLength=0;
        vector<string> words;
        for (auto it = vec->begin(); it < vec->end(); it++)
        {   
            if (it->value.length > maxLength) {
                words.clear();
                words.push_back(it->key.k);
                maxLength=it->value.length;
            } 
            else if (it->value.length== maxLength) {
                words.push_back(it->key.k);
            }
        }
        return words;
    }

    vector<string> longestNonRepeat(){
        int maxLength = 0;
        vector<string> words;

        for (auto it = vec->begin(); it < vec->end(); it++)
        {   
            if(!hasRepeat(it->key.k)){
                if (it->value.length > maxLength) {
                    words.clear();
                    words.push_back(it->key.k);
                    maxLength=it->value.length;
                } 
                else if (it->value.length== maxLength) {
                    words.push_back(it->key.k);
                }
            }
        }
    
        return words;
    }

    vector<string> shortestNonRepeatVowels(){
        vector<string> words;
        int maxVowels = 0;
        int shortestLength = INT_MAX;

        for (auto it = vec->begin(); it < vec->end(); it++)
        {
            if (it->value.vowels > maxVowels) {
                maxVowels = it->value.vowels;
                shortestLength = it->value.length;
                words.clear();
                words.push_back(it->key.k);
            } 
            else if (it->value.vowels == maxVowels && it->value.length < shortestLength) {
                shortestLength = it->value.length;
                words.clear();
                words.push_back(it->key.k);
            } 
            else if (it->value.vowels == maxVowels && it->value.length == shortestLength) {
                words.push_back(it->key.k);
            }
        }
        return words;
    }
};

class ABB {            //binary search tree
    private:
    Node* root;

    Node* put (Node* node, Key key, Item value) {
        if (node==nullptr) {
            node = new Node(key,value,nullptr,nullptr);
            return node;
        }

        if (node->key.k==key.k){
            node->value=value;
            return node;
        }

        if (node->key.k < key.k) {
            node->right=put(node->right,key,value);
            return node;
        }

        node->left=put(node->left,key,value);
        return node;
    }

    Item get (Node* node, Key key) {
        if (node==nullptr) return Item();
        if (node->key.k == key.k) return node->value;
        if (node->key.k < key.k) return get(node->right,key);
        return get(node->left,key);
    }

    void _mostFrequent(Node* node, vector<string>* words, int& maxFreq){
        if(node==nullptr) return;

        _mostFrequent(node->left,words,maxFreq);

        if (node->value.freq > maxFreq) {
            words->clear();
            words->push_back(node->key.k);
            maxFreq=node->value.freq;
        } 
        else if (node->value.freq== maxFreq) {
            words->push_back(node->key.k);
        }
        
        _mostFrequent(node->right,words,maxFreq);
    }

    
    void _longestWords(Node* node, vector<string>* words, int& maxLength){
        if(node==nullptr) return;

        _longestWords(node->left,words,maxLength);

        if (node->value.length > maxLength) {
            words->clear();
            words->push_back(node->key.k);
            maxLength=node->value.length;
        } 
        else if (node->value.length== maxLength) {
            words->push_back(node->key.k);
        }
        
        _longestWords(node->right,words,maxLength);
    }

    void _longestNonRepeat(Node* node, vector<string>* words, int& maxLength){
        if(node==nullptr) return;

        _longestNonRepeat(node->left,words,maxLength);

        if(!hasRepeat(node->key.k)){
            if (node->value.length > maxLength) {
                words->clear();
                words->push_back(node->key.k);
                maxLength=node->value.length;
            } 
            else if (node->value.length== maxLength) {
                words->push_back(node->key.k);
            }
        }
        
        _longestNonRepeat(node->right,words,maxLength);
    }

    void _shortestNonRepeatVowels(Node* node, vector<string> *words, int& maxVowels, int& shortestLength){
        if(node==nullptr) return;

        _shortestNonRepeatVowels(node->left,words,maxVowels,shortestLength);
        
            if (node->value.vowels > maxVowels) {
                maxVowels = node->value.vowels;
                shortestLength =node->value.length;
                words->clear();
                words->push_back(node->key.k);
            } 
            else if (node->value.vowels == maxVowels && node->value.length < shortestLength) {
                shortestLength = node->value.length;
                words->clear();
                words->push_back(node->key.k);
            } 
            else if (node->value.vowels == maxVowels && node->value.length == shortestLength) {
                words->push_back(node->key.k);
            }
        
        _shortestNonRepeatVowels(node->right,words,maxVowels,shortestLength);
    }    
    public:

    ABB() {
        root=nullptr;
    };

    void add(Key key, Item value) {
        root=put(root,key,value);
    };

    Item value (Key key) {
        return get(root,key);
    };

    vector<string> mostFrequent(){
        vector <string>* words = new vector<string>;
        int maxFreq=0;
        _mostFrequent(root,words,maxFreq);
        return *words;
    }

    vector<string> longestWords(){
        vector <string>* words = new vector<string>;
        int maxLength=0;
        _longestWords(root,words,maxLength);
        return *words;
    }

    vector<string> longestNonRepeat(){
        vector <string>* words = new vector<string>;
        int maxLength=0;
        _longestNonRepeat(root,words,maxLength);
        return *words;
    }

    vector<string> shortestNonRepeatVowels(){
        vector <string>* words = new vector<string>;
        int maxVowels = 0;
        int shortestLength = INT_MAX;
        _shortestNonRepeatVowels(root,words,maxVowels,shortestLength);
        return *words;
    }
};

class TR {             //treap
    private:
    Node* root;

    Node* rotateRight(Node* node) {
        if (node->left != nullptr) {
            Node* aux = node->left;
            node->left = aux->right;
            aux->right=node;
            return aux;
        }
        return node;
    };

    Node* rotateLeft(Node* node) {
        if (node->right != nullptr) {
            Node* aux = node->right;
            node->right = aux->left;
            aux->left=node;
            return aux;
        }
        return node;
    };

    Node* put (Node* node, Key key, Item value) {
        if (node==nullptr) {
            int height=rand()%100;
            node = new Node(key,value,nullptr,nullptr,height);
            return node;
        }

        if (node->key.k==key.k){
            node->value=value;
            return node;
        }

        if (node->key.k < key.k) {
            node->right=put(node->right,key,value);
            if(node->right->height > node->height) node=rotateLeft(node);          //to comply with maxHeap property
            return node;
        }

        node->left=put(node->left,key,value);
        if(node->left->height > node->height) node=rotateRight(node);
        return node;
    }

    Item get (Node* node, Key key) {
        if (node==nullptr) return Item();
        if (node->key.k == key.k) return node->value;
        if (node->key.k < key.k) return get(node->right,key);
        return get(node->left,key);
    }

    void _mostFrequent(Node* node, vector<string> *words, int& maxFreq){
        if(node==nullptr) return;

        _mostFrequent(node->left,words,maxFreq);

        if (node->value.freq > maxFreq) {
            words->clear();
            words->push_back(node->key.k);
            maxFreq=node->value.freq;
        } 
        else if (node->value.freq== maxFreq) {
            words->push_back(node->key.k);
        }
        
        _mostFrequent(node->right,words,maxFreq);
    }

    void _longestWords(Node* node, vector<string> *words, int& maxLength){
        if(node==nullptr) return;

        _longestWords(node->left,words,maxLength);

        if (node->value.length > maxLength) {
            words->clear();
            words->push_back(node->key.k);
            maxLength=node->value.length;
        } 
        else if (node->value.length== maxLength) {
            words->push_back(node->key.k);
        }
        
        _longestWords(node->right,words,maxLength);
    }

    void _longestNonRepeat(Node* node, vector<string>* words, int& maxLength){
        if(node==nullptr) return;

        _longestNonRepeat(node->left,words,maxLength);

        if(!hasRepeat(node->key.k)){
            if (node->value.length > maxLength) {
                words->clear();
                words->push_back(node->key.k);
                maxLength=node->value.length;
            } 
            else if (node->value.length== maxLength) {
                words->push_back(node->key.k);
            }
        }
        
        _longestNonRepeat(node->right,words,maxLength);
    }

    void _shortestNonRepeatVowels(Node* node, vector<string>* words, int& maxVowels, int& shortestLength){
        if(node==nullptr) return;

        _shortestNonRepeatVowels(node->left,words,maxVowels,shortestLength);
        
            if (node->value.vowels > maxVowels) {
                maxVowels = node->value.vowels;
                shortestLength =node->value.length;
                words->clear();
                words->push_back(node->key.k);
            } 
            else if (node->value.vowels == maxVowels && node->value.length < shortestLength) {
                shortestLength = node->value.length;
                words->clear();
                words->push_back(node->key.k);
            } 
            else if (node->value.vowels == maxVowels && node->value.length == shortestLength) {
                words->push_back(node->key.k);
            }
        
        _shortestNonRepeatVowels(node->right,words,maxVowels,shortestLength);
    }

    public:
    TR() {
        root=nullptr;
    };

    void add(Key key, Item value) {
        root=put(root,key,value);
    };

    Item value (Key key) {
        return get(root,key);
    };

    vector<string> mostFrequent(){
        vector <string> *words=new vector<string>;
        int maxFreq=0;
        _mostFrequent(root,words,maxFreq);
        return *words;
    }

    vector<string> longestWords(){
        vector <string> *words=new vector<string>;
        int maxLength=0;
        _longestWords(root,words,maxLength);
        return *words;
    }

    vector<string> longestNonRepeat(){
        vector <string> *words=new vector<string>;
        int maxLength=0;
        _longestNonRepeat(root,words,maxLength);
        return *words;
    }

    vector<string> shortestNonRepeatVowels(){
        vector <string> *words=new vector<string>;
        int maxVowels = 0;
        int shortestLength = INT_MAX;
        _shortestNonRepeatVowels(root,words,maxVowels,shortestLength);
        return *words;
    }
};

class A23 {            //2-3 tree
    private:
    NodeA23* root;

    NodeA23* put(NodeA23* node, Key key, Item value, bool& hasGrown){
        if (node == nullptr) {
		    node = new NodeA23(key,value,nullptr,nullptr);
		    hasGrown=true;
		    return node;
	    }
        if (node->key1.k==key.k){                                           //checking if key is in tree
            node->value1=value;
            return node;
        }
        if(!node->is_2node && node->key2.k==key.k){
            node->value2=value;
            return node;
        }
        if (node->leftPtr != nullptr) {                                      // if node is not a leaf
            if (node->key1.k>key.k) {
		        NodeA23* p = put(node->leftPtr, key, value,hasGrown); 
		        if (hasGrown) {                                               //if tree has grown
                    if (node->is_2node) {                                     //if node is 2node
                        node->key2=node->key1;
                        node->value2=node->value1;
                        node->rightPtr=node->midPtr;
                        node->key1=p->key1;
                        node->value1=p->value1;
                        node->midPtr=p->midPtr;
                        node->leftPtr=p->leftPtr;
                        hasGrown=false;
                        node->is_2node=false;
                        return node;
			        }
				    else {                                                    //if node is 3node
					    NodeA23* newNode= new NodeA23(node->key2,node->value2,node->midPtr,node->rightPtr); 
                        NodeA23* newRoot= new NodeA23(node->key1,node->value1,node,newNode);                
                        node->key1=p->key1;
                        node->value1=p->value1;
                        node->key2=Key();
                        node->value2=Item();
                        node->rightPtr=nullptr;
                        node->midPtr=p->midPtr;
                        node->is_2node=true;
                        hasGrown = true;
					    return newRoot;
				    }
		        }
		        else {
			        node->leftPtr = p;
                    hasGrown=false;
		            return node;
			    }                                                                       
		    }
		    else {                                                  // key is greater than node->key1
			    if (node->is_2node) {                               // node is 2node
				    NodeA23* p = put(node->midPtr, key,value, hasGrown); 
				    if (hasGrown) {                                 //if tree has grown
                        node->key2=p->key1;
                        node->value2=p->value1;
                        node->rightPtr=p->midPtr;
                        node->midPtr=p->leftPtr;
                        hasGrown=false;
                        node->is_2node=false;
                        return node;
				    } 
                    else {                                          //if tree has not grown
					    node->midPtr = p;
					    return node; 
				    }
			    }
			    else {                                              // if the node is a 3node
				    if (node->key2.k <key.k) {                     
				        NodeA23* p = put(node->rightPtr, key,value, hasGrown); 
				        if (hasGrown) {
                            NodeA23* newNode= new NodeA23(p->key1,p->value1,p->leftPtr,p->midPtr); 
                            NodeA23* newRoot= new NodeA23(node->key2,node->value2,node,newNode);                

                            node->key2=Key();
                            node->value2=Item();
                            node->rightPtr=nullptr;
                            node->is_2node=true;

                            hasGrown = true; 
					        return newRoot;
					    }
					    else 
                        {
                            node->rightPtr = p;
					        return node;
				        }
		    	    }
				    else {                                          // if node->key1<k<node->key2
					    NodeA23* p = put(node->midPtr, key, value, hasGrown); 
					    if (hasGrown) 
                        {
                            NodeA23* newNode= new NodeA23(node->key2,node->value2,p->midPtr,node->rightPtr); 
                            NodeA23* newRoot= new NodeA23(p->key1,p->value1,node,newNode);                

                            node->key2=Key();
                            node->value2=Item();
                            node->rightPtr=nullptr;
                            node->midPtr=p->leftPtr;
                            node->is_2node=true;
                            hasGrown = true;
					        return newRoot;
					    } 
                        else 
                        {
                            node->midPtr = p;
					        return node; 
					    }
				    }
			    }
		    }
	    }
	    else {                                                                  //if node is a leaf
		    if (node->is_2node) {                                               //if node is a 2node
			    if (node->key1.k>key.k) { 
                    node->key2=node->key1;
                    node->value2=node->value1;
                    node->key1=key;
                    node->value1=value;
                    node->is_2node=false;
				    hasGrown = false;
				    return node;
			    }
		    	else {                                                          // key>node->key1
				    node->key2 = key;
                    node->value2 = value;
				    node->is_2node=false;
				    hasGrown=false;
				    return node;
			    }
		    } 
		    else {                                                              //if node is a 3node
                if (key.k < node->key1.k) {                                     //key<key1
                    NodeA23* newNode=new NodeA23(node->key2,node->value2,nullptr,nullptr);
                    NodeA23* newRoot=new NodeA23(node->key1,node->value1,node,newNode);
                    
                    node->key1=key;
                    node->value1=value;
                    node->key2=Key();
                    node->value2=Item();
                    node->is_2node=true;
                    hasGrown=true;
                    return newRoot;
			    } 
                else if (key.k < node->key2.k) {                                //key is between key1 and key2 
                    NodeA23* newNode=new NodeA23(node->key2,node->value2,nullptr,nullptr);
                    NodeA23* newRoot=new NodeA23(key,value,node,newNode);

                    node->key2=Key();
                    node->value2=Item();
                    node->is_2node=true;
                    hasGrown=true;
                    return newRoot;
			    }
			    else {                                                         //key>key2
                    NodeA23* newNode=new NodeA23(key,value,nullptr,nullptr);
                    NodeA23* newRoot=new NodeA23(node->key2,node->value2,node,newNode);

                    node->key2=Key();
                    node->value2=Item();
                    node->is_2node=true;
                    hasGrown=true;
                    return newRoot;
			    }
		    }
	    }       
    }

    Item get(NodeA23* node, Key key){
        if (!node) {
		    return Item();                                              //if node is nullptr, return empty item
	    }
        if (key.k == node->key1.k) {
            return node->value1;
        } 
        else if (!node->is_2node && key.k == node->key2.k) {
            return node->value2;
        } 
        else if (key.k < node->key1.k) {
            return get(node->leftPtr, key);
        } 
        else if (!node->is_2node && key.k > node->key2.k) {
            return get(node->rightPtr, key);
        } 
        else {
            return get(node->midPtr, key);
        }
    }

    void _mostFrequent(NodeA23* node, vector<string>& words, int& maxFreq){
        if (node== nullptr) {
            return;
        }

        _mostFrequent(node->leftPtr, words, maxFreq);

        if (node->value1.freq > maxFreq) {
            maxFreq = node->value1.freq;
            words.clear();
            words.push_back(node->key1.k);
        } 
        else if (node->value1.freq == maxFreq) {
            words.push_back(node->key1.k);
        }
        if(!node->is_2node){
            if (node->value2.freq > maxFreq) {
                maxFreq = node->value2.freq;
                words.clear();
                words.push_back(node->key2.k);
            } else if (node->value2.freq == maxFreq) {
                words.push_back(node->key2.k);
            }
        }

        _mostFrequent(node->midPtr, words, maxFreq);
        _mostFrequent(node->rightPtr, words, maxFreq);    
    }

    void _longestWords(NodeA23* node, vector<string>& words, int& maxLength){
        if (node== nullptr) {
            return;
        }

        _longestWords(node->leftPtr, words, maxLength);

        if (node->value1.length > maxLength) {
            maxLength = node->value1.length;
            words.clear();
            words.push_back(node->key1.k);
        } 
        else if (node->value1.length == maxLength) {
            words.push_back(node->key1.k);
        }
        if(!node->is_2node){
            if (node->value2.length > maxLength) {
                maxLength = node->value2.length;
                words.clear();
                words.push_back(node->key2.k);
            } else if (node->value2.length == maxLength) {
                words.push_back(node->key2.k);
            }
        }

        _longestWords(node->midPtr, words, maxLength);
        _longestWords(node->rightPtr, words, maxLength);
    }

    void _longestNonRepeat(NodeA23* node, vector<string>& words, int& maxLength){
        if (node== nullptr) {
            return;
        }

        _longestNonRepeat(node->leftPtr, words, maxLength);

        if(!hasRepeat(node->key1.k)){
            if (node->value1.length > maxLength) {
                maxLength = node->value1.length;
                words.clear();
                words.push_back(node->key1.k);
            } 
            else if (node->value1.length == maxLength) {
                words.push_back(node->key1.k);
            }
        }

        if(!node->is_2node && !hasRepeat(node->key2.k)){
            if (node->value2.length > maxLength) {
                maxLength = node->value2.length;
                words.clear();
                words.push_back(node->key2.k);
            } else if (node->value2.length == maxLength) {
                words.push_back(node->key2.k);
            }
        }

        _longestNonRepeat(node->midPtr, words, maxLength);
        _longestNonRepeat(node->rightPtr, words, maxLength);
    }

    void _shortestNonRepeatVowels(NodeA23* node, vector<string>& words, int& maxVowels, int& shortestLength){
        if(node==nullptr) return;

        _shortestNonRepeatVowels(node->leftPtr,words,maxVowels,shortestLength);
        
        if (node->value1.vowels > maxVowels) {
            maxVowels = node->value1.vowels;
            shortestLength =node->value1.length;
            words.clear();
            words.push_back(node->key1.k);
        } 
        else if (node->value1.vowels == maxVowels && node->value1.length < shortestLength) {
            shortestLength = node->value1.length;
            words.clear();
            words.push_back(node->key1.k);
        } 
        else if (node->value1.vowels == maxVowels && node->value1.length == shortestLength) {
            words.push_back(node->key1.k);
        }
        
        if(!node->is_2node){
            if (node->value2.vowels > maxVowels) {
                maxVowels = node->value2.vowels;
                shortestLength =node->value2.length;
                words.clear();
                words.push_back(node->key2.k);
            } 
            else if (node->value2.vowels == maxVowels && node->value2.length < shortestLength) {
                shortestLength = node->value2.length;
                words.clear();
                words.push_back(node->key2.k);
            } 
            else if (node->value2.vowels == maxVowels && node->value2.length == shortestLength) {
                words.push_back(node->key2.k);
            }
        }

        _shortestNonRepeatVowels(node->midPtr,words,maxVowels,shortestLength);
        _shortestNonRepeatVowels(node->rightPtr,words,maxVowels,shortestLength);
    }
    
    public:
    A23() {
        root=nullptr;
    }

    void add(Key key, Item value) {
        bool hasGrown = false;        
        root=put(root,key,value,hasGrown);
    };

    Item value (Key key) {
        return get(root,key);
    };

    vector<string> mostFrequent(){
        vector<string> words;
        int maxFreq = 0;
        _mostFrequent(root, words, maxFreq);
        return words;
    }

     vector<string> longestWords(){
        vector<string> words;
        int maxLength = 0;
        _longestWords(root, words, maxLength);
        return words;
    }

    vector<string> longestNonRepeat(){
        vector<string> words;
        int maxLength = 0;
        _longestNonRepeat(root, words, maxLength);
        return words;
    }

    vector<string> shortestNonRepeatVowels(){
        vector <string> words;
        int maxVowels = 0;
        int shortestLength = INT_MAX;
        _shortestNonRepeatVowels(root,words,maxVowels,shortestLength);
        return words;
    }
};

class ARN {            //red black tree
    private:
    NodeARN* root;

    void leftRotate(NodeARN* node) {
		NodeARN* aux = node->right;
		node->right = aux->left;
		if (aux->left != nullptr) {
			aux->left->parent = node;
		}
		aux->parent =node->parent;
		if (node->parent == nullptr) {
			this->root = aux;
		} else if (node == node->parent->left) {
			node->parent->left = aux;
		} else {
			node->parent->right = aux;
		}
		aux->left = node;
		node->parent = aux;
	}

	void rightRotate(NodeARN* node) {
		NodeARN* aux = node->left;
		node->left = aux->right;
		if (aux->right != nullptr) {
			aux->right->parent = node;
		}
		aux->parent = node->parent;
		if (node->parent == nullptr) {
			this->root = aux;
		} else if (node == node->parent->right) {
			node->parent->right = aux;
		} else {
			node->parent->left = aux;
		}
		aux->right = node;
		node->parent = aux;
	}

    void fixTree(NodeARN* node){
		NodeARN* uncle;
		while (node->parent->color) {                                   //while the color of the parent is red
        //(if the color of the parent is black, we have fixed the tree)    
			if (node->parent == node->parent->parent->right) {          //parent>grandparent
				uncle = node->parent->parent->left;                  
				if (uncle && uncle->color) {                            //parent and uncle are red
					uncle->color = node->parent->color = false;
					node->parent->parent->color = true;
					node = node->parent->parent;
				} 
                else {	                                                //black uncle
                    if (node == node->parent->left) {                   //node<parent
						node=node->parent;
                        rightRotate(node);
					}
					node->parent->color = false;
					node->parent->parent->color = true;
                    leftRotate(node->parent->parent);
				}
			} 
            
            else {                                                      //parent<grandparent
				uncle = node->parent->parent->right;                    
				if (uncle && uncle->color) {                            //parent and uncle are red
					uncle->color = node->parent->color = false;
					node->parent->parent->color = true;
					node = node->parent->parent;
				} 
                else {                                                  //black uncle
					if (node == node->parent->right) {
						node =node->parent;
                        leftRotate(node);
					}
					node->parent->color = false;
					node->parent->parent->color = true;
                    rightRotate(node->parent->parent);
				}
			}
            if (node == root) {                                        //exit the loop when we get to the root
				break;
			}
		}
		root->color = false;                                           //and paint the root black 
	}

    NodeARN* put (NodeARN* node, Key key, Item value) {
        if (node==nullptr) {
            node = new NodeARN(key,value,nullptr,nullptr,nullptr,true);
            return node;
        }

        if (node->key.k==key.k){
            node->value=value;
            return node;
        }

        if (node->key.k < key.k) {
            node->right=put(node->right,key,value);
            return node;
        }

        node->left=put(node->left,key,value);
        return node;
    }

    Item get (NodeARN* node, Key key) {
        if (node==nullptr) return Item();
        if (node->key.k == key.k) return node->value;
        if (node->key.k < key.k) return get(node->right,key);
        return get(node->left,key);
    }

    void _mostFrequent(NodeARN* node, vector<string> *words, int& maxFreq){
        if(node==nullptr) return;

        _mostFrequent(node->left,words,maxFreq);

        if (node->value.freq > maxFreq) {
            words->clear();
            words->push_back(node->key.k);
            maxFreq=node->value.freq;
        } 
        else if (node->value.freq== maxFreq) {
            words->push_back(node->key.k);
        }
        
        _mostFrequent(node->right,words,maxFreq);
    }

    void _longestWords(NodeARN* node, vector<string>* words, int& maxLength){
        if(node==nullptr) return;

        _longestWords(node->left,words,maxLength);

        if (node->value.length > maxLength) {
            words->clear();
            words->push_back(node->key.k);
            maxLength=node->value.length;
        } 
        else if (node->value.length== maxLength) {
            words->push_back(node->key.k);
        }
        
        _longestWords(node->right,words,maxLength);
    }

    void _longestNonRepeat(NodeARN* node, vector<string>* words, int& maxLength){
        if(node==nullptr) return;

        _longestNonRepeat(node->left,words,maxLength);

        if(!hasRepeat(node->key.k)){
            if (node->value.length > maxLength) {
                words->clear();
                words->push_back(node->key.k);
                maxLength=node->value.length;
            } 
            else if (node->value.length== maxLength) {
                words->push_back(node->key.k);
            }
        }
        
        _longestNonRepeat(node->right,words,maxLength);
    }

    void _shortestNonRepeatVowels(NodeARN* node, vector<string> *words, int& maxVowels, int& shortestLength){
        if(node==nullptr) return;

        _shortestNonRepeatVowels(node->left,words,maxVowels,shortestLength);
        
            if (node->value.vowels > maxVowels) {
                maxVowels = node->value.vowels;
                shortestLength =node->value.length;
                words->clear();
                words->push_back(node->key.k);
            } 
            else if (node->value.vowels == maxVowels && node->value.length < shortestLength) {
                shortestLength = node->value.length;
                words->clear();
                words->push_back(node->key.k);
            } 
            else if (node->value.vowels == maxVowels && node->value.length == shortestLength) {
                words->push_back(node->key.k);
            }
        
        _shortestNonRepeatVowels(node->right,words,maxVowels,shortestLength);
    }
    public:

    ARN() {
        root=nullptr;
    }

    void add(Key key, Item value) {
        root = put(root,key,value);         //adding the node with simple binary search tree insertion

        NodeARN* node=root; 
        NodeARN* p= nullptr;
        while(node->key.k != key.k){        //finding the node
            if(node->key.k>key.k) {
                p=node;
                node=node->left;
            }
            else if(node->key.k<key.k)  
            {
                p=node;
                node=node->right;
            }
        }
        if (p) node->parent=p;

		if (node->parent == nullptr){       //if node is root: simply paint it black and return
			node->color = false;
			return;
		}

	    if (node->parent->parent == nullptr) {  //if node's parent is root: no fixing required
			return;
		}

		fixTree(node);	
    };

    Item value (Key key) {
        return get(root,key);
    };

    vector<string> mostFrequent(){
        vector <string> *words=new vector<string>;
        int maxFreq=0;
        _mostFrequent(root,words,maxFreq);
        return *words;
    }

    vector<string> longestWords(){
        vector <string> *words=new vector<string>;
        int maxLength=0;
        _longestWords(root,words,maxLength);
        return *words;
    }

    vector<string> longestNonRepeat(){
        vector <string> *words=new vector<string>;
        int maxLength=0;
        _longestNonRepeat(root,words,maxLength);
        return *words;
    }

    vector<string> shortestNonRepeatVowels(){
        vector <string> *words=new vector<string>;
        int maxVowels = 0;
        int shortestLength = INT_MAX;
        _shortestNonRepeatVowels(root,words,maxVowels,shortestLength);
        return *words;
    }
};

//Main 

int main(){
    srand(time(nullptr));                               //initializing pseudo-random number generator
    
    string structure;                                   //structure we will work with
    string word;                                        
    string query;                                       //query type
    string queryWord;                                   //for "O" query: which word we want the frequency of
    int numWords;                                       //number of words
    int numQueries;                                     //number of queries
    int cnt=0;

    string filename = "dickens.txt";           //text comes from file
    ifstream input(filename);
    
    if (!input) {                                       //if file is not in the same folder  
        cerr << "Arquivo nao encontrado: " << filename << endl;
        exit(1);
    }

    input>>structure>>numWords;

    //Queries
    if(structure=="VO"){
        VDO str;
        while(cnt<numWords){
            input>>word;            
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end()); //remove punctuation
            transform(word.begin(), word.end(), word.begin(), ::tolower);           //to lower case

            if(!word.empty()){                    //word is empty if it was only punctuation, so it does not count as a word
                if(str.value(Key(word)).freq==0){ //word is not in structure
                str.add(Key(word),Item(1,word.length(),difVowels(word)));   //add to tree
                }
                else{                             //word is in structure
                    int n = str.value(Key(word)).freq;
                    str.add(Key(word),Item(n+1,word.length(),difVowels(word)));  //frequency++
                };
                cnt++;
            }
        }
        input>>numQueries;  //number of queries
        for (int i = 0; i < numQueries; i++)
        {
            input>>query;   
            if (query=="F"){
                vector<string> result = str.mostFrequent();
                if(!result.empty()){                            //print results
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="O"){
                input>>queryWord;
                transform(queryWord.begin(), queryWord.end(), queryWord.begin(), ::tolower); 
                cout << str.value(Key(queryWord)).freq<<endl;
            }
            else if(query=="L"){
                vector<string> result = str.longestWords();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="SR"){
                vector<string> result = str.longestNonRepeat();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="VD"){
                vector<string> result = str.shortestNonRepeatVowels();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else{
                cout<<"Invalid query"<<endl;
            }
        }
        
    }
    else if (structure=="ABB"){
        ABB str;
        while(cnt<numWords){
            input>>word;
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end()); //remove punctuation
            transform(word.begin(), word.end(), word.begin(), ::tolower);           //to lower case

            if(!word.empty()){
                if(str.value(Key(word)).freq==0){ //word is not in structure
                str.add(Key(word),Item(1,word.length(),difVowels(word)));
                }
                else{                             //word is in structure
                    int n = str.value(Key(word)).freq;
                    str.add(Key(word),Item(n+1,word.length(),difVowels(word)));
                };
                cnt++;
            }
        }
        input>>numQueries;
        for (int i = 0; i < numQueries; i++)
        {
            input>>query;
            if (query=="F"){
                vector<string> result = str.mostFrequent();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="O"){
                input>>queryWord;
                transform(queryWord.begin(), queryWord.end(), queryWord.begin(), ::tolower); 
                cout << str.value(Key(queryWord)).freq<<endl;
            }
            else if(query=="L"){
                vector<string> result = str.longestWords();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="SR"){
                vector<string> result = str.longestNonRepeat();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="VD"){
                vector<string> result = str.shortestNonRepeatVowels();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else{
                cout<<"Invalid query"<<endl;
            }
        }
    }
    else if (structure=="TR"){
        TR str;
        while(cnt<numWords){
            input>>word;
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end()); //remove punctuation
            transform(word.begin(), word.end(), word.begin(), ::tolower);           //to lower case

            if(!word.empty()){
                if(str.value(Key(word)).freq==0){ //word is not in structure
                str.add(Key(word),Item(1,word.length(),difVowels(word)));
                }
                else{                             //word is in structure
                    int n = str.value(Key(word)).freq;
                    str.add(Key(word),Item(n+1,word.length(),difVowels(word)));
                };
                cnt++;
            }
        }
        input>>numQueries;
        for (int i = 0; i < numQueries; i++)
        {
            input>>query;
            if (query=="F"){
                vector<string> result = str.mostFrequent();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="O"){
                input>>queryWord;
                transform(queryWord.begin(), queryWord.end(), queryWord.begin(), ::tolower); 
                cout << str.value(Key(queryWord)).freq<<endl;
            }
            else if(query=="L"){
                vector<string> result = str.longestWords();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="SR"){
                vector<string> result = str.longestNonRepeat();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="VD"){
                vector<string> result = str.shortestNonRepeatVowels();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else{
                cout<<"Invalid query"<<endl;
            }
        }
    }
    else if (structure=="A23"){
        A23 str;
        while(cnt<numWords){
            input>>word;
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end()); //remove punctuation
            transform(word.begin(), word.end(), word.begin(), ::tolower);           //to lower case

            if(!word.empty()){
                if(str.value(Key(word)).freq==0){ //word is not in structure
                str.add(Key(word),Item(1,word.length(),difVowels(word)));
                }
                else{                             //word is in structure
                    int n = str.value(Key(word)).freq;
                    str.add(Key(word),Item(n+1,word.length(),difVowels(word)));
                };
                cnt++;
            }
        }
        input>>numQueries;
        for (int i = 0; i < numQueries; i++)
        {
            input>>query;
            if (query=="F"){
                vector<string> result = str.mostFrequent();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="O"){
                input>>queryWord;
                transform(queryWord.begin(), queryWord.end(), queryWord.begin(), ::tolower); 
                cout << str.value(Key(queryWord)).freq<<endl;
            }
            else if(query=="L"){
                vector<string> result = str.longestWords();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="SR"){
                vector<string> result = str.longestNonRepeat();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="VD"){
                vector<string> result = str.shortestNonRepeatVowels();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else{
                cout<<"Invalid query"<<endl;
            }
        }
    }
    else if (structure=="ARN"){
        ARN str;
        while(cnt<numWords){
            input>>word;
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end()); //remove punctuation
            transform(word.begin(), word.end(), word.begin(), ::tolower);           //to lower case

            if(!word.empty()){
                if(str.value(Key(word)).freq==0){ //word is not in structure
                str.add(Key(word),Item(1,word.length(),difVowels(word)));
                }
                else{                             //word is in structure
                    int n = str.value(Key(word)).freq;
                    str.add(Key(word),Item(n+1,word.length(),difVowels(word)));
                };
                cnt++;
            }
        }
        input>>numQueries;

        for (int i = 0; i < numQueries; i++)
        {
            input>>query;
            if (query=="F"){
                vector<string> result = str.mostFrequent();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="O"){
                input>>queryWord;
                transform(queryWord.begin(), queryWord.end(), queryWord.begin(), ::tolower); 
                cout << str.value(Key(queryWord)).freq<<endl;
            }
            else if(query=="L"){
                vector<string> result = str.longestWords();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="SR"){
                vector<string> result = str.longestNonRepeat();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else if(query=="VD"){
                vector<string> result = str.shortestNonRepeatVowels();
                if(!result.empty()){
                    for (auto it = result.begin(); it < result.end(); it++)
                    {
                        cout<< *it<<" ";
                    }
                }
                cout<<endl;
            }
            else{
                cout<<"Consulta invalida"<<endl;
            }
        }
    }
    else cout<<"Estrutura invalida"<<endl;

    input.close();
}
