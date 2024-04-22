#include <bits/stdc++.h>
using namespace std;

bool getArrival(float pp) {
    return ((float) rand()/RAND_MAX<=pp);
}

bool getEmergency(float pe) {
    return ((float) rand()/RAND_MAX<=pe);
}

class Runway {
    public:
    bool occupied;
    int timeLeft;
    bool canLand;               //false: runway only for take-offs, unless there is an emergency

    Runway(bool cl) {                      //constructor
        occupied=false;
        timeLeft=0;
        canLand=cl;
    }

    void useRunway() {
        occupied=true;
        timeLeft=2;
    }

    bool isOccupied() {
        return(occupied);
    }

    bool updateRunway() {                           //returns true if runway is still blocked
        timeLeft--;
        if (timeLeft==0) {
            occupied=false;
            return false;    
        }
        return true;
    }
};

class Plane {
    public: 
    tuple <char,char,int,int,int> id;           //identification of the plane
    tuple <char,char,char> apcode;              //which airport the plane comes from/goes to
    int x;                                      //either how much fuel the plane has left or how long the flight will be 
    bool isArrival;                             //if true, plane is arriving, if false, plane is departing
    bool isEmergency;
    int timeWaited;                             //time waited by a departing plane
    int timeWaitedLanding;                      //time waited by a landing plane
    bool changed;

    Plane(int c,int v,float pp, float pe) {
        id=make_tuple('a' + rand()%26, 'a' + rand()%26, rand()%10, rand()%10,rand()%10);
        apcode=make_tuple('a' + rand()%26, 'a' + rand()%26, 'a' + rand()%26);
        isArrival=getArrival(pp);
        isEmergency=getEmergency(pe);
        if(isArrival){
            x=1+rand()%c;                       //if plane is arriving (type==true), see how much fuel left
        }
        else {
            x=1+rand()%v;
        };
        timeWaited=0;
        timeWaitedLanding=0;
        changed=false;
    }
    
    void whatId (){
        cout<< "Flight " <<get<0>(id)<< get<1>(id) << get<2>(id) << get<3>(id) << get<4>(id);
        if(isArrival){
            cout<< " coming from " << get<0>(apcode)<< get<1>(apcode) << get<2>(apcode) << " is waiting to land with " << x << " units of fuel." <<endl;
        }
        else{
            cout<< " going to " << get<0>(apcode)<< get<1>(apcode) << get<2>(apcode) << " is waiting to depart on a " << x << " units of time long trip." <<endl;
        }
    };

    void decreaseFuel(){
        x--;
    };

    void increaseTime(){
        timeWaited++;
    };

    void increaseTimeLanding(){
        timeWaitedLanding++;
    };

    void change() {
        changed=true;
    }

    bool checkArrival(){
        return(isArrival);
    };

    bool checkEmergency(){
        return(isEmergency);
    };
    
    int getTime(){
        return(timeWaited);
    };

    int getTimeLanding(){
        return(timeWaitedLanding);
    };

    int getX(){
        return(x);
    };

    bool hasChanged() {
        return(changed);
    };

    bool isLate() {
        return (timeWaited>(float)x/10);
    }

    bool canLand3() {                                       //returns whether the plane can land in runway 3
        return (!isArrival || isEmergency || x==0 || timeWaited>(float)x/10);
    };
};

class Node {
    public:
    Plane* data;
    Node* next;
    int priority;
    Node (Plane* dt, int pr) {
        data=dt;
        priority=pr;
        next=nullptr;
    };

    int getPriority(){
        return(priority);
    };
};

class PriorityQueue {
    public:
    Node* first;                                    //element with the highest priority
    int n;                                          //number of elements
    int nfuel;                                      //number of elements without fuel
    int nlate;                                      //number of late flights (flights that have waited >10% flight time)
    int nemergency;                                 //number of emergency flights waiting
    int ndepartures;                                //number of departure flights waiting
    float meanTimeLanding;
    float meanTimeDeparting;
    float meanFuelLanding;
    float meanFuelLanded;
    int nlanded;                                    //number of flights landed at every instant
    int ndeparted;                                  //number of flights departed at every instant
    int emergencies;                                //number of flights landed/departed in any emergency
    bool changingPriority;

    PriorityQueue(){                                //constructor
        n=0;
        first=nullptr;
        nfuel=0;
        nlate=0;
        nemergency=0;
        ndepartures=0;
        meanTimeLanding=0;
        meanTimeDeparting=0;
        meanFuelLanding=0;
        meanFuelLanded=0;
        nlanded=0;
        ndeparted=0;
        emergencies=0;
        changingPriority=false;
    };

