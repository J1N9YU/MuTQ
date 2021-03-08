#ifndef MuTQ_DETECTOR_CONSTRUCTION_H
#define MuTQ_DETECTOR_CONSTRUCTION_H 1

#include "globals.hh"
#include "G4TessellatedSolid.hh"
#include "G4PVPlacement.hh"
#include "G4VUserDetectorConstruction.hh"

class MuTQDetectorConstruction : public G4VUserDetectorConstruction {
public:
    // World

    static const G4String fWorldName;
    static const G4String fWorldMaterialName;

    // Terrian

    static const G4String fTerrainName;
    static const G4String fTerrainMaterialName;

    // Tunnel

    static const G4String fTunnelName;
    static const G4double fTunnelRadius;
    static const G4double fTunnelSliceAngle;
    static const G4double fTunnelGrad;
    static const G4double fTunnelSlopeAngle;
    static const G4double fTunnelEntranceX;
    static const G4double fTunnelEntranceAltitude;
    static const G4double fTunnelLength;

    // MuGrid

    static const G4String fMuGridName;
    static const G4double fMuGridHeight;
    static const G4double fMuGridHalfSize[3];

    // temp object

    static const G4String fTempName;

private:
    // Solid terrain
    static G4TessellatedSolid* fSolidTerrain;
    // Logical MuGrid
    // Note: For register SD.
    G4LogicalVolume* fLogicalMuGrid;

    static G4double fTerrainMaxX;
    static G4double fTerrainMinX;
    static G4double fTerrainMaxY;
    static G4double fTerrainMinY;
    static G4double fTerrainMaxZ;
    static G4ThreeVector fMuGridPosition;

public:
    MuTQDetectorConstruction();
    virtual ~MuTQDetectorConstruction() {}

    virtual G4VPhysicalVolume* Construct();

    static G4double GetTerrainMaxXExtent() { return fTerrainMaxX; }
    static G4double GetTerrainMinXExtent() { return fTerrainMinX; }
    static G4double GetTerrainMaxYExtent() { return fTerrainMaxY; }
    static G4double GetTerrainMinYExtent() { return fTerrainMinY; }
    static G4double GetTerrainMaxZExtent() { return fTerrainMaxZ; }
    static const G4ThreeVector& GetMuGridPosition() { return fMuGridPosition; }

private:
    virtual void ConstructSDandField();

    typedef std::array<G4ThreeVector, 3> Triangle;
    static G4TessellatedSolid* ConstructTerrain(
        const std::string& pointsFile, const std::string& meshFile, const std::string& boundaryFile);
    static void RearrangeToCounterClockWise(Triangle& t);
    static void RearrangeToClockWise(Triangle& t);
};

#endif

