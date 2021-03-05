#include "G4Step.hh"

#include "MuTQSteppingAction.hh"

MuTQSteppingAction::MuTQSteppingAction() :
    G4UserSteppingAction() {}

MuTQSteppingAction::~MuTQSteppingAction() {}

void MuTQSteppingAction::UserSteppingAction(const G4Step* step) {
    if (step->GetPreStepPoint()->GetPosition().z() < 0) {
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
}

