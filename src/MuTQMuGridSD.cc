#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4OpticalPhoton.hh"

#include "MuTQMuGridSD.hh"
#include "MuTQConfigs.hh"
#include "MuTQMuGridHit.hh"

MuTQMuGridSD::MuTQMuGridSD(const G4String& scintillatorSDName) :
    G4VSensitiveDetector(scintillatorSDName),
    fMuonHitsCollection(nullptr),
    fAnalysisManager(nullptr) {
    fAnalysisManager = G4AnalysisManager::Instance();
    collectionName.push_back("muon_hits_collection");
}

MuTQMuGridSD::~MuTQMuGridSD() {}

void MuTQMuGridSD::Initialize(G4HCofThisEvent* hitCollectionOfThisEvent) {
    fMuonHitsCollection = new MuTQMuGridHitsCollection(SensitiveDetectorName, collectionName[0]);
    hitCollectionOfThisEvent->AddHitsCollection(GetCollectionID(0), fMuonHitsCollection);
}

G4bool MuTQMuGridSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    auto presentParticle = step->GetTrack()->GetParticleDefinition();
    if (presentParticle != G4MuonPlus::Definition() &&
        presentParticle != G4MuonMinus::Definition()) {
        return false;
    }
    if (!step->IsFirstStepInVolume()) { return false; }
    // Present step point.
    auto preStepPoint = step->GetPreStepPoint();
    // A new hit.
    auto hit = new MuTQMuGridHit();
    hit->SetPosition(preStepPoint->GetPosition());
    hit->SetTime(preStepPoint->GetGlobalTime());
    hit->SetKineticEnergy(preStepPoint->GetKineticEnergy());
    hit->SetMomentum(preStepPoint->GetMomentum());
    hit->SetEnergyDeposition(step->GetTotalEnergyDeposit());
    hit->SetParticleDefinition(presentParticle);
    fMuonHitsCollection->insert(hit);
    return true;
}

void MuTQMuGridSD::EndOfEvent(G4HCofThisEvent*) {
    if (!fMuonHitsCollection) {
        G4ExceptionDescription exceptout;
        exceptout << "fMuonHitsCollection is nullptr." << G4endl;
        exceptout << "Maybe it was unexpectedly deleted?" << G4endl;
        G4Exception(
            "MuTQMuGridSD::EndOfEvent(G4HCofThisEvent*)",
            "HCNotFound",
            JustWarning,
            exceptout
        );
        return;
    }
    FillHistrogram();
}

void MuTQMuGridSD::FillHistrogram() const {
    for (size_t i = 0; i < fMuonHitsCollection->entries(); ++i) {
        auto hit = static_cast<MuTQMuGridHit*>(fMuonHitsCollection->GetHit(i));
        fAnalysisManager->FillH1(3, hit->GetKineticEnergy());
        fAnalysisManager->FillH1(4, M_PI - hit->GetMomentum().theta());
        fAnalysisManager->FillH1(5, hit->GetMomentum().phi());
    }
}