    void push(Plane* plane, int pr){
        Node* start= first;
        Node* node = new Node(plane, pr);
        
        if (start==nullptr) {
            first=node;
        }
        else if (node->priority > first->priority) {                //if the priority of the node we want to add is greater than the first
            Node* t =first;
            first=node;
            first->next=t;
        }
        else if (node->priority == first->priority) {                 //if it is equal
            while (start->next !=nullptr){
                if (node->priority==start->next->priority){
                    start=start->next;
                }
                else {
                    break;
                }
            }
            Node* t=start->next;
            start->next=node;
            node->next=t; 
        }
        else {                                                       //if it is lesser
            while (node->priority<=start->next->priority && start->next !=nullptr){
                start=start->next;
                if(start->next==nullptr) {
                    break;
                }
            }
            Node* t=start->next;
            start->next=node;
            node->next=t;
        }
        if(!changingPriority){
            n++;
            if (!plane->checkArrival()) {
                ndepartures++;
            }
        }
    }; 

    void pop() {
        Node* t = first;
        first=first->next;
        if (!changingPriority){
            if (!t->data->checkArrival()) {
                ndepartures--;
                ndeparted++;
                meanTimeDeparting+=t->data->getTime();
            }
            else {                           //if landing
                meanFuelLanded += t->data->getX();
                meanFuelLanding-=t->data->getX();
                nlanded++;
                meanTimeLanding+=t->data->getTimeLanding();
            };
            if (t->data->checkEmergency()){
                nemergency--;
                emergencies++;
            }
            if(t->data->getX()==0) {
                nfuel--;
                emergencies++;
            }
            if(t->data->isLate()) {
                nlate--;
                emergencies++;
            }
            n--;
        }
        delete t->data;
        delete t;
    };

    void changePriority(Plane* plane, int priority){
        changingPriority=true;
        Node*  start= first;
        if(first->data==plane){
            pop();
            push(plane,priority);
        }
        else{
            while(start->next->data != plane && start->next !=nullptr) {
                start=start->next;
            }
            Node* t= start->next->next;                        
            delete start->next;
            start->next=t;
            push(plane,priority);
        }
        changingPriority=false;
    };
    

    bool isEmpty(){
        return (n==0);
    };
    int size() {
        return n;
    };
    Plane* peek() {
        return first -> data;
    };
    int getNfuel() {
        return nfuel;
    };
    void increaseNemergency (){
        nemergency++;
    };
    bool canRunway3() {                         //returns if there flights that can use runway 3
        return (nemergency>0||nfuel>0||nlate>0||ndepartures>0);
    }

    void displayPlanes() {
        Node*  start= first;
        while (start != nullptr) {
            start->data->whatId();
            if (start->data->checkArrival()){
                meanFuelLanding+=start->data->getX();
            }
            start=start->next;
        }
    };

    void displayData(int numTransfers) {
        if(nlanded !=0){
            cout<< "Mean waiting time to land: "<<meanTimeLanding/nlanded<<endl;
        }
        else{
            cout<<"Mean waiting time to land: no planes landed"<<endl;
        }
        if(ndeparted !=0){
           cout<< "Mean waiting time to depart: "<<meanTimeDeparting/ndeparted<<endl;
        }
        else{
            cout<<"Mean waiting time to depart: no planes departed"<<endl;
        }
        if(n-ndepartures !=0){
           cout<< "Mean fuel reseve of flights waiting to land: "<< meanFuelLanding/(n-ndepartures)<<endl;
        }
        else{
            cout<<"Mean fuel reseve of flights waiting to land: no planes waiting to land"<<endl;
        }
        if(nlanded !=0){
           cout<< "Mean fuel reserve of landed flights: "<< meanFuelLanded/nlanded <<endl;
        }
        else{
            cout<<"Mean fuel reserve of landed flights: no planes landed"<<endl;
        }
        cout<< "Number of landed/departed flights in any kind of emergency(includes lack of fuel, tardiness and emergency flights): "<< emergencies <<endl;
        if(numTransfers>0) {
            cout<<"Number of transferred flights: "<< numTransfers <<endl;
        }
        else{
            cout<<"Number of transferred flights: 0" <<endl;
        }
        cout<<endl;
    };

