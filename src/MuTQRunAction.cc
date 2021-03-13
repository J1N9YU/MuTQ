#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "MuTQRunAction.hh"
#include "MuTQPrimaryGeneratorAction.hh"
#include "MuTQDetectorConstruction.hh"
#include "MuTQConfigs.hh"

MuTQRunAction::MuTQRunAction() :
    G4UserRunAction(),
    fAnalysisManager(new MuTQAnalysisManager()) {
    fAnalysisManager->Initialize();
}

MuTQRunAction::~MuTQRunAction() {
    delete G4AnalysisManager::Instance();
    delete fAnalysisManager;
}

void MuTQRunAction::BeginOfRunAction(const G4Run* run) {
    fAnalysisManager->Open();
    MuTQProgressMonitor::Instance().RunStart(run->GetNumberOfEventToBeProcessed());
}

void MuTQRunAction::EndOfRunAction(const G4Run*) {
    fAnalysisManager->WriteAndClose();
    MuTQProgressMonitor::Instance().RunComplete();
}

