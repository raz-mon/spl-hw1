//
// Created by spl211 on 03/11/2020.
//

#include "../include/Tree.h"
#include <string>
#include "../include/Agent.h"
#include "../include/Graph.h"
#include <vector>
#include <iostream>
#include "../include/Session.h"
#include <queue>

using namespace std;

Tree::Tree(int rootLabel): node(rootLabel), children(vector<Tree*>()){}

//Implementation of: Rule of 3
//Destructor
Tree::~Tree(){
  //  for(uint i = 0; i < children.size(); ++i){
   //     delete(children[i]);
   // }
   clear();
    cout << "!!!!!!!Destructor!!!!!!!" << endl;
}

//Copy Constructor
Tree::Tree(const Tree &other):node(other.node), children(){
    copyChildren(other);
    cout << "!!!!!!!Copy Constructor!!!!!!!" << endl;
}

//Move Constractor
Tree::Tree(Tree &&other):node(other.node), children(other.children) {}

//Copy Assignment Operator
Tree & Tree::operator=(const Tree &other){          // Attention!!!! We have new memory to take care of.
    if(this != &other){
        this->node = other.node;
        copyChildren(other);
    }
    cout << "!!!!!!!Copy Assignment Operator!!!!!!!" << endl;
    return *this;                   //Why endless loop?
}

//Move Assignment Operator
Tree& Tree::operator=(Tree &&other) {
    if(this != &other){
        this->node = other.node;
        clear();
        copyChildren(other);
    }
    cout << "!!!!!!!Move Assignment Operator!!!!!!!" << endl;
    return *this;
}

void Tree::clear() {
    while (!children.empty()){
        delete (children[0]);
        //children[0]= nullptr;
        children.erase(children.cbegin());
    }
    cout << "!!!!!!!CLEAR!!!!!!!" << endl;
}

void Tree::copyChildren(const Tree &other) {
    for (uint i = 0; i < other.children.size(); ++i){
        this->children.push_back(other.children[i]->clone());
    }
}

void Tree::addChild(const Tree& child){
    //cout << "Children Size 1: " << this->children.size() << endl;
    Tree *newChild = child.clone();     //In clone we declare "new" Tree on the Heap, and Delete the prior (child). This will take place in BFS!
    children.push_back(newChild);
    //cout << "############" << endl;
    //for (int i = 0; i < children.size(); ++i) {
   //     cout << children[i]->node << endl;
    //}
    this->organize();
    //cout << "Children Size 2: " << this->children.size() << endl;
}

Tree* Tree::createTree(const Session& session, int rootLabel) {
    TreeType tt = session.getTreeType();
    if (tt == Cycle) {
        CycleTree *CT = new CycleTree(rootLabel, session.getCycle());        //Need to make currCycle field in Session.
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
    if (children.size() < 2){
        return;
    }
    int i = children.size() - 2;
    while((children[i]->node) > (children[i+1]->node)){
        Tree *Temp = children[i]->clone();          // Beware of memory leaks!
        children[i] = children[i+1];
        children[i+1] = Temp;
        --i;
    }
}

Tree& Tree::BFS(int RootInd,Session& session){
    Tree* root = createTree(session, RootInd);
    vector<int> visited;
    visited.assign(session.getGraph().getSize(),0);
    //vector<Tree*> TreeQueue;
    queue<Tree*> RealTreeQueue;
    //TreeQueue.push_back(root);
    RealTreeQueue.push(root);
    while (!RealTreeQueue.empty()){
        Tree* &curr_Tree = RealTreeQueue.front();
        RealTreeQueue.pop();
        //TreeQueue.erase(TreeQueue.cbegin());
        if (visited[curr_Tree->node] != 2){
            vector<int> *neighbors = (*curr_Tree).getNeighbors(curr_Tree->node, session);
            for (uint i = 0; i < neighbors->size() ; ++i) {
                if(visited[(*neighbors)[i]] == 0){
                    Tree *temp = createTree(session,(*neighbors)[i]);
                    curr_Tree->addChild(*temp);
                    RealTreeQueue.push(temp);
                    visited[(*neighbors)[i]] = 1;
                }
            }
            visited[curr_Tree->node] = 2;
            //delete(neighbors);
        }
    }
    return (*root);
}

vector<int>* Tree::getNeighbors(int Node,Session& session){
    Graph& g = session.getGraph();
    vector<int> *ret = new vector<int>();
    for (int i = 0; i < g.getSize(); ++i){
        if(g.hasEdge(Node,i))
            ret->push_back(i);
    }
    return ret;
}




MaxRankTree::MaxRankTree(int rootLabel): Tree(rootLabel){}

int MaxRankTree::traceTree(){
    return 0;
}

MaxRankTree* MaxRankTree::clone() const {
    MaxRankTree *MR = new MaxRankTree(*this);
    return MR;
}

RootTree::RootTree(int rootLabel):Tree(rootLabel){}

int RootTree::traceTree(){
    return 0;
}

RootTree* RootTree::clone() const {
    RootTree *RT = new RootTree(*this);
    return RT;
}


CycleTree::CycleTree(int rootLabel, int currCycle): Tree(rootLabel), currCycle(currCycle){}

int CycleTree::traceTree(){
    return 0;
}

CycleTree* CycleTree::clone() const {
    CycleTree *CT = new CycleTree(*this);
    return CT;
}

void Tree::print(Session& session, int rootind){
    Tree *bfs = createTree(session, rootind);
    *bfs = BFS(rootind, session);
    cout << "print" << endl;
    cout << bfs->node << endl;
    cout << "children size:" << (*bfs).children.size() <<endl;
    for (uint i=0; i<(*bfs).children.size(); ++i){
        cout << (*bfs).children[i]->node << "," ;
    }
    cout << endl;
}







