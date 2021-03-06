#ifndef MuTQ_PRIMARY_GENERATOR_ACTION_H
#define MuTQ_PRIMARY_GENERATOR_ACTION_H 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

#include "CreateMapFromCSV.hh"
#include "MuTQGlobal.hh"

class G4ParticleGun;
class G4Event;

constexpr G4double maxE_GeV = 10000;
constexpr G4double effectiveRange = 0.25 * m;

class MuTQPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
private:
    G4ParticleGun* fParticleGun;
public:
    static G4double fSphereRadius;
    static G4ThreeVector fMuGridPosition;

public:
    MuTQPrimaryGeneratorAction();
    virtual ~MuTQPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);
    inline const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

private:
    G4double EnergySpectrum(G4double E_GeV, G4double theta) const;
    void FindEnergyAndTheta(G4double& energy, G4double& theta) const;

    void SetMuonProperties() const;
};

#endif

