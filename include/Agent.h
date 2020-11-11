#ifndef AGENT_H_
#define AGENT_H_

#include <vector>
#include "Session.h"
#include <string>

using namespace std;

class Agent{
public:
    Agent(Session& session);

    virtual void act()=0;

    virtual Agent* clone() const=0;

    virtual ~Agent();

protected:
    Session& session;
};



class ContactTracer: public Agent{
public:
    ContactTracer(Session& session);
/*
    //Rule of 3:
    ~ContactTracer();
    ContactTracer(const ContactTracer& other);
    ContactTracer& operator=(const ContactTracer& other);
*/

    virtual void act();
    virtual ContactTracer* clone() const;
};




class Virus: public Agent{
public:
    Virus(int nodeInd, Session& session);
    
    virtual void act();
    virtual Virus* clone() const;
/*
    //Rule of 3:
    ~Virus();
    Virus(const Virus& other);
    Virus& operator=(const Virus& other);
*/

private:
    const int nodeInd;
};

#endif
