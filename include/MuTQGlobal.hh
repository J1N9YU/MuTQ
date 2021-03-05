#ifndef MuTQ_GLOBAL_H
#define MuTQ_GLOBAL_H 1

#include <utility>
#include <cmath>
#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"

//
// Batch flag

extern G4bool gRunningInBatch;

//
// Detector construction

// World

static const G4String gWorldName("world");
static const G4String gWorldMaterialName("G4_AIR");

// Terrian

static const G4String gTerrainName("terrian");
static const G4String gTerrainMaterialName("granite");
static const std::string gTerrainBoundaryFileName("datafiles/terrainBoundaryPoints.csv");
static const std::string gTerrainPointFileName("datafiles/terrainPoints.csv");
static const std::string gTerrainMeshDefFileName("datafiles/terrainMeshDef.csv");

// Tunnel

static const G4String gTunnelName("tunnel");
constexpr G4double gTunnelRadius = 8.0 * m;
constexpr G4double gTunnelAngle = 204 * deg;
constexpr G4double gTunnelGrad = 0.003;
constexpr G4double gTunnelEntranceX = 55 * m;
constexpr G4double gTunnelEntranceAltitude = 10.982 * m;
constexpr G4double gTunnelLength = 529.389 * m - gTunnelEntranceX;

// MuGrid

static const G4String gMuGridName("MuGrid");
extern G4double gMuGridPosition[2];
constexpr G4double gMuGridHeight = 50 * cm;
constexpr G4double gMuGridHalfSize[3] = { 25 * cm, 25 * cm, 25 * cm };

// temp object

static const G4String gTempName("_tmp");

//
// Analysis & file io

static const G4String gRootFileName("MuGridAtTianQin");

#endif

