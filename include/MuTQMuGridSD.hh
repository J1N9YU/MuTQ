#ifndef MuTQ_SCINTILLATOR_SD_H
#define MuTQ_SCINTILLATOR_SD_H 1

#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "g4analysis.hh"

#include "MuTQMuGridHit.hh"

class MuTQMuGridSD : public G4VSensitiveDetector {
private:
    MuTQMuGridHitsCollection* fMuonHitsCollection;

    G4ToolsAnalysisManager* fAnalysisManager;

public:
    MuTQMuGridSD(const G4String& scintillatorSDName);
    virtual ~MuTQMuGridSD();

    virtual void Initialize(G4HCofThisEvent* eventHitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory*);
    virtual void EndOfEvent(G4HCofThisEvent*);

private:
    void FillHistrogram() const;
};

#endif

