#include "MuTQDetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4TriangularFacet.hh"
#include "G4QuadrangularFacet.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4SDManager.hh"

#include "MuTQExtern.hh"
#include "MuTQConfigs.hh"
#include "CreateMapFromCSV.hh"
#include "MuTQMuGridSD.hh"
#include "MuTQPrimaryGeneratorAction.hh"

#include <algorithm>

// World

const G4String MuTQDetectorConstruction::fWorldName = "world";
const G4String MuTQDetectorConstruction::fWorldMaterialName = "void";

// Terrian

const G4String MuTQDetectorConstruction::fTerrainName = "terrian";
const G4String MuTQDetectorConstruction::fTerrainMaterialName = "granite";
G4TessellatedSolid* MuTQDetectorConstruction::fSolidTerrain = nullptr;
G4double MuTQDetectorConstruction::fTerrainMaxX = 0.0;
G4double MuTQDetectorConstruction::fTerrainMinX = 0.0;
G4double MuTQDetectorConstruction::fTerrainMaxY = 0.0;
G4double MuTQDetectorConstruction::fTerrainMinY = 0.0;
G4double MuTQDetectorConstruction::fTerrainMaxZ = 0.0;

// Tunnel

const G4String MuTQDetectorConstruction::fTunnelName = "tunnel";
const G4double MuTQDetectorConstruction::fTunnelRadius = 8.0 * m;
const G4double MuTQDetectorConstruction::fTunnelSliceAngle = 204 * deg;
const G4double MuTQDetectorConstruction::fTunnelGrad = 0.003;
const G4double MuTQDetectorConstruction::fTunnelSlopeAngle = atan(fTunnelGrad);
const G4double MuTQDetectorConstruction::fTunnelEntranceX = 55 * m;
const G4double MuTQDetectorConstruction::fTunnelEntranceAltitude = 10.982 * m;
const G4double MuTQDetectorConstruction::fTunnelLength = 529.389 * m - fTunnelEntranceX;
G4ThreeVector MuTQDetectorConstruction::fMuGridPosition = G4ThreeVector();

// MuGrid

const G4String MuTQDetectorConstruction::fMuGridName = "MuGrid";
const G4double MuTQDetectorConstruction::fMuGridPlaceHeight = 50 * cm;
const G4double MuTQDetectorConstruction::fMuGridWidth = 10 * cm;
const G4double MuTQDetectorConstruction::fMuGridLength = 60 * cm;

// temp object

const G4String MuTQDetectorConstruction::fTempName = "_tmp";

MuTQDetectorConstruction::MuTQDetectorConstruction() :
    fLogicalSingleMuGrid(nullptr) {}

