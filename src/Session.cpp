//
// Created by spl211 on 03/11/2020.
//

#include "../include/Session.h"
#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include "../include/Agent.h"
#include <string>


using json = nlohmann::json;
using namespace std;

Session::Session():g(),treeType(Root), agents(){}

Session::Session(const std::string& path):g(),treeType(),agents(){
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
        }
        else {
            ContactTracer *ct = new ContactTracer(*this);
            agents.push_back(ct);
        }
    }
}

void Session::simulate(){
    while (!g.finish()){
        //Indicating vector size
        int currentSize = agents.size();
        for(int i = 0; i < currentSize; i++){
            agents[i]->act();
        }
    }
}




