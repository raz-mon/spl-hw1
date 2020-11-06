//
// Created by spl211 on 03/11/2020.
//

#include "../include/Graph.h"
#include <string>
#include <iostream>

using namespace std;

Graph::Graph():edges(), nodesStatus(){}

Graph::Graph(std::vector<std::vector<int>> matrix): edges(matrix), nodesStatus(){
    for(uint i=0;i<edges.size();++i){
        nodesStatus.push_back(I);
    }
}

// From here on, we built.

Status Graph::getStatus(int node){
    return nodesStatus[node];
}


bool Graph::finish(){

/*
    //This part is ONLY for TESTING finish()!
    vector<vector<int>> temp{{0,0,0,1,1,0},{0,0,0,0,1,0},{0,0,0,0,0,1},{1,0,0,0,0,0},{1,1,0,0,0,0},{0,0,1,0,0,0}};
    bool ans = true;
    vector<Status> temp2{H,I,C,H,I,C};
    for(uint i=0;i<temp.size();++i){
        Status currNodeStatus = temp2[i];
        for(uint j=0;(j<temp.size()) & (ans);++j){
            if ((i!=j) & (temp[i][j]==1) & (currNodeStatus!=temp2[j])){
                ans = false;
                cout << "nodes " << i << " and " << j << " are of different states!" << endl;
            }
        }
    }
*/

//          This is the actual function I want to KEEP! The rest are tests.
    bool ans = true;
    for(uint i=0;i<edges.size();++i){
        Status currNodeStatus = nodesStatus[i];
        for(uint j=0;(j<edges.size()) & (ans);++j){
            if ((i!=j) & (edges[i][j]==1) & (currNodeStatus!=nodesStatus[j])){
                ans = false;
                cout << "nodes " << i << " and " << j << " are of different states!" << endl;
            }
        }
    }
    return ans;


/*    for (uint i=0; i<nodesStatus.size(); ++i){
        cout << nodesStatus[i] << ", ";
    }
    cout << endl;
*/
}





