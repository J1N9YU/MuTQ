#include "MuTQAnalysisManager.hh"
#include "G4SystemOfUnits.hh"

MuTQAnalysisManager::MuTQAnalysisManager() :
    fRootFileName("output") {}

constexpr G4double ceilingHeight = 10 * m;
constexpr G4double halfSpanAngle = 45 * deg;

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

#define MuTQ_H2_CEILING_SIZE 100, -ceilingHeight * tan(halfSpanAngle), ceilingHeight * tan(halfSpanAngle)
#define MuTQ_H2_CEILING_INTERSECTION "IntersectionPoint", MuTQ_H2_CEILING_SIZE, MuTQ_H2_CEILING_SIZE, "m", "m"
    g4AnalysisManager->CreateH2("CeilIntersectionOfGenerated", MuTQ_H2_CEILING_INTERSECTION); // 0
    g4AnalysisManager->CreateH2("CeilIntersectionOfHit", MuTQ_H2_CEILING_INTERSECTION); // 1
    g4AnalysisManager->CreateH2("CeilIntersectionOfReconstructable", MuTQ_H2_CEILING_INTERSECTION); // 2
    g4AnalysisManager->CreateH2("CeilIntersectionOfUnreconstructable", MuTQ_H2_CEILING_INTERSECTION); // 3
    g4AnalysisManager->CreateH2("CeilIntersectionOfHit2", MuTQ_H2_CEILING_INTERSECTION); // 4
    g4AnalysisManager->CreateH2("CeilIntersectionOfHit3", MuTQ_H2_CEILING_INTERSECTION); // 5
    g4AnalysisManager->CreateH2("CeilIntersectionOfHit4", MuTQ_H2_CEILING_INTERSECTION); // 6
    g4AnalysisManager->CreateH2("CeilIntersectionOfHit5", MuTQ_H2_CEILING_INTERSECTION); // 7
    g4AnalysisManager->CreateH2("CeilIntersectionOfHit6", MuTQ_H2_CEILING_INTERSECTION); // 8

#define MuTQ_H2_DIRECTION "Direction", 90, 0*deg, 360*deg, 90, 0*deg, 90*deg, "deg", "deg"
    g4AnalysisManager->CreateH2("DirectionOfGenerated", MuTQ_H2_DIRECTION); // 9
    g4AnalysisManager->CreateH2("DirectionOfHit", MuTQ_H2_DIRECTION); // 10
    g4AnalysisManager->CreateH2("DirectionOfReconstructable", MuTQ_H2_DIRECTION); // 11
    g4AnalysisManager->CreateH2("DirectionOfUnreconstructable", MuTQ_H2_DIRECTION); // 12
    g4AnalysisManager->CreateH2("DirectionOfHit2", MuTQ_H2_DIRECTION); // 13
    g4AnalysisManager->CreateH2("DirectionOfHit3", MuTQ_H2_DIRECTION); // 14
    g4AnalysisManager->CreateH2("DirectionOfHit4", MuTQ_H2_DIRECTION); // 15
    g4AnalysisManager->CreateH2("DirectionOfHit5", MuTQ_H2_DIRECTION); // 16
    g4AnalysisManager->CreateH2("DirectionOfHit6", MuTQ_H2_DIRECTION); // 17
}

void MuTQAnalysisManager::Open() const {
    G4AnalysisManager::Instance()->OpenFile(fRootFileName);
}

void MuTQAnalysisManager::Fill(G4int&& id, const G4double& energy, const G4double& phi, const G4double& theta) const {
    G4AnalysisManager::Instance()->FillNtupleFColumn(id, 0, 1.0 / CLHEP::GeV * energy);
    G4AnalysisManager::Instance()->FillNtupleFColumn(id, 1, 1.0 / CLHEP::rad * phi);
    G4AnalysisManager::Instance()->FillNtupleFColumn(id, 2, 1.0 / CLHEP::rad * theta);
    G4AnalysisManager::Instance()->AddNtupleRow(id);
    G4double rho = ceilingHeight * tan(theta);
    G4AnalysisManager::Instance()->FillH2(id, rho * cos(phi), rho * sin(phi));
    G4AnalysisManager::Instance()->FillH2(id + 9, phi, theta);
}

void MuTQAnalysisManager::WriteAndClose() const {
    G4AnalysisManager::Instance()->Write();
    G4AnalysisManager::Instance()->CloseFile();
}

