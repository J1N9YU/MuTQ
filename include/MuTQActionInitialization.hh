#ifndef MuTQ_ACTION_INITIALIZATION_H
#define MuTQ_ACTION_INITIALIZATION_H 1

#include "G4VUserActionInitialization.hh"

class MuTQActionInitialization : public G4VUserActionInitialization {
public:
    MuTQActionInitialization();
    virtual ~MuTQActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif


