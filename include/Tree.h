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
    //Rule of 3:
    virtual ~Tree();
    Tree(const Tree &other);
    Tree & operator=(const Tree &other);

    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;

    //from here, our implementation:
    virtual Tree* clone() const=0;
    void organize();
    void BFS(int RootInd, Session session);

private:
    int node;
    std::vector<Tree*> children;
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
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();

    virtual RootTree* clone() const;
};

#endif
