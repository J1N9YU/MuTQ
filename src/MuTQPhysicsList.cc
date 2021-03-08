#include "MuTQPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4SpinDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"

#include "MuTQConfigs.hh"

MuTQPhysicsList::MuTQPhysicsList() : G4VModularPhysicsList() {
    //G4SpinDecayPhysics depends on G4DecayPhysics.
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4SpinDecayPhysics());
    RegisterPhysics(new G4EmStandardPhysics());
}

MuTQPhysicsList::~MuTQPhysicsList() {}

void MuTQPhysicsList::SetCuts() {
    G4VUserPhysicsList::SetCuts();
}

