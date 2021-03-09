#ifndef MuTQ_SCINTILLATOR_HIT_H
#define MuTQ_SCINTILLATOR_HIT_H 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"

class MuTQMuGridHit : public G4VHit {
private:
    G4int                       fScintillatorID;
    // G4double                    fTime;
    // G4ThreeVector               fPosition;
    G4ThreeVector               fMomentum;
    G4double                    fKineticEnergy;
    // G4double                    fEnergyDeposition;
    // const G4ParticleDefinition* fParticleDefinition;

public:
    MuTQMuGridHit();
    MuTQMuGridHit(const MuTQMuGridHit& rhs);
    ~MuTQMuGridHit();
    const MuTQMuGridHit& operator=(const MuTQMuGridHit& rhs);
    G4bool operator==(const MuTQMuGridHit& rhs) const { return this == &rhs; }
    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    const G4int& GetScintillatorID() const { return fScintillatorID; }
    // const G4double& GetTime() const { return fTime; }
    // const G4ThreeVector& GetPosition() const { return fPosition; }
    const G4ThreeVector& GetMomentum() const { return fMomentum; }
    const G4double& GetKineticEnergy() const { return fKineticEnergy; }
    // const G4double& GetEnergyDeposition() const { return fEnergyDeposition; }
    // const G4ParticleDefinition* GetParticleDefinition() const { return fParticleDefinition; }

    void SetScintillatorID(const G4int& scintillatorID) { fScintillatorID = scintillatorID; }
    // void SetTime(const G4double& time) { fTime = time; }
    // void SetPosition(const G4ThreeVector& position) { fPosition = position; }
    void SetMomentum(const G4ThreeVector& momentum) { fMomentum = momentum; }
    void SetKineticEnergy(const G4double& kineticEnergy) { fKineticEnergy = kineticEnergy; }
    // void SetEnergyDeposition(const G4double& energyDeposition) { fEnergyDeposition = energyDeposition; }
    // void SetParticleDefinition(const G4ParticleDefinition* particleDefinition) {
    //     fParticleDefinition = particleDefinition;
    // }
};

typedef G4THitsCollection<MuTQMuGridHit> MuTQMuGridHitsCollection;

extern G4ThreadLocal G4Allocator<MuTQMuGridHit>* MuTQScintillatorHitAllocator;

inline void* MuTQMuGridHit::operator new(size_t) {
    if (!MuTQScintillatorHitAllocator) {
        MuTQScintillatorHitAllocator = new G4Allocator<MuTQMuGridHit>();
    }
    return (void*)MuTQScintillatorHitAllocator->MallocSingle();
}

inline void MuTQMuGridHit::operator delete(void* aHit) {
    MuTQScintillatorHitAllocator->FreeSingle(static_cast<MuTQMuGridHit*>(aHit));
}

#endif

