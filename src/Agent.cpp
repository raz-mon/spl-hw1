//
// Created by spl211 on 03/11/2020.
//

#include "../include/Agent.h"
#include "../include/Session.h"
#include <iostream>
#include <string>
#include "../include/Graph.h"
#include "../include/Tree.h"

using namespace std;

Agent::Agent(Session& session): session(session){}

Agent::~Agent(){}




ContactTracer::ContactTracer(Session& session):Agent(session) {}

ContactTracer* ContactTracer::clone() const{
    return new ContactTracer(*this);
}

void ContactTracer::act(){
    int dequeue = session.dequeueInfected();
    Tree* root = Tree::createTree(session, dequeue);
    root->BFS(session);
    int toIsolate = root->traceTree();
    session.getGraph().isolate(toIsolate);
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
    int spreaded = g.closestNode(nodeInd);
    if(spreaded != -1){
        Virus *v = new Virus(spreaded, session);
        g.setStatus(spreaded, C);
        session.addAgent(*v);
    }

}


