#include "MuTQMuGridHit.hh"
#include "MuTQDetectorConstruction.hh"

G4ThreadLocal G4Allocator<MuTQMuGridHit>* MuTQScintillatorHitAllocator = nullptr;

MuTQMuGridHit::MuTQMuGridHit() :
    G4VHit(),
    fScintillatorID(-1),
    fTime(0.0),
    fPosition(0.0),
    fMomentum(0.0),
    fKineticEnergy(0.0),
    fEnergyDeposition(0.0),
    fParticleDefinition(nullptr) {}

MuTQMuGridHit::MuTQMuGridHit(const MuTQMuGridHit& rhs) :
    G4VHit(),
    fScintillatorID(rhs.fScintillatorID),
    fTime(rhs.fTime),
    fPosition(rhs.fPosition),
    fMomentum(rhs.fMomentum),
    fKineticEnergy(rhs.fKineticEnergy),
    fEnergyDeposition(rhs.fEnergyDeposition),
    fParticleDefinition(rhs.fParticleDefinition) {}

MuTQMuGridHit::~MuTQMuGridHit() {}

const MuTQMuGridHit& MuTQMuGridHit::operator=(const MuTQMuGridHit& rhs) {
    if (&rhs != this) {
        this->fScintillatorID = rhs.fScintillatorID;
        this->fTime = rhs.fTime;
        this->fPosition = rhs.fPosition;
        this->fMomentum = rhs.fMomentum;
        this->fKineticEnergy = rhs.fKineticEnergy;
        this->fEnergyDeposition = rhs.fEnergyDeposition;
        this->fParticleDefinition = rhs.fParticleDefinition;
    }
    return *this;
}

