#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "Randomize.hh"
#include "g4analysis.hh"

#include "MuTQPrimaryGeneratorAction.hh"

G4double MuTQPrimaryGeneratorAction::fSphereRadius = 0.0;
G4ThreeVector MuTQPrimaryGeneratorAction::fMuGridPosition;

MuTQPrimaryGeneratorAction::MuTQPrimaryGeneratorAction() :
    G4VUserPrimaryGeneratorAction(),
    fParticleGun(new G4ParticleGun(1)) {}

MuTQPrimaryGeneratorAction::~MuTQPrimaryGeneratorAction() {
    delete fParticleGun;
}

void MuTQPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    SetMuonProperties();
    fParticleGun->GeneratePrimaryVertex(anEvent);
    if (gRunningInBatch) {
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillH1(0, fParticleGun->GetParticleEnergy());
        analysisManager->FillH1(1, M_PI - fParticleGun->GetParticleMomentumDirection().theta());
        analysisManager->FillH1(2, fParticleGun->GetParticleMomentumDirection().phi());
    }
}

constexpr G4double _2_pi = 2.0 * M_PI;

void MuTQPrimaryGeneratorAction::SetMuonProperties() const {
    G4double theta = 0.0;
    G4double energy = 0.0;
    FindEnergyAndTheta(energy, theta);

    G4double sinTheta = sin(theta);
    G4double phi = _2_pi * G4UniformRand();
    auto relativePositionVec = G4ThreeVector(
        sinTheta * cos(phi),
        sinTheta * sin(phi),
        cos(theta)
    ) * fSphereRadius;

    auto sphereCentre = G4ThreeVector(
        2.0 * G4UniformRand() - 1.0,
        2.0 * G4UniformRand() - 1.0,
        0.0
    ) * effectiveRange;

    // Muon definition.
    if (G4UniformRand() > 0.563319) {
        fParticleGun->SetParticleDefinition(G4MuonMinus::Definition());
    } else {
        fParticleGun->SetParticleDefinition(G4MuonPlus::Definition());
    }
    fParticleGun->SetParticleEnergy(energy);
    fParticleGun->SetParticlePosition(relativePositionVec + sphereCentre + fMuGridPosition);
    fParticleGun->SetParticleMomentumDirection(-relativePositionVec);
}

void MuTQPrimaryGeneratorAction::FindEnergyAndTheta(G4double& energy, G4double& theta) const {
    G4double y;
    do {
        energy = G4UniformRand() * maxE_GeV;
        theta = G4UniformRand() * M_PI_2;
        y = G4UniformRand();
    } while (y > EnergySpectrum(energy, theta));
    energy *= GeV;
}

G4double MuTQPrimaryGeneratorAction::EnergySpectrum(G4double E_GeV, G4double theta) const {
    G4double cosTheta = cos(theta);
    G4double y = log(E_GeV * cosTheta);
    G4double exponent = 0.0481903 - 0.00171198 * y;
    exponent = -0.559371 + exponent * y;
    exponent = -0.2455 + exponent * y;
    return 0.973902 * cosTheta * cosTheta * cosTheta * pow(E_GeV * cosTheta, exponent);
}

