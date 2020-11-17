//
// Created by spl211 on 03/11/2020.
//

#include "../include/Agent.h"
#include <string>
#include "../include/Tree.h"

using namespace std;
//CTR
Agent::Agent(Session& session): session(session){}
//Destructor
Agent::~Agent(){}

ContactTracer::ContactTracer(Session& session):Agent(session) {}

ContactTracer* ContactTracer::clone() const{
    return new ContactTracer(*this);        // Return a new instance with the same attributes as (*this) (calls copy CTR).
}

void ContactTracer::act(){
    int dequeue = session.dequeueInfected();
    if(dequeue == -1)       // queue is empty.
        return;
    Tree* root = Tree::createTree(session, dequeue);
    root->BFS(session);     // Create the relevant BFS tree, where root is its root.
    int toIsolate = root->traceTree();      // Put in toisolate the node that we want to isolate.
    session.getGraph().isolate(toIsolate);      // Isolate the relevant node.
    delete(root);                           // Free alocated memory by root (no need for him any more).
    root = nullptr;                         // So we won't reffer to an address that has garbage inside it.
}






Virus::Virus(int nodeInd, Session& session):Agent(session), nodeInd(nodeInd){}

Virus* Virus::clone() const{
    return new Virus(*this);
}

void Virus::act(){
    Graph& g = session.getGraph();
    if(!g.isInfected(nodeInd)){
        g.infectNode(nodeInd);
        session.enqueueInfected(nodeInd);
    }
    int spreaded = g.closestNode(nodeInd);      // spreaded will hold the smallest indexed healthy node, or (-1) if none exists.
    if(spreaded != -1){
        Virus *v = new Virus(spreaded, session);
        g.setStatus(spreaded, C);           // Set the node status to 'carrying'.
        session.addAgent(*v);                   // Create a new agent with the new virus, and add it to agents (vector).
        delete(v);                          // Free alocated memory by v.
    }

}


