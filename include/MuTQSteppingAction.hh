#ifndef MuTQ_STEPPING_ACTION_H
#define MuTQ_STEPPING_ACTION_H 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class MuTQSteppingAction : public G4UserSteppingAction {
public:
    MuTQSteppingAction();
    virtual ~MuTQSteppingAction();

    virtual void UserSteppingAction(const G4Step*);
};

#endif
