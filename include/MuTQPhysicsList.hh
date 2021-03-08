#ifndef MuTQ_PHYSICS_LIST_H
#define MuTQ_PHYSICS_LIST_H 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class MuTQPhysicsList : public G4VModularPhysicsList {
public:
    MuTQPhysicsList();
    virtual ~MuTQPhysicsList();
    virtual void SetCuts();
};

#endif

