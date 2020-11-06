//
// Created by spl211 on 03/11/2020.
//

#include "../include/Agent.h"
#include "../include/Session.h"
#include <iostream>
#include <string>
#include "../include/Graph.h"

using namespace std;

Agent::Agent(Session& session): session(session){}


ContactTracer::ContactTracer(Session& session):Agent(session) {}

void ContactTracer::act(){}


Virus::Virus(int nodeInd, Session& session):Agent(session), nodeInd(nodeInd){}


void Virus::act(){
    Graph& g = session.getGraph();
    if(!g.isInfected(nodeInd)){
        g.infectNode(nodeInd);
        session.enqueueInfected(nodeInd);
    }
    int spreaded = g.closestNode(nodeInd);
    if(spreaded != -1){
        Virus *v = new Virus(spreaded, session);
        session.addAgent(*v);
    }

}
