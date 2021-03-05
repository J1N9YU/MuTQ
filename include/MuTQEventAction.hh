#ifndef MuTQ_EVENT_ACTION_H
#define MuTQ_EVENT_ACTION_H 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "MuTQRunAction.hh"

class MuTQEventAction : public G4UserEventAction {
public:
    MuTQEventAction();
    virtual ~MuTQEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
};

#endif