    //updates the fuel/waiting time, checks whether there are planes without fuel 
    //or that have waited more than 10% of their flight times
    //planes without fuel - priority 4. planes that have waited for more than 10% of their time: priority 1.
    //emergency flights without fuel-priority 6. 
    //emergency flights that have waited for more than 10% of their time: priority 3.
    void update() {                                 
        Node* start=first;
        while(start !=nullptr){
            Plane* plane=start->data;
            if(plane->checkArrival()) {
                plane->increaseTimeLanding();
                plane->decreaseFuel();
                if(plane->getX() == 0) {
                    changePriority(plane, start->getPriority()+4);
                    nfuel++;
                    if(plane->checkEmergency()){
                        nemergency--;
                    };
                };
            }
            else {
                plane->increaseTime();
                if(plane->getTime()>(float)(plane->getX()/10) && !plane->hasChanged()){
                    changePriority(plane, start->getPriority()+1);
                    plane->change();
                    nlate++;
                    if(plane->checkEmergency()){
                        nemergency--;
                    }
                }
            }
            start=start->next;
        }
        meanFuelLanded=0;
        meanFuelLanding=0;
        meanTimeLanding=0;
        meanTimeDeparting=0;
        nlanded=0;
        ndeparted=0;
        emergencies=0;
    };

    void transfer(int nTransfers) {
        for (int i = 0; i < nTransfers; i++)
        {
            pop();
        }    
    }

    int AssignRunways(Runway* r1, Runway* r2, Runway* r3) {
        int usedRunways=0;
        if (!r3->isOccupied() && canRunway3()) {
            Node* start=first;
            if (first->data->canLand3()) {
                pop();
            }
            else {
                while(start != nullptr){
                    start=start->next;
                    if (first->data->canLand3()){
                        changePriority(first->data,100);            //to the top
                        pop();
                        break;
                    }
                }
            }
            usedRunways++;
            r3->useRunway();
        }
        if(!r1->isOccupied() && !isEmpty()){
            pop();
            r1->useRunway();
            usedRunways++;
        }
        if(!r2->isOccupied() && !isEmpty()){
            pop();
            r2->useRunway();
            usedRunways++;
        }
        return usedRunways;
    }
};

class Airport {
    public:
    Runway* runway1;
    Runway* runway2;
    Runway* runway3;              //only for take-offs,unless there is an emergency
    int n;                        //number of runways available

    Airport() {                         //constructor
        runway1= new Runway(true);
        runway2= new Runway(true);
        runway3= new Runway(false);
        n=3;
    }

    int getNRunways() {
        return n;
    }

    void decreaseNRunways() {
        n--;
    }

    void decreaseNRunwaysby(int k) {
        n=n-k;
    }

    void updateRunways() {
        bool a= runway1->updateRunway();
        bool b =runway2->updateRunway();
        bool c= runway3->updateRunway();
        if(!a) {
            n++;
        };
        if(!b) {
            n++;
        };
        if(!c) {
            n++;
        };
    }
};


void simulate(int t,int k, int c, int v, float pp, float pd, float pe){
    srand(0);
    PriorityQueue prq;                //priority queue
    Airport arpt; 
    int usedRunways=0;
    int numPlanes=0;
    vector<vector<Plane*>> vec1;

    for (int i = 0; i < t; i++)
    {   
        cout<< "Instant " << i<<endl;
        numPlanes = rand() % (k+1);
        cout<<"Number of new planes: "<<numPlanes<<endl;
        vector<Plane*> vec;
        vec1.push_back(vec);

        for (int j = 0; j < numPlanes; j++)         //at every instant, numPlanes planes contact the tower
        {
            Plane *plane = new Plane(c,v,pp,pe);
            vec.push_back(plane);

            if (!plane->checkEmergency()) {               //if the flight is not an emergency, it goes to the normal queue
                prq.push(plane,0);                        //priority is zero for planes that are not emergencies
            }
            else {                                        //if it is, it goes to the priority queue with priority 2
                prq.push(plane,2);
                prq.increaseNemergency();
            }
        };
        
        prq.displayPlanes();

        //we have to check whether there are more planes without fuel than available runaways. 
        //if so, we transfer the exceeding planes to another airport
        int numTransfers=prq.getNfuel() - arpt.getNRunways();
        if (numTransfers>0) {
            prq.transfer(numTransfers);
        }
        
        //then we have to assign planes to the runways and lock them for 2 instants
        usedRunways= prq.AssignRunways(arpt.runway1,arpt.runway2,arpt.runway3);
        arpt.decreaseNRunwaysby(usedRunways);                                //update number of available runways
        prq.displayData(numTransfers);

        //finally, we have to update the planes' fuel/waiting time and
        //check whether there are planes without fuel
        //or planes that have waited more than 10% of their flight time
        prq.update();

        //and we have to update the runways
        arpt.updateRunways();
    }
}   

int main (){
    int t,k,c,v=0;
    float pp, pd, pe=0;
    cin >> t >> k >> pp >> pd >> pe >> c >> v;
    simulate(t,k,c,v,pp,pd,pe);   
}
