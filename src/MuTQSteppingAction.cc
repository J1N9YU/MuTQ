#include "MuTQSteppingAction.hh"

#include "G4Step.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "MuTQConfigs.hh"

MuTQSteppingAction::MuTQSteppingAction() :
    G4UserSteppingAction() {}

MuTQSteppingAction::~MuTQSteppingAction() {}

void MuTQSteppingAction::UserSteppingAction(const G4Step* step) {
    if (step->GetPreStepPoint()->GetPosition().z() < 0) {
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
#if MuTQ_KILL_GAMMA
    if (step->GetTrack()->GetParticleDefinition() == G4Gamma::Definition()) {
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
#endif
#if MuTQ_KILL_ELECTRON
    if (step->GetTrack()->GetParticleDefinition() == G4Electron::Definition()) {
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
#endif
}

