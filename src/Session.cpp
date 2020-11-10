//
// Created by spl211 on 03/11/2020.
//

#include "../include/Session.h"
#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include "../include/Agent.h"
#include <string>
#include <vector>
#include "../include/Tree.h"    //We need to delete this

using json = nlohmann::json;
using namespace std;

Session::Session():g(),treeType(Root), agents(),InfectedQueue(),cycle(){}

Session::Session(const std::string& path):g(),treeType(),agents(),InfectedQueue(),cycle(0){
    ifstream i(path);
    json j;
    i >> j;
    //Initializing treeType
    if (j["tree"] == "M"){
        this->treeType  = MaxRank;
    }
    else if (j["tree"] == "C"){
        this->treeType  = Cycle;
    }
    else{
        treeType  = Root;
    }

    //Initializing Graph
    vector<vector<int>> a;
    for(uint k = 0; k < j["graph"].size(); ++k){
        vector<int> temp;
        for(uint l = 0; l<j["graph"][k].size(); ++l){
            temp.push_back(j["graph"][k][l]);
        }
        a.push_back(temp);
    }

    g = Graph(a);

    //Initializing Agents
    for(uint m=0; m<j["agents"].size(); ++m){
        if(j["agents"][m][0] == "V"){
            Virus *v = new Virus(j["agents"][m][1], *this);
            agents.push_back(v);
            g.setStatus(j["agents"][m][1], C);
        }
        else {
            ContactTracer *ct = new ContactTracer(*this);
            agents.push_back(ct);
        }
    }
}

Session::Session(const Session &otherSess): g(otherSess.g),
        treeType(otherSess.treeType), agents(), InfectedQueue(otherSess.InfectedQueue),cycle(0){
    for (uint i=0; i<agents.size(); ++i){
        this->agents.push_back(otherSess.agents[i]->clone());           // Atention!!!! We have new memory to take care of.
    }
}

Session & Session::operator=(const Session &otherSess){          // Atention!!!! We have new memory to take care of.
    this->g = otherSess.g;
    this->treeType = otherSess.treeType;
    this->InfectedQueue = otherSess.InfectedQueue;
    this->cycle = otherSess.cycle;
    while (agents.size()!=0){
        delete(agents[0]);
        agents.erase(agents.begin());
    }
    for (uint i=0; i<otherSess.agents.size(); ++i){
        this->agents.push_back(otherSess.agents[i]->clone());
    }
    return *this;
}

Session::~Session(){
    for (uint i=0; i<agents.size(); ++i){
        delete(agents[i]);
    }
}
//kkkk
//move assignment operator:
kkk

void Session::simulate(){
    while (!g.finish()){
        int currentSize = agents.size();
        for(int i = 0; i < currentSize; i++){       // run only through agents that where created prior to this cycle.
            agents[i]->act();
        }
        ++cycle;
    }
}

TreeType Session::getTreeType() const{
    return this->treeType;
}


Graph& Session::getGraph(){// QUESTION!!!! do we need to do this stupid proccess of making a reference and returning it? The func returns a reference by definition.
    Graph& temp = g;
    return temp;
}

/*
 * enqueueInfected
 */
void Session::enqueueInfected(int node){
    this->InfectedQueue.push_back(node);
}

int Session::dequeueInfected(){
    if(InfectedQueue.empty())       //Queue is empty
        return -1;
    int infectedNode = InfectedQueue.front();            //Save the first element
    InfectedQueue.erase(InfectedQueue.cbegin()); //Erase the first element
    return infectedNode;
}

/*
 * addAgent             //We need to check this shit
 */
void Session::addAgent(const Agent& agent){
    agents.push_back(agent.clone());
}

int Session::getCycle() const{
    return this->cycle;
}

void Session::outputConfig(){
    json j;
    ofstream o("./output.json");
    j["graph"];
    j["infected"];
    for (int i=0; i<g.getSize(); ++i){
        j["graph"][i] = g.getEdges()[i];
    }
    vector<int> infectedGuys;
    for(uint i=0; i<g.getNodesStatus().size(); ++i){
        if (g.getNodesStatus()[i]!=H){
            infectedGuys.push_back(i);
        }
    }
    j["infected"] = infectedGuys;
    o << j << endl;
}







