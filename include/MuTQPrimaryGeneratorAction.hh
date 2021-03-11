#ifndef MuTQ_PRIMARY_GENERATOR_ACTION_H
#define MuTQ_PRIMARY_GENERATOR_ACTION_H 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "MuTQAnalysisManager.hh"

class G4ParticleGun;
class G4Event;

constexpr G4double maxE_GeV = 10000;
constexpr G4double targetSize = 60 * CLHEP::cm;

class MuTQPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
private:
    G4ParticleGun* fMuonPlusGun;
    G4ParticleGun* fMuonMinusGun;

public:
    MuTQPrimaryGeneratorAction();
    virtual ~MuTQPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);

private:
    G4double EnergySpectrum(G4double E_GeV, G4double theta) const;
    void FindEnergyAndTheta(G4double& energy, G4double& theta) const;
};

#endif

