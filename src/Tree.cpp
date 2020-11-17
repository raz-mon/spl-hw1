//
// Created by spl211 on 03/11/2020.
//

#include "../include/Tree.h"
#include <string>
#include <vector>
#include <queue>

using namespace std;

Tree::Tree(int rootLabel): node(rootLabel), children(vector<Tree*>()){}

//Implementation of: Rule of 5
//Destructor
Tree::~Tree(){
   clear();
}
//Copy Constructor
Tree::Tree(const Tree &other):node(other.node), children(){
    copyChildren(other);
}
//Move Constractor
Tree::Tree(Tree &&other):node(other.node), children(other.children){
    for (uint i=0; i<other.children.size(); ++i){       // after "shallow copy", send other's pointers to null.
        other.children[i] = nullptr;
    }
}
//Copy Assignment Operator
Tree & Tree::operator=(const Tree &other){
    if(this != &other){
        clear();                // delete memory allocated by this->children.
        this->node = other.node;
        copyChildren(other);    // copy other's children.
    }
    return *this;
}
//Move Assignment Operator
Tree& Tree::operator=(Tree &&other) {
    if (this == &other)
        return *this;
    clear();            // delete allocated memory by this.children.
    this->node = other.node;
    for (uint i=0; i<other.children.size(); ++i){       // assigns other.children to this.children, and others children pointers to nullptr.
        this->children.push_back(other.children[i]);
        other.children[i] = nullptr;
    }
    return *this;
}

void Tree::clear() {
    while (!children.empty()){
        delete(children[0]);
        children.erase(children.cbegin());
    }
    node = -1;          // so we know this Tree is cleared.
}

//Creating clones of other children and insert them to this children
void Tree::copyChildren(const Tree &other) {
    for (uint i = 0; i < other.children.size(); ++i){
        this->children.push_back(other.children[i]->clone());
    }
}

//Insert a new child(Tree) to the children vector
void Tree::addChild(const Tree& child){
    Tree *newChild = child.clone();     //In clone we declare "new" Tree on the Heap, and Delete the prior (child). This will take place in BFS!
    children.push_back(newChild);
    this->organize();
}

/**
 * Creating new tree with reference to the tree type
 * @param session
 * @param rootLabel
 * @return MaxrankTree/CycleTree/RootTree tree
 */
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

//Sorting tree's children by node's index
void Tree::organize(){
    if (children.size() < 2){
        return;
    }
    int i = children.size() - 2;
    while((children[i]->node) > (children[i+1]->node)){
        Tree *temp = children[i];
        children[i] = children[i+1];
        children[i+1] = temp;
        --i;
    }
}

/**
 * Check which vertices have edge with this vertx and add them to a vector
 * @param Node
 * @param session
 * @return Node's neighbors
 */
vector<int>* Tree::getNeighbors(int Node,Session& session){
    Graph g = session.getGraph();
    vector<int> *ret = new vector<int>();
    for (int i = 0; i < g.getSize(); ++i){
        if(g.hasEdge(Node,i))
            ret->push_back(i);
    }
    return ret;
}

//Return tree's children
vector<Tree*>& Tree::getChildren(){
    return this->children;
}

MaxRankTree::MaxRankTree(int rootLabel): Tree(rootLabel){}

int MaxRankTree::traceTree(){
    return MaxRank();
}

/**
 * BFS algorithm to find a node with the most edges
 * @return node's index for act() method
 */
int MaxRankTree::MaxRank() {
    queue<Tree*> Queue;
    Queue.push(this);
    int index = Queue.front()->getNode();                       //initializing node's index to be return
    int childrenSize = Queue.front()->getChildren().size();     //initializing node's children size for equalization
    int curr_index;
    int curr_children;
    while (!Queue.empty()){                 //The same termination conditions like BFS
        Tree* & curr = Queue.front();
        Queue.pop();
        curr_index = curr->getNode();                   //Updating current index to be the same as current tree
        curr_children = curr->getChildren().size();     //Updating current children to be the same as current tree
        if (curr_children > childrenSize){              //check for node with more children and update correspondingly
            index = curr_index;
            childrenSize = curr_children;
        }
        for (int i = 0; i < curr_children ; ++i) {      //push tree's children into queue
            Queue.push(curr->getChildren()[i]);
        }
    }
    return index;
}

//Creating clone for MaxRankTree
MaxRankTree* MaxRankTree::clone() const {
    MaxRankTree *MR = new MaxRankTree(*this);
    return MR;
}

RootTree::RootTree(int rootLabel):Tree(rootLabel){}

int RootTree::traceTree(){
    return this->getNode();
}
//Creating clone for RootTree
RootTree* RootTree::clone() const {     //We return a pointer, meaning we move on responsibility for the Tree.
    RootTree *RT = new RootTree(*this);
    return RT;
}

CycleTree::CycleTree(int rootLabel, int currCycle): Tree(rootLabel), currCycle(currCycle){}

int CycleTree::traceTree(){
    int c = this->getNode();
    Tree *tree = this;
    for (int i = 0; i < this->currCycle; ++i) {
        if(tree->getChildren().size() != 0) {       //while tree still have a child proceed
            tree = (tree->getChildren()[0]);
            c = tree->getNode();
        }
    }
    return c;
}
//Creating clone for CycleTree
CycleTree* CycleTree::clone() const {
    CycleTree *CT = new CycleTree(*this);
    return CT;
}

/**
 * BFS algorithm - Building a tree from  tree's node
 * @param session
 */
void Tree::BFS(Session& session){
    vector<int> visited;            //keep the visited nodes
    visited.assign(session.getGraph().getSize(),0);  //initializing visited with zeros
    queue<Tree*> TreeQueue;
    Tree &root = *this;
    TreeQueue.push(&root);
    while (!TreeQueue.empty()){         //Termination conditions for BFS
        Tree* &currTree = TreeQueue.front();
        TreeQueue.pop();
        if (visited[currTree->node] != 2){      //Check if we associated already this node to other nodes
            vector<int> *neighbors = (*currTree).getNeighbors(currTree->node, session);
            for (uint i = 0; i < neighbors->size() ; ++i) {
                if(visited[(*neighbors)[i]] == 0){      // if this is a node we didn't encountered - proceed
                    Tree *temp = createTree(session,(*neighbors)[i]);
                    currTree->addChild(*temp);      //Add child to current tree - only if it came later in queue
                    TreeQueue.push(currTree->getChildren()[currTree->getChildren().size()-1]);
                    visited[(*neighbors)[i]] = 1;       //Node in queue
                    delete(temp);
                }
            }
            visited[currTree->node] = 2;        //Marking node as visited one
            delete(neighbors);
        }
    }
}

int Tree::getNode() const{return this->node;}

















