#ifndef MuTQ_DETECTOR_CONSTRUCTION_H
#define MuTQ_DETECTOR_CONSTRUCTION_H 1

#include "globals.hh"
#include "G4TessellatedSolid.hh"
#include "G4VUserDetectorConstruction.hh"

typedef std::pair<G4VPhysicalVolume*, G4VPhysicalVolume*> G4VPhysicalVolumePair;

class MuTQDetectorConstruction : public G4VUserDetectorConstruction {
private:
    //
    // MuGrid logical volume
    // Note: use for registering sensitive detector in ConstructSDandField().
    G4LogicalVolume* fLogicalMuGrid;

public:
    MuTQDetectorConstruction();
    virtual ~MuTQDetectorConstruction() {}

public:
    virtual G4VPhysicalVolume* Construct();

private:
    virtual void ConstructSDandField();

    typedef std::array<G4ThreeVector, 3> Triangle;
    static G4TessellatedSolid* ConstructTerrain(
        const std::string& pointsFile, const std::string& meshFile, const std::string& boundaryFile);
    static void RearrangeToCounterClockWise(Triangle& t);
    static void RearrangeToClockWise(Triangle& t);
};

#endif