G4VPhysicalVolume* MuTQDetectorConstruction::Construct() {
    // Set if check overlaps
    //
    constexpr G4bool checkOverlaps = true;

    auto nist = G4NistManager::Instance();

    // ============================================================================
    // Solid terrain
    // ============================================================================

    if (!fSolidTerrain) {
        fSolidTerrain = ConstructTerrain(
            "datafiles/terrainPoints.csv",
            "datafiles/terrainMeshDef.csv",
            "datafiles/terrainBoundaryPoints.csv"
        );
        fTerrainMaxX = fSolidTerrain->GetMaxXExtent();
        fTerrainMinX = fSolidTerrain->GetMinXExtent();
        fTerrainMaxY = fSolidTerrain->GetMaxYExtent();
        fTerrainMinY = fSolidTerrain->GetMinYExtent();
        fTerrainMaxZ = fSolidTerrain->GetMaxZExtent();
    }

    // ============================================================================
    // Solid tunnel
    // ============================================================================

    G4double groundToCentre = fTunnelRadius * sin(fTunnelSliceAngle * 0.5 - 90 * deg);
    auto solidTunnel = new G4SubtractionSolid(
        fTunnelName,
        new G4Tubs(
            fTempName,
            0.0,
            fTunnelRadius,
            fTunnelLength * 0.5,
            0.0,
            2 * M_PI
        ),
        new G4Box(
            fTempName,
            fTunnelRadius,
            fTunnelRadius,
            fTunnelLength
        ),
        nullptr,
        G4ThreeVector(-fTunnelRadius - groundToCentre)
    );

    // ============================================================================
    // Solid terrain - tunnel
    // ============================================================================

    G4RotationMatrix flip(
        G4ThreeVector(0.0, 1.0, 0.0),
        -90 * deg - fTunnelSlopeAngle
    );
    G4ThreeVector correct(
        0.5 * fTunnelLength * (cos(fTunnelSlopeAngle) - 1.0),
        0.0,
        0.5 * fTunnelLength * sin(fTunnelSlopeAngle)
    );
    G4ThreeVector move(
        fTunnelEntranceX + 0.5 * fTunnelLength,
        0.0,
        fTunnelEntranceAltitude + groundToCentre
    );
    auto fSolidTerrainWithTunnel = new G4SubtractionSolid(
        fTerrainName + "-" + fTunnelName,
        fSolidTerrain,
        solidTunnel,
        G4Transform3D(
            flip,
            correct + move
        )
    );

    // ============================================================================
    // world
    // ============================================================================

    // world material
    //
    auto worldMaterial = nist->BuildMaterialWithNewDensity(fWorldMaterialName, "G4_AIR", DBL_EPSILON);

    // world construction

    auto solidWorld = new G4Box(
        fWorldName,
        std::max(fabs(fTerrainMaxX), fabs(fTerrainMinX)) + 1 * mm,
        std::max(fabs(fTerrainMaxY), fabs(fTerrainMinY)) + 1 * mm,
        fTerrainMaxZ + 1 * mm
    );
    auto logicalWorld = new G4LogicalVolume(
        solidWorld,
        worldMaterial,
        fWorldName
    );
    auto physicalWorld = new G4PVPlacement(
        G4Transform3D(),
        fWorldName,
        logicalWorld,
        nullptr,
        false, 0,
        checkOverlaps
    );

    // ============================================================================
    // terrain - tunnel
    // ============================================================================

    // terrain material
    //
    auto terrainMaterial = new G4Material(
        fTerrainMaterialName,
        3.0 * g / cm3,
        6,
        kStateSolid
    );
    terrainMaterial->AddElement(nist->FindOrBuildElement("O"), 0.641);
    terrainMaterial->AddElement(nist->FindOrBuildElement("Si"), 0.25);
    terrainMaterial->AddElement(nist->FindOrBuildElement("Al"), 0.08);
    terrainMaterial->AddElement(nist->FindOrBuildElement("Ca"), 0.015);
    terrainMaterial->AddElement(nist->FindOrBuildElement("Mg"), 0.01);
    terrainMaterial->AddElement(nist->FindOrBuildElement("Fe"), 0.004);

    // terrain - tunnel construction

    auto logicalTerrainWithTunnel = new G4LogicalVolume(
        fSolidTerrainWithTunnel,
        terrainMaterial,
        fTerrainName + "-" + fTunnelName
    );
    new G4PVPlacement(
        G4Transform3D(),
        fTerrainName + "-" + fTunnelName,
        logicalTerrainWithTunnel,
        physicalWorld,
        false, 0,
        checkOverlaps
    );

    // ============================================================================
    // MuGrid
    // ============================================================================

    // MuGrid Material
    //
    auto MuGridMaterial = nist->FindOrBuildMaterial("G4_POLYCARBONATE");

    fMuGridPosition =
        G4ThreeVector(fTunnelEntranceX, 0, fTunnelEntranceAltitude + 3 * fMuGridWidth + fMuGridPlaceHeight)
        + G4ThreeVector(gMuGridPosition[0] - fTunnelEntranceX, gMuGridPosition[1]).rotateY(-fTunnelSlopeAngle);

    // MuGrid construction

    auto solidSingleMuGrid = new G4Box(
        fMuGridName,
        fMuGridLength * 0.5,
        fMuGridWidth * 0.5,
        fMuGridWidth * 0.5
    );
    fLogicalSingleMuGrid = new G4LogicalVolume(
        solidSingleMuGrid,
        MuGridMaterial,
        fMuGridName
    );
    G4ThreeVector oddLayerPosition(-2.0 * fMuGridWidth, 0.0, 2.5 * fMuGridWidth);
    G4ThreeVector evenLayerPosition(0.0, 2.0 * fMuGridWidth, 1.5 * fMuGridWidth);
    G4bool isOdd = true;
    for (size_t i = 0; i < 18; ++i) {
        if (isOdd) {
            new G4PVPlacement(
                G4Transform3D(
                    G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 90 * deg),
                    oddLayerPosition + fMuGridPosition
                ),
                fMuGridName,
                fLogicalSingleMuGrid,
                physicalWorld,
                true, i,
                checkOverlaps
            );
            oddLayerPosition.setX(oddLayerPosition.x() + 2.0 * fMuGridWidth);
        } else {
            new G4PVPlacement(
                G4Transform3D(
                    G4RotationMatrix(),
                    evenLayerPosition + fMuGridPosition
                ),
                fMuGridName,
                fLogicalSingleMuGrid,
                physicalWorld,
                true, i,
                checkOverlaps
            );
            evenLayerPosition.setY(evenLayerPosition.y() - 2.0 * fMuGridWidth);
        }
        if ((i + 1) % 3 == 0) {
            if (isOdd) {
                oddLayerPosition.setZ(oddLayerPosition.z() - 2.0 * fMuGridWidth);
                oddLayerPosition.setX(-2.0 * fMuGridWidth);
                isOdd = false;
            } else {
                evenLayerPosition.setZ(evenLayerPosition.z() - 2.0 * fMuGridWidth);
                evenLayerPosition.setY(2.0 * fMuGridWidth);
                isOdd = true;
            }
        }
    }

    return physicalWorld;
}

