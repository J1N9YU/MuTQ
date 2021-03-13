#include "G4GlobalConfig.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "MuTQDetectorConstruction.hh"
#include "MuTQActionInitialization.hh"
#include "MuTQPhysicsList.hh"
#include "MuTQConfigs.hh"
#include "MuTQAnalysisManager.hh"

#include <sstream>
#include <string>
#include <ctime>

int main(int argc, char** argv) {
    if (argc < 3 || argc > 9) {
        std::cerr
            << "Usage: MuGridAtTianQin [MuGridX(m)] [MuGridY(m)]" << std::endl
            << "Optional: -e [nEvents](will run in batch if set) -r [ROOTFileName] -s [RandomSeed]" << std::endl;
        return 1;
    }

    bool isBatch = false;
    G4int numberOfEvents = 0;
    std::string rootFileName("untitled");
    for (int i = 3; i < argc; ++i) {
        std::string argvStr(argv[i]);
        if (argvStr == "-e") {
            isBatch = true;
            std::stringstream(argv[i + 1]) >> numberOfEvents;
        } else if (argvStr == "-r") {
            rootFileName = argv[i + 1];
            MuTQAnalysisManager::SetRootFileName(argv[i + 1]);
        } else if (argvStr == "-s") {
            long seed = 0;
            std::stringstream(argv[i + 1]) >> seed;
            G4Random::setTheSeed(seed);
        }
    }

    if (std::system(("mkdir " + rootFileName + ".lock >/dev/null 2>&1").c_str()) != 0) {
        std::cerr << '<' << rootFileName + "> is locked, maybe it is running?" << std::endl;
        return 1;
    }

    std::stringstream(argv[1]) >> MuTQDetectorConstruction::fMuGridRelativePosition[0];
    std::stringstream(argv[2]) >> MuTQDetectorConstruction::fMuGridRelativePosition[1];
    MuTQDetectorConstruction::fMuGridRelativePosition[0] *= CLHEP::m;
    MuTQDetectorConstruction::fMuGridRelativePosition[1] *= CLHEP::m;

    G4RunManager* runManager = new G4RunManager();
    runManager->SetUserInitialization(new MuTQDetectorConstruction());
    runManager->SetUserInitialization(new MuTQPhysicsList());
    runManager->SetUserInitialization(new MuTQActionInitialization());

    if (isBatch) {
        runManager->Initialize();
        runManager->BeamOn(numberOfEvents);
    } else {
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        G4VisManager* visManager = new G4VisExecutive();
        G4UImanager* UImanager = G4UImanager::GetUIpointer();

        visManager->Initialize();
        UImanager->ApplyCommand("/control/macroPath macros");
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();

        delete ui;
        delete visManager;
    }

    delete runManager;

    std::system(("rm -r " + rootFileName + ".lock").c_str());

    return 0;
}

