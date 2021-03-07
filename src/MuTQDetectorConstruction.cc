#include "MuTQDetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4TriangularFacet.hh"
#include "G4QuadrangularFacet.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4SDManager.hh"

#include "MuTQGlobal.hh"
#include "MuTQConfigs.hh"
#include "CreateMapFromCSV.hh"
#include "MuTQMuGridSD.hh"
#include "MuTQPrimaryGeneratorAction.hh"

#include <algorithm>

MuTQDetectorConstruction::MuTQDetectorConstruction() :
    fLogicalMuGrid(nullptr) {}

G4VPhysicalVolume* MuTQDetectorConstruction::Construct() {
    // Set if check overlaps
    //
    constexpr G4bool checkOverlaps = true;

    auto nist = G4NistManager::Instance();

    // ============================================================================
    // Solid terrain
    // ============================================================================

    auto solidTerrain = ConstructTerrain(
        gTerrainPointFileName,
        gTerrainMeshDefFileName,
        gTerrainBoundaryFileName
    );

    // ============================================================================
    // Solid tunnel
    // ============================================================================

    G4double groundToCentre = gTunnelRadius * sin(gTunnelAngle * 0.5 - 90 * deg);
    auto solidTunnel = new G4SubtractionSolid(
        gTunnelName,
        new G4Tubs(
            gTempName,
            0.0,
            gTunnelRadius,
            gTunnelLength * 0.5,
            0.0,
            2 * M_PI
        ),
        new G4Box(
            gTempName,
            gTunnelRadius,
            gTunnelRadius,
            gTunnelLength
        ),
        nullptr,
        G4ThreeVector(-gTunnelRadius - groundToCentre)
    );

    // ============================================================================
    // Solid terrain - tunnel
    // ============================================================================

    G4double slopeAngle = atan(gTunnelGrad);
    G4RotationMatrix flip(
        G4ThreeVector(0.0, 1.0, 0.0),
        -90 * deg - slopeAngle
    );
    G4ThreeVector correct(
        0.5 * gTunnelLength * (cos(slopeAngle) - 1.0),
        0.0,
        0.5 * gTunnelLength * sin(slopeAngle)
    );
    G4ThreeVector move(
        gTunnelEntranceX + 0.5 * gTunnelLength,
        0.0,
        gTunnelEntranceAltitude + groundToCentre
    );
    auto solidTerrainWithTunnel = new G4SubtractionSolid(
        gTerrainName + "-" + gTunnelName,
        solidTerrain,
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
    auto worldMaterial = nist->FindOrBuildMaterial(gWorldMaterialName);

    // world construction

    auto solidWorld = new G4Box(
        gWorldName,
        solidTerrain->GetMaxXExtent() + 1 * mm,
        solidTerrain->GetMaxYExtent() + 1 * mm,
        solidTerrain->GetMaxZExtent() + 1 * mm
    );
    auto logicalWorld = new G4LogicalVolume(
        solidWorld,
        worldMaterial,
        gWorldName
    );
    auto physicalWorld = new G4PVPlacement(
        G4Transform3D(),
        gWorldName,
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
        gTerrainMaterialName,
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
        solidTerrainWithTunnel,
        terrainMaterial,
        gTerrainName + "-" + gTunnelName
    );
    new G4PVPlacement(
        G4Transform3D(),
        gTerrainName + "-" + gTunnelName,
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

    // MuGrid construction

    auto solidMuGrid = new G4Box(
        gMuGridName,
        gMuGridHalfSize[0],
        gMuGridHalfSize[1],
        gMuGridHalfSize[2]
    );
    fLogicalMuGrid = new G4LogicalVolume(
        solidMuGrid,
        MuGridMaterial,
        gMuGridName
    );

    G4ThreeVector MuGridOutOfTunnelSection(
        gTunnelEntranceX, 0, gTunnelEntranceAltitude + gMuGridHalfSize[2] + gMuGridHeight);
    G4ThreeVector MuGridInsideOfTunnelSection(
        gMuGridPosition[0] - gTunnelEntranceX, gMuGridPosition[1]);
    MuGridInsideOfTunnelSection.rotateY(-slopeAngle);
    G4ThreeVector MuGridAbsolutePosition(MuGridOutOfTunnelSection + MuGridInsideOfTunnelSection);

    new G4PVPlacement(
        G4Transform3D(
            G4RotationMatrix(),
            MuGridAbsolutePosition
        ),
        gMuGridName,
        fLogicalMuGrid,
        physicalWorld,
        false, 0,
        checkOverlaps
    );

    MuTQPrimaryGeneratorAction::fMuGridPosition = std::move(MuGridAbsolutePosition);
    MuTQPrimaryGeneratorAction::fSphereRadius = solidTerrain->GetMaxZExtent();

    return physicalWorld;
}

void MuTQDetectorConstruction::ConstructSDandField() {
    if (gRunningInBatch) {
        auto SDManager = G4SDManager::GetSDMpointer();
        auto MuGridSD = new MuTQMuGridSD(gMuGridName);
        SDManager->AddNewDetector(MuGridSD);
        SetSensitiveDetector(fLogicalMuGrid, MuGridSD);
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

    G4TessellatedSolid* terrain = new G4TessellatedSolid(gTerrainName);

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
