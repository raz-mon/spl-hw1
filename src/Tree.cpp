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

int MaxRankTree::traceTree(){

}

RootTree::RootTree(int rootLabel):Tree(rootLabel){}

int RootTree::traceTree(){

}


CycleTree::CycleTree(int rootLabel, int currCycle): Tree(rootLabel), currCycle(currCycle){}

int CycleTree::traceTree(){

}







