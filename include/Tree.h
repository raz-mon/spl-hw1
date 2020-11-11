#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <string>
#include "../include/Agent.h"
#include "../include/Graph.h"

class Session;

class Tree{
public:
    Tree(int rootLabel);
    void addChild(const Tree& child);
    //Rule of 5:
    virtual ~Tree();
    Tree(const Tree &other);
    Tree(Tree && other);
    Tree & operator=(const Tree &other);
    Tree & operator=(Tree && other);

    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;

    //from here, our implementation:
    virtual Tree* clone() const=0;
    void organize();

    vector<int>* getNeighbors(int Node,Session& session);


    //This method should be arased.
    void print(Session& session, int rootind);

    void BFS(Session& session);
    int getNode() const;
    vector<Tree*>& getChildren();

private:
    int node;
    std::vector<Tree*> children;

    void clear();
    void copyChildren(const Tree &other);
};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);            // currCycle: probably a field we will need to maintain.
    virtual int traceTree();

    virtual CycleTree* clone() const;
private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual int traceTree();

    virtual MaxRankTree* clone() const;
private:
    int MaxRank();
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();

    virtual RootTree* clone() const;
};








#endif
