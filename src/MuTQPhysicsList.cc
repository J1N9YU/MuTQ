#include "MuTQPhysicsList.hh"

MuTQPhysicsList::MuTQPhysicsList() : G4VModularPhysicsList() {
    //G4SpinDecayPhysics depends on G4DecayPhysics.
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4SpinDecayPhysics());
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4EmPenelopePhysics());
    RegisterPhysics(new G4StepLimiterPhysics());
}

MuTQPhysicsList::~MuTQPhysicsList() {}

void MuTQPhysicsList::SetCuts() {
    G4VUserPhysicsList::SetCuts();
}

