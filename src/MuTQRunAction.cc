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
#define MuTQ_HISTROGRAM_ENERGY "Energy", 200, 0 * GeV, 1 * TeV, "GeV"
#define MuTQ_HISTROGRAM_ZENITH "ZenithAngle", 90, 0 * deg, 90 * deg, "deg"
#define MuTQ_HISTROGRAM_POLAR "PolarAngle", 360, -180 * deg, 180 * deg, "deg"
    fAnalysisManager->CreateH1("EnergyOfGenerated", MuTQ_HISTROGRAM_ENERGY);
    fAnalysisManager->CreateH1("ZenithAngleOfGenerated", MuTQ_HISTROGRAM_ZENITH);
    fAnalysisManager->CreateH1("PolarAngleOfGenerated", MuTQ_HISTROGRAM_POLAR);
    fAnalysisManager->CreateH1("EnergyOfHit", MuTQ_HISTROGRAM_ENERGY);
    fAnalysisManager->CreateH1("ZenithAngleOfHit", MuTQ_HISTROGRAM_ZENITH);
    fAnalysisManager->CreateH1("PolarAngleOfHit", MuTQ_HISTROGRAM_POLAR);
    fAnalysisManager->CreateH1("NumOfScintillatorHit", "NumOfScintillator", 7, 1, 8);
    fAnalysisManager->CreateH1("ScintillatorIDOfUnreconstructable", "ScintillatorID", 18, 1, 19);
    fAnalysisManager->CreateH1("EnergyOfReconstructable", MuTQ_HISTROGRAM_ENERGY);
    fAnalysisManager->CreateH1("ZenithAngleOfReconstructable", MuTQ_HISTROGRAM_ZENITH);
    fAnalysisManager->CreateH1("PolarAngleOfReconstructable", MuTQ_HISTROGRAM_POLAR);
    fAnalysisManager->CreateH1("EnergyOfUnreconstructable", MuTQ_HISTROGRAM_ENERGY);
    fAnalysisManager->CreateH1("ZenithAngleOfUnreconstructable", MuTQ_HISTROGRAM_ZENITH);
    fAnalysisManager->CreateH1("PolarAngleOfUnreconstructable", MuTQ_HISTROGRAM_POLAR);
    fAnalysisManager->CreateH1("EnergyOfHit2", MuTQ_HISTROGRAM_ENERGY);
    fAnalysisManager->CreateH1("ZenithAngleOfHit2", MuTQ_HISTROGRAM_ZENITH);
    fAnalysisManager->CreateH1("PolarAngleOfHit2", MuTQ_HISTROGRAM_POLAR);
    fAnalysisManager->CreateH1("EnergyOfHit3", MuTQ_HISTROGRAM_ENERGY);
    fAnalysisManager->CreateH1("ZenithAngleOfHit3", MuTQ_HISTROGRAM_ZENITH);
    fAnalysisManager->CreateH1("PolarAngleOfHit3", MuTQ_HISTROGRAM_POLAR);
    fAnalysisManager->CreateH1("EnergyOfHit4", MuTQ_HISTROGRAM_ENERGY);
    fAnalysisManager->CreateH1("ZenithAngleOfHit4", MuTQ_HISTROGRAM_ZENITH);
    fAnalysisManager->CreateH1("PolarAngleOfHit4", MuTQ_HISTROGRAM_POLAR);
    fAnalysisManager->CreateH1("EnergyOfHit5", MuTQ_HISTROGRAM_ENERGY);
    fAnalysisManager->CreateH1("ZenithAngleOfHit5", MuTQ_HISTROGRAM_ZENITH);
    fAnalysisManager->CreateH1("PolarAngleOfHit5", MuTQ_HISTROGRAM_POLAR);
    fAnalysisManager->CreateH1("EnergyOfHit6", MuTQ_HISTROGRAM_ENERGY);
    fAnalysisManager->CreateH1("ZenithAngleOfHit6", MuTQ_HISTROGRAM_ZENITH);
    fAnalysisManager->CreateH1("PolarAngleOfHit6", MuTQ_HISTROGRAM_POLAR);
}

