#include "MuTQActionInitialization.hh"
#include "MuTQPrimaryGeneratorAction.hh"
#include "MuTQRunAction.hh"
#include "MuTQEventAction.hh"
#include "MuTQSteppingAction.hh"

MuTQActionInitialization::MuTQActionInitialization() :
    G4VUserActionInitialization() {}

MuTQActionInitialization::~MuTQActionInitialization() {}

void MuTQActionInitialization::BuildForMaster() const {
    SetUserAction(new MuTQRunAction());
}

void MuTQActionInitialization::Build() const {
    SetUserAction(new MuTQPrimaryGeneratorAction());
    SetUserAction(new MuTQRunAction());
    SetUserAction(new MuTQEventAction());
    SetUserAction(new MuTQSteppingAction());
}

