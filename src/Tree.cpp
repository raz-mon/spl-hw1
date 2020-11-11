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
        children[0]= nullptr;
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
    Tree *newChild = child.clone();     //In clone we declare "new" Tree on the Heap, and Delete the prior (child). This will take place in BFS!
    children.push_back(newChild);
    this->organize();
}

Tree* Tree::createTree(const Session& session, int rootLabel) {     // Here too. A pointer is returned!!!
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

vector<int>* Tree::getNeighbors(int Node,Session& session){
    Graph& g = session.getGraph();
    vector<int> *ret = new vector<int>();
    for (int i = 0; i < g.getSize(); ++i){
        if(g.hasEdge(Node,i))
            ret->push_back(i);
    }
    return ret;
}

//This method is made for testing. Delete when done.
vector<Tree*>& Tree::getChildren(){
    return this->children;
}





MaxRankTree::MaxRankTree(int rootLabel): Tree(rootLabel){}

int MaxRankTree::traceTree(){
    return MaxRank();
}

int MaxRankTree::MaxRank() {
    queue<Tree*> Queue;
    Queue.push(this);
    int index = Queue.front()->getNode();
    int childrenSize = Queue.front()->getChildren().size();
    int curr_index;
    int curr_children;
    while (!Queue.empty()){
        Tree* & curr = Queue.front();
        Queue.pop();
        curr_index = curr->getNode();
        curr_children = curr->getChildren().size();
        if (curr_children > childrenSize){
            index = curr_index;
            childrenSize = curr_children;
        }
        for (int i = 0; i < curr_children ; ++i) {
            Queue.push(curr->getChildren()[i]);
        }
    }
    return index;
}

MaxRankTree* MaxRankTree::clone() const {
    MaxRankTree *MR = new MaxRankTree(*this);
    return MR;
}

RootTree::RootTree(int rootLabel):Tree(rootLabel){}

int RootTree::traceTree(){
    return this->getNode();
}

RootTree* RootTree::clone() const {     //We return a pointer, i.e move on responsibility for the Tree.
    RootTree *RT = new RootTree(*this);
    return RT;
}


CycleTree::CycleTree(int rootLabel, int currCycle): Tree(rootLabel), currCycle(currCycle){}

int CycleTree::traceTree(){
    cout << "Entre traceTree: " << endl;
    int c = this->getNode();
    Tree *tree = this;

    int check = 0;  //Delete this!!
    cout << "currCycle Number: " << currCycle << endl;
    for (int i = 0; i < this->currCycle & check < 100; ++i) {     // Question: maybe currCycle -> (currCycle-1).
        if(tree->getChildren().size() != 0) {
            tree = (tree->getChildren()[0]);
            c = tree->getNode();
        }
        cout << "traceTree Cycle Number: " << check << endl;
        ++check;
    }
    return c;
}

CycleTree* CycleTree::clone() const {
    CycleTree *CT = new CycleTree(*this);
    return CT;
}

void Tree::BFS(Session& session){
    vector<int> visited;
    visited.assign(session.getGraph().getSize(),0);
    queue<Tree*> TreeQueue;
    Tree &root = *this;
    TreeQueue.push(&root);
    while (!TreeQueue.empty()){
        Tree* &currTree = TreeQueue.front();
        TreeQueue.pop();
        if (visited[currTree->node] != 2){
            vector<int> *neighbors = (*currTree).getNeighbors(currTree->node, session);
            for (uint i = 0; i < neighbors->size() ; ++i) {
                if(visited[(*neighbors)[i]] == 0){
                    Tree *temp = createTree(session,(*neighbors)[i]);
                    currTree->addChild(*temp);
                    TreeQueue.push(currTree->getChildren()[currTree->getChildren().size()-1]);
                    visited[(*neighbors)[i]] = 1;
                }
            }
            visited[currTree->node] = 2;
/*
            cout << "node " << currTree->node << "'s children are: ";
            for (uint i=0; i<currTree->children.size(); ++i){
                cout << currTree->children[i]->node << ", ";
            }
            cout << endl;
*/
        }
    }
}

int Tree::getNode() const{return this->node;}

















