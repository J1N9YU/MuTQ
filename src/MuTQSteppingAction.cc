#include "MuTQSteppingAction.hh"

#include "G4Step.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "MuTQConfigs.hh"
#include "MuTQDetectorConstruction.hh"

MuTQSteppingAction::MuTQSteppingAction() :
    G4UserSteppingAction() {}

MuTQSteppingAction::~MuTQSteppingAction() {}

void MuTQSteppingAction::UserSteppingAction(const G4Step* step) {
    if (
#if MuTQ_KILL_ELECTRON
        step->GetTrack()->GetParticleDefinition() == G4Electron::Definition() ||
#endif
#if MuTQ_KILL_GAMMA
        step->GetTrack()->GetParticleDefinition() == G4Gamma::Definition() ||
#endif
        step->GetPreStepPoint()->GetPosition().z() < MuTQDetectorConstruction::fTunnelEntranceAltitude
        ) {
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
}

