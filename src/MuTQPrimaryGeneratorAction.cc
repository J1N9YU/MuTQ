#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "Randomize.hh"
#include "g4analysis.hh"

#include "MuTQPrimaryGeneratorAction.hh"
#include "MuTQDetectorConstruction.hh"
#include "MuTQConfigs.hh"
#include "MuTQExtern.hh"

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

void MuTQPrimaryGeneratorAction::SetMuonProperties() const {
    G4double theta = 0.0;
    G4double energy = 0.0;
    FindEnergyAndTheta(energy, theta);
    G4double phi = 2.0 * M_PI * G4UniformRand();

    // basis vectors parallel to muon target.
    auto targetY = G4ThreeVector(0.0, 1.0, 0.0).rotateZ(phi);
    // basis vectors parallel to muon target.
    auto targetX = G4ThreeVector(1.0, 0.0, 0.0).rotateZ(phi).rotate(targetY, theta);
    // basis vectors propotional to muon target, point to muon coming direction.
    auto targetZ = G4ThreeVector(0.0, 0.0, 1.0).rotate(targetY, theta);
    // x coordinate of the aim(in target frame).
    G4double x;
    // y coordinate of the aim(in target frame).
    G4double y;
    do {
        x = 2.0 * targetRadius * G4UniformRand() - targetRadius;
        y = 2.0 * targetRadius * G4UniformRand() - targetRadius;
    } while (x * x + y * y > targetRadius * targetRadius);
    G4ThreeVector aim = MuTQDetectorConstruction::GetMuGridPosition() + x * targetX + y * targetY;

    G4ThreeVector position = aim + targetZ
        * ((MuTQDetectorConstruction::GetTerrainMaxZExtent() - aim.z()) / targetZ.z());

    // Muon definition.
    if (G4UniformRand() > 0.563319) {
        fParticleGun->SetParticleDefinition(G4MuonMinus::Definition());
    } else {
        fParticleGun->SetParticleDefinition(G4MuonPlus::Definition());
    }
    fParticleGun->SetParticleEnergy(energy);
    fParticleGun->SetParticlePosition(position);
    fParticleGun->SetParticleMomentumDirection(-targetZ);
}

void MuTQPrimaryGeneratorAction::FindEnergyAndTheta(G4double& energy, G4double& theta) const {
    G4double y;
    do {
        energy = G4UniformRand() * maxE_GeV;
        theta = G4UniformRand() * M_PI_2;
        y = G4UniformRand();
    } while (y > EnergySpectrum(energy, theta));
    energy *= CLHEP::GeV;
}

// Note: E in GeV, theta in rad.
//
G4double MuTQPrimaryGeneratorAction::EnergySpectrum(G4double E, G4double theta) const {
#if MuTQ_USING_REYNA && MuTQ_USING_TANG
#error "More than 1 energy spectrm are enabled!"
#elif MuTQ_USING_REYNA
    G4double cosTheta = cos(theta);
    G4double y = log(E * cosTheta);
    G4double exponent = 0.0481903 - 0.00171198 * y;
    exponent = -0.559371 + exponent * y;
    exponent = -0.2455 + exponent * y;
    return 0.973902 * cosTheta * cosTheta * cosTheta * pow(E * cosTheta, exponent);
#elif MuTQ_USING_TANG
    constexpr G4double p1 = 0.102573;
    constexpr G4double p2 = -0.068287;
    constexpr G4double p3 = 0.958633;
    constexpr G4double p4 = 0.0407253;
    constexpr G4double p5 = 0.817285;
    constexpr G4double r = -2.7;
    G4double x = cos(theta);
    G4double xStar = sqrt((x * x + p1 * p1 + p2 * pow(x, p3) + p4 * pow(x, p5)) / (1.0 + p1 * p1 + p2 + p4));
    G4double F = 0.0;
    if (E > 100.0 / xStar) {
        F = pow(E, r) * (1.0 / (1.0 + 1.1 / 115.0 * E * x) + 0.054 / (1.0 + 1.1 / 850.0 * E * x));
    } else {
        if (E < 1.0 / xStar) {
            E = 0.3 * E + 0.7 / xStar;
        }
        constexpr G4double rc = 0.0001;
        G4double Ecr = E + 0.00206 * (950.0 / xStar - 90.0);
        G4double A = 1.1 * pow(90.0 / 1030.0 * sqrt(x + 0.001), 4.5 / (E * xStar));
        F = A * pow(E, r) * (1.0 / (1.0 + 1.1 / 115.0 * Ecr * x) + 0.054 / (1.0 + 1.1 / 850.0 * Ecr * x) + rc);
    }
    return 4247.29821266489 * 0.14 * F;
#else
#error "No energy spectrm is enabled!"
#endif
}

