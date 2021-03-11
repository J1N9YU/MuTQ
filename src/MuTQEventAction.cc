#include "MuTQEventAction.hh"
#include "MuTQProgressMonitor.hh"

MuTQEventAction::MuTQEventAction() :
    G4UserEventAction() {}

MuTQEventAction::~MuTQEventAction() {}

void MuTQEventAction::BeginOfEventAction(const G4Event*) {}

void MuTQEventAction::EndOfEventAction(const G4Event*) {
    MuTQProgressMonitor::Instance().EventComplete();
}

