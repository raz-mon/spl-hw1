//
// Created by spl211 on 03/11/2020.
//

#include "../include/Tree.h"
#include <string>
#include "../include/Agent.h"
#include "../include/Graph.h"
#include <vector>

using namespace std;

Tree::Tree(int rootLabel): node(rootLabel), children(vector<Tree*>()){}

//Implementation of: Rule of 3
//Destructor
Tree::~Tree(){
    for(uint i=0; i<children.size(); ++i){
        delete(children[i]);
    }
}

//Copy Constractor
Tree::Tree(const Tree &other):node(other.node), children(){
    for(uint i = 0; i < other.children.size(); ++i){
        this->children.push_back(other.children[i]->clone());           // Atention!!!! We have new memory to take care of.
    }
}

//Copy Assignment Operator
Tree & Tree::operator=(const Tree &other){          // Atention!!!! We have new memory to take care of.
    this->node = other.node;
    while (!children.empty()){
        delete (children[0]);
        children.erase(children.cbegin());
    }
    for (uint i = 0; i < other.children.size(); ++i){
        this->children.push_back(other.children[i]->clone());
    }
    return *this;
}

void Tree::addChild(const Tree& child){
    Tree *newChild = child.clone();     //In clone we declare "new" Tree on the Heap, and Delete the prior (child). (Maybe).
    children.push_back(newChild);
    this->organize();
}

Tree* Tree::createTree(const Session& session, int rootLabel) {
    TreeType tt = session.getTreeType();
    if (tt == Cycle) {
        CycleTree *CT = new CycleTree(rootLabel, currCycle);        //Need to make cuurCycle field in Session.
        return CT;
    } else if (tt == MaxRank) {
        MaxRankTree *MRT = new MaxRankTree(rootLabel);
        return MRT;
    } else {
        RootTree *RT = new RootTree(rootLabel);
        return RT;
    }
}

void Tree::organize(){
    if (children.size()==1){
        return;
    }
    int i = children.size()-2;
    while((children[i]->node)>(children[i+1]->node)){
        Tree *Temp = children[i]->clone();          // Beware of memory leaks!
        children[i] = children[i+1];
        children[i+1] = Temp;
        --i;
    }
}






MaxRankTree::MaxRankTree(int rootLabel): Tree(rootLabel){}

int MaxRankTree::traceTree(){}

MaxRankTree* MaxRankTree::clone() const {
    MaxRankTree *MR = new MaxRankTree(*this);
    return MR;
}

RootTree::RootTree(int rootLabel):Tree(rootLabel){}

int RootTree::traceTree(){}

RootTree* RootTree::clone() const {
    RootTree *RT = new RootTree(*this);
    return RT;
}


CycleTree::CycleTree(int rootLabel, int currCycle): Tree(rootLabel), currCycle(currCycle){}

int CycleTree::traceTree(){}

CycleTree* CycleTree::clone() const {
    CycleTree *CT = new CycleTree(*this);
    return CT;
}









