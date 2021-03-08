#include "G4GlobalConfig.hh"
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "MuTQDetectorConstruction.hh"
#include "MuTQActionInitialization.hh"
#include "MuTQPhysicsList.hh"
#include "MuTQConfigs.hh"
#include "MuTQProgressMonitor.hh"
#include "MuTQExtern.hh"

#include <sstream>
#include <ctime>

bool gRunningInBatch;
double gMuGridPosition[2];
const char* gRootFileName;

int main(int argc, char** argv) {
    // Detect interactive mode (if no arguments) and define UI session
    //
    if (argc < 3 || argc > 5) {
        std::cout << "Usage: MuGridAtTianQin [MuGrid X (m)] [MuGrid Y (m)] ([macro file]) ([root file name])" << std::endl;
        return 1;
    }

    std::stringstream(argv[1]) >> gMuGridPosition[0];
    std::stringstream(argv[2]) >> gMuGridPosition[1];
    gMuGridPosition[0] *= CLHEP::m;
    gMuGridPosition[1] *= CLHEP::m;

    if (argc == 5) {
        gRootFileName = argv[4];
    } else {
        gRootFileName = "output";
    }

    G4UIExecutive* ui = nullptr;
    if (argc == 3) {
        ui = new G4UIExecutive(argc, argv);
        gRunningInBatch = false;
    } else {
        gRunningInBatch = true;
    }

    // Random engine seed.
    //
#if MuTQ_USING_TIME_RANDOM_SEED
    CLHEP::HepRandom::setTheSeed((long)time(nullptr));
#endif

    // Construct the default run manager
    //
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager();
#else
    G4RunManager* runManager = new G4RunManager();
#endif

    // Set mandatory initialization classes
    //
    // Detector construction
    runManager->SetUserInitialization(new MuTQDetectorConstruction());

    // Physics list
    runManager->SetUserInitialization(new MuTQPhysicsList());

    // User action initialization
    runManager->SetUserInitialization(new MuTQActionInitialization());

    // Initialize visualization
    //
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    //
    UImanager->ApplyCommand("/control/macroPath macros");
    if (gRunningInBatch) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[3];
        UImanager->ApplyCommand(command + fileName);
    } else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted 
    // in the main() program !

    delete visManager;
    delete runManager;

    return 0;
}