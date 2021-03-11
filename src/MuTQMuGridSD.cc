#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4OpticalPhoton.hh"

#include "MuTQMuGridSD.hh"
#include "MuTQConfigs.hh"
#include "MuTQMuGridHit.hh"

MuTQMuGridSD::MuTQMuGridSD(const G4String& scintillatorSDName) :
    G4VSensitiveDetector(scintillatorSDName),
    fMuonHitsCollection(nullptr) {
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
    hit->SetScintillatorID(preStepPoint->GetPhysicalVolume()->GetCopyNo());
    hit->SetKineticEnergy(preStepPoint->GetKineticEnergy());
    hit->SetMomentum(preStepPoint->GetMomentum());
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
    if (fMuonHitsCollection->entries() == 0) { return; }

    auto hit = static_cast<MuTQMuGridHit*>(fMuonHitsCollection->GetHit(0));
    auto energy = hit->GetKineticEnergy();
    auto phi = M_PI + hit->GetMomentum().phi();
    auto theta = M_PI - hit->GetMomentum().theta();

    MuTQAnalysisManager::Instance().Fill(1, energy, phi, theta);
    if (fMuonHitsCollection->entries() > 1) {
        MuTQAnalysisManager::Instance().Fill(2, energy, phi, theta);
        MuTQAnalysisManager::Instance().Fill(fMuonHitsCollection->entries() + 2, energy, phi, theta);
    } else {
        MuTQAnalysisManager::Instance().Fill(3, energy, phi, theta);
    }
}