void MuTQDetectorConstruction::ConstructSDandField() {
    if (gRunningInBatch) {
        auto SDManager = G4SDManager::GetSDMpointer();
        auto MuGridSD = new MuTQMuGridSD(fMuGridName);
        SDManager->AddNewDetector(MuGridSD);
        SetSensitiveDetector(fLogicalSingleMuGrid, MuGridSD);
    }
}

G4TessellatedSolid* MuTQDetectorConstruction::ConstructTerrain(
    const std::string& pointsFile, const std::string& meshFile, const std::string& boundaryFile) {
    // Get points.
    auto pointMap(CreateMapFromCSV<G4int, G4double>(pointsFile));
    G4ThreeVectorList point(pointMap.size());
    G4ThreeVectorList prjPoint(pointMap.size());
    for (size_t i = 0; i < pointMap.size(); ++i) {
        auto pointVector = pointMap[i];
        point[i] = G4ThreeVector(pointVector[0], pointVector[1], pointVector[2]) * m;
        prjPoint[i] = G4ThreeVector(pointVector[0], pointVector[1], 0) * m;
    }

    // Get mesh definition.
    auto meshDefMap(CreateMapFromCSV<G4int, G4int>(meshFile));
    std::vector<Triangle> topMesh(meshDefMap.size());
    std::vector<Triangle> botMesh(meshDefMap.size());
    for (size_t i = 0; i < topMesh.size(); ++i) {
        auto meshDef = meshDefMap[i];
        topMesh[i] = { point[meshDef[0]], point[meshDef[1]], point[meshDef[2]] };
        RearrangeToCounterClockWise(topMesh[i]);
        botMesh[i] = { prjPoint[meshDef[0]], prjPoint[meshDef[1]], prjPoint[meshDef[2]] };
        RearrangeToClockWise(botMesh[i]);
    }

    // Get mesh boundary.
    auto boundaryPointIndex(CreateMapFromCSV<std::string, G4int>(boundaryFile)["AntiClockWiseBoundary"]);

    G4TessellatedSolid* terrain = new G4TessellatedSolid(fTerrainName);

    // Create top surface.
    for (const auto& aMesh : topMesh) {
        terrain->AddFacet(
            new G4TriangularFacet(
                std::get<0>(aMesh),
                std::get<1>(aMesh),
                std::get<2>(aMesh),
                ABSOLUTE
            )
        );
    }
    // Create bottom surface.
    for (const auto& aMesh : botMesh) {
        terrain->AddFacet(
            new G4TriangularFacet(
                std::get<0>(aMesh),
                std::get<1>(aMesh),
                std::get<2>(aMesh),
                ABSOLUTE
            )
        );
    }
    // Create cliff.
    auto last = boundaryPointIndex.end() - 1;
    auto here = boundaryPointIndex.begin();
    do {
        terrain->AddFacet(
            new G4QuadrangularFacet(
                point[*here],
                point[*last],
                prjPoint[*last],
                prjPoint[*here],
                ABSOLUTE
            )
        );
        last = here;
        ++here;
    } while (here != boundaryPointIndex.end());

    terrain->SetSolidClosed(true);

    return terrain;
}

void MuTQDetectorConstruction::RearrangeToCounterClockWise(Triangle& t) {
    G4ThreeVector c = (t[0] + t[1] + t[2]) / 3.0;
    if ((t[0] - c).cross(t[1] - c).z() < 0) {
        std::swap(t[0], t[1]);
    }
}

void MuTQDetectorConstruction::RearrangeToClockWise(Triangle& t) {
    G4ThreeVector c = (t[0] + t[1] + t[2]) / 3.0;
    if ((t[0] - c).cross(t[1] - c).z() > 0) {
        std::swap(t[0], t[1]);
    }
}
