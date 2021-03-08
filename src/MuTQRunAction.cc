#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "MuTQRunAction.hh"
#include "MuTQPrimaryGeneratorAction.hh"
#include "MuTQDetectorConstruction.hh"
#include "MuTQConfigs.hh"
#include "MuTQProgressMonitor.hh"
#include "MuTQExtern.hh"

MuTQRunAction::MuTQRunAction() :
    G4UserRunAction(),
    fAnalysisManager(nullptr) {
    if (gRunningInBatch) {
        fAnalysisManager = G4Analysis::ManagerInstance("root");
        G4cout << "G4Analysis manager is using " << fAnalysisManager->GetType() << '.' << G4endl;

        // setting
        fAnalysisManager->SetNtupleMerging(true);
        fAnalysisManager->SetVerboseLevel(0);
        fAnalysisManager->SetFileName(gRootFileName);
    } else {
        G4cout << "Running in interactive mode, SD and file I/O is disabled." << G4endl;
    }
}

MuTQRunAction::~MuTQRunAction() {
    if (gRunningInBatch) {
        delete G4AnalysisManager::Instance();
    }
}

void MuTQRunAction::BeginOfRunAction(const G4Run* run) {
    if (gRunningInBatch) {
        CreateTreeAndHistrogram();
        G4AnalysisManager::Instance()->OpenFile();
    }
    MuTQProgressMonitor::Instance()->StartRun(run->GetNumberOfEventToBeProcessed());
}

void MuTQRunAction::EndOfRunAction(const G4Run*) {
    if (gRunningInBatch) {
        G4AnalysisManager::Instance()->Write();
        G4AnalysisManager::Instance()->CloseFile();
    }
    MuTQProgressMonitor::Instance()->EndRun();
}

void MuTQRunAction::CreateTreeAndHistrogram() const {
    fAnalysisManager->CreateH1("EnergyOfGeneratedEvent", "Energy", 200, 0 * GeV, 1 * TeV, "GeV");
    fAnalysisManager->CreateH1("ZenithAngleOfGeneratedEvent", "ZenithAngle", 90, 0 * deg, 90 * deg, "deg");
    fAnalysisManager->CreateH1("PolarAngleOfGeneratedEvent", "PolarAngle", 360, -180 * deg, 180 * deg, "deg");
    fAnalysisManager->CreateH1("EnergyOfHitEvent", "Energy", 200, 0 * GeV, 1 * TeV, "GeV");
    fAnalysisManager->CreateH1("ZenithAngleOfHitEvent", "ZenithAngle", 90, 0 * deg, 90 * deg, "deg");
    fAnalysisManager->CreateH1("PolarAngleOfHitEvent", "PolarAngle", 360, -180 * deg, 180 * deg, "deg");
}

