#include <list>
#include <sstream>
#include <ctime>
#include "g4analysis.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"
#include "MuTQAnalysisManager.hh"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: MuTQMergeParallelRun [NameOfROOTFileToBeMerged] [NumberOfRun]" << std::endl;
    }
    std::string outputName(argv[1]);
    if (std::system(("mkdir " + outputName + " >/dev/null 2>&1").c_str()) == 0) {
        std::system(("rm -r " + outputName).c_str());
        std::cerr << "Error: <" << outputName << "> does not exist." << std::endl;
        return 1;
    } else {
        std::cout << "Directory <" << outputName << "> found." << std::endl;
    }
    uint32_t numberOfRun = 0;
    std::stringstream(argv[2]) >> numberOfRun;
    std::list<std::string> partNameList;
    for (uint32_t i = 0; i < numberOfRun; ++i) {
        std::stringstream ss;
        std::string partName;
        ss << outputName << '/' << outputName << "_part_" << i;
        ss >> partName;
        partNameList.push_back(partName);
    }
/* 
    std::cout << "Press any key to start verifying and (waiting for) merge process ..." << std::endl;
    std::cout << "Warning: If some run has just started execution, "
        << "please make sure that all root files generated by run already exist "
        << "before press any key to continue. Otherwise, some root file may be lost." << std::endl;
    std::cin.get();

    std::cout << "Verifying root file name ... ";
    for (auto partName = partNameList.begin(); partName != partNameList.end(); ++partName) {
        if (std::system(("mkdir " + *partName + ".root >/dev/null 2>&1").c_str()) == 0) {
            std::system(("rm -r " + *partName + ".root").c_str());
            std::cerr << "Warning: <" << *partName << ".root> does not exist." << std::endl;
            partNameList.erase(partName);
        }
    }
    std::cout << "done." << std::endl;
 */
    std::cout << "Now wait for the calculation to end and all root files are unlocked ..." << std::endl;
    std::system("sleep 1");
    bool isWaiting = true;
    while (isWaiting) {
        for (auto partName = partNameList.begin(); partName != partNameList.end(); ++partName) {
            if (std::system(("mkdir " + *partName + ".lock >/dev/null 2>&1").c_str()) == 0) {
                std::system(("rm -r " + *partName + ".lock").c_str());
                partName++;
                if (partName == partNameList.end()) { isWaiting = false; }
                partName--;
            } else {
                break;
            }
        }
        if (isWaiting) {
            time_t currentTime = time(nullptr);
            std::cout << ctime(&currentTime) << ">> Waiting ..." << std::endl;
            std::system("sleep 10");
        }
    }
    std::cout << "All locks have been removed, ready to merge." << std::endl;

    std::cout << "Start merging ..." << std::endl;
    std::array<G4String, 9> ntupleName = {
        "GeneratedEvent",
        "HitEvent",
        "ReconstructableEvent",
        "UnreconstructableEvent",
        "Hit2Event",
        "Hit3Event",
        "Hit4Event",
        "Hit5Event",
        "Hit6Event"
    };
    MuTQAnalysisManager::SetRootFileName(outputName.c_str());
    auto writer = new MuTQAnalysisManager();
    writer->Initialize();
    writer->Open();
    for (const auto& partName : partNameList) {
        std::cout << "Start to merge " << partName << ".root to " << outputName << ".root ... ";
        for (size_t ntupleID = 0; ntupleID < ntupleName.size(); ++ntupleID) {
            G4AnalysisReader* reader = G4AnalysisReader::Instance();
            reader->SetFileName(partName.c_str());
            reader->GetNtuple(ntupleName[ntupleID]);
            G4float energy, phi, theta;
            reader->SetNtupleFColumn("Energy[GeV]", energy);
            reader->SetNtupleFColumn("AzimuthAngle[rad]", phi);
            reader->SetNtupleFColumn("ZenithAngle[rad]", theta);
            while (reader->GetNtupleRow()) {
                writer->Fill(ntupleID, energy * GeV, phi * rad, theta * rad);
            }
            delete reader;
        }
        std::cout << "done." << std::endl;
    }
    writer->WriteAndClose();
    delete writer;

    std::cout << "Merging completed." << std::endl;

    return 0;
}

