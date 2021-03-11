#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "MuTQRunAction.hh"
#include "MuTQPrimaryGeneratorAction.hh"
#include "MuTQDetectorConstruction.hh"
#include "MuTQConfigs.hh"

MuTQRunAction::MuTQRunAction() :
    G4UserRunAction() {
    MuTQAnalysisManager::Instance().Initialize();
}

MuTQRunAction::~MuTQRunAction() {
    delete G4AnalysisManager::Instance();
}

void MuTQRunAction::BeginOfRunAction(const G4Run* run) {
    MuTQAnalysisManager::Instance().Open();
    MuTQProgressMonitor::Instance().RunStart(run->GetNumberOfEventToBeProcessed());
}

void MuTQRunAction::EndOfRunAction(const G4Run*) {
    MuTQAnalysisManager::Instance().WriteAndClose();
    MuTQProgressMonitor::Instance().RunComplete();
}

