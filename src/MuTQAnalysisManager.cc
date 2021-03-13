#include "MuTQAnalysisManager.hh"
#include "G4SystemOfUnits.hh"

const char* MuTQAnalysisManager::fRootFileName = "untitled";

void MuTQAnalysisManager::Initialize() const {
    auto g4AnalysisManager = G4Analysis::ManagerInstance("root");
    g4AnalysisManager->SetNtupleMerging(true);

    // Trees.

    // 0
    g4AnalysisManager->CreateNtuple("GeneratedEvent", "Infomation");
    g4AnalysisManager->CreateNtupleFColumn("Energy[GeV]");
    g4AnalysisManager->CreateNtupleFColumn("AzimuthAngle[rad]");
    g4AnalysisManager->CreateNtupleFColumn("ZenithAngle[rad]");
    g4AnalysisManager->FinishNtuple();
    // 1
    g4AnalysisManager->CreateNtuple("HitEvent", "Infomation");
    g4AnalysisManager->CreateNtupleFColumn("Energy[GeV]");
    g4AnalysisManager->CreateNtupleFColumn("AzimuthAngle[rad]");
    g4AnalysisManager->CreateNtupleFColumn("ZenithAngle[rad]");
    g4AnalysisManager->FinishNtuple();
    // 2
    g4AnalysisManager->CreateNtuple("ReconstructableEvent", "Infomation");
    g4AnalysisManager->CreateNtupleFColumn("Energy[GeV]");
    g4AnalysisManager->CreateNtupleFColumn("AzimuthAngle[rad]");
    g4AnalysisManager->CreateNtupleFColumn("ZenithAngle[rad]");
    g4AnalysisManager->FinishNtuple();
    // 3
    g4AnalysisManager->CreateNtuple("UnreconstructableEvent", "Infomation");
    g4AnalysisManager->CreateNtupleFColumn("Energy[GeV]");
    g4AnalysisManager->CreateNtupleFColumn("AzimuthAngle[rad]");
    g4AnalysisManager->CreateNtupleFColumn("ZenithAngle[rad]");
    g4AnalysisManager->FinishNtuple();
    // 4
    g4AnalysisManager->CreateNtuple("Hit2Event", "Infomation");
    g4AnalysisManager->CreateNtupleFColumn("Energy[GeV]");
    g4AnalysisManager->CreateNtupleFColumn("AzimuthAngle[rad]");
    g4AnalysisManager->CreateNtupleFColumn("ZenithAngle[rad]");
    g4AnalysisManager->FinishNtuple();
    // 5
    g4AnalysisManager->CreateNtuple("Hit3Event", "Infomation");
    g4AnalysisManager->CreateNtupleFColumn("Energy[GeV]");
    g4AnalysisManager->CreateNtupleFColumn("AzimuthAngle[rad]");
    g4AnalysisManager->CreateNtupleFColumn("ZenithAngle[rad]");
    g4AnalysisManager->FinishNtuple();
    // 6
    g4AnalysisManager->CreateNtuple("Hit4Event", "Infomation");
    g4AnalysisManager->CreateNtupleFColumn("Energy[GeV]");
    g4AnalysisManager->CreateNtupleFColumn("AzimuthAngle[rad]");
    g4AnalysisManager->CreateNtupleFColumn("ZenithAngle[rad]");
    g4AnalysisManager->FinishNtuple();
    // 7
    g4AnalysisManager->CreateNtuple("Hit5Event", "Infomation");
    g4AnalysisManager->CreateNtupleFColumn("Energy[GeV]");
    g4AnalysisManager->CreateNtupleFColumn("AzimuthAngle[rad]");
    g4AnalysisManager->CreateNtupleFColumn("ZenithAngle[rad]");
    g4AnalysisManager->FinishNtuple();
    // 8
    g4AnalysisManager->CreateNtuple("Hit6Event", "Infomation");
    g4AnalysisManager->CreateNtupleFColumn("Energy[GeV]");
    g4AnalysisManager->CreateNtupleFColumn("AzimuthAngle[rad]");
    g4AnalysisManager->CreateNtupleFColumn("ZenithAngle[rad]");
    g4AnalysisManager->FinishNtuple();

    // Histrograms.

#define MuTQ_H2_DIRECTION "Direction", 720, 0*deg, 360*deg, 180, 0*deg, 90*deg, "deg", "deg"
    g4AnalysisManager->CreateH2("DirectionOfGenerated", MuTQ_H2_DIRECTION); // 0
    g4AnalysisManager->CreateH2("DirectionOfHit", MuTQ_H2_DIRECTION); // 1
    g4AnalysisManager->CreateH2("DirectionOfReconstructable", MuTQ_H2_DIRECTION); // 2
    g4AnalysisManager->CreateH2("DirectionOfUnreconstructable", MuTQ_H2_DIRECTION); // 3
    g4AnalysisManager->CreateH2("DirectionOfHit2", MuTQ_H2_DIRECTION); // 4
    g4AnalysisManager->CreateH2("DirectionOfHit3", MuTQ_H2_DIRECTION); // 5
    g4AnalysisManager->CreateH2("DirectionOfHit4", MuTQ_H2_DIRECTION); // 6
    g4AnalysisManager->CreateH2("DirectionOfHit5", MuTQ_H2_DIRECTION); // 7
    g4AnalysisManager->CreateH2("DirectionOfHit6", MuTQ_H2_DIRECTION); // 8
}

void MuTQAnalysisManager::Open() const {
    G4AnalysisManager::Instance()->OpenFile(fRootFileName);
}

void MuTQAnalysisManager::Fill(G4int id, const G4double& energy, const G4double& phi, const G4double& theta) const {
    auto g4AnalysisManager = G4AnalysisManager::Instance();
    g4AnalysisManager->FillNtupleFColumn(id, 0, 1.0 / GeV * energy);
    g4AnalysisManager->FillNtupleFColumn(id, 1, 1.0 / rad * phi);
    g4AnalysisManager->FillNtupleFColumn(id, 2, 1.0 / rad * theta);
    g4AnalysisManager->AddNtupleRow(id);
    g4AnalysisManager->FillH2(id, phi, theta);
}

void MuTQAnalysisManager::WriteAndClose() const {
    G4AnalysisManager::Instance()->Write();
    G4AnalysisManager::Instance()->CloseFile();
}

