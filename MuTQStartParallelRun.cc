#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 5 || argc > 12) {
        std::cout
            << "Usage: MuTQStartParallelRun [MuGridX(m)] [MuGridY(m)] -e [nEvents]" << std::endl
            << "Optional: -t [nThreads] -r [ROOTFileName] -s [RandomSeedForFirstTask] -noautomerge -override"
            << std::endl;
        return 1;
    }

    uint32_t numberOfEvents = 0;
    uint32_t numberOfRun = std::thread::hardware_concurrency();
    std::string outputName = "untitled";
    long firstSeed = 1;
    bool autoMerge = true;
    bool overRide = false;
    for (int i = 3; i < argc; ++i) {
        std::string argvStr(argv[i]);
        if (argvStr == "-e") {
            std::stringstream(argv[i + 1]) >> numberOfEvents;
        } else if (argvStr == "-t") {
            std::stringstream(argv[i + 1]) >> numberOfRun;
        } else if (argvStr == "-r") {
            outputName = argv[i + 1];
        } else if (argvStr == "-s") {
            std::stringstream(argv[i + 1]) >> firstSeed;
        } else if (argvStr == "-noautomerge") {
            autoMerge = false;
        } else if (argvStr == "-override") {
            overRide = true;
        }
    }

    uint32_t localNEvent = numberOfEvents / numberOfRun;
    uint32_t nEventResidues = numberOfEvents % numberOfRun;
    std::vector<uint32_t> nEventList(numberOfRun, localNEvent);
    for (uint32_t i = 0; i < nEventResidues; ++i) {
        ++nEventList[i];
    }

    std::vector<std::string> rootFileNameList(numberOfRun);
    for (uint32_t i = 0; i < numberOfRun; ++i) {
        std::stringstream ss;
        ss << outputName << '/' << outputName << "_part_" << i;
        ss >> rootFileNameList[i];
    }

    std::vector<long> seedList(numberOfRun, firstSeed);
    for (uint32_t i = 0; i < numberOfRun; ++i) {
        seedList[i] += i;
    }

    if (std::system(("mkdir " + outputName + " >/dev/null 2>&1").c_str()) != 0) {
        if (overRide) {
            std::cout << "Warning: <" << outputName << "> is being override." << std::endl;
        } else {
            std::cerr << "Error: <" << outputName << "> is already exist. Try another name, or make sure <"
                << outputName << "> folder is not in use and delete or rename it or use -override flag." << std::endl;
            return 1;
        }
    }

    std::string batchFileName = outputName + "/.MuTQ_parallel.sh";
    std::ofstream fout(batchFileName);
    if (fout.is_open()) {
        std::cout << "Parallel script has been created." << std::endl;
    } else {
        std::cerr << "Error: Cannot create batch file. Execution terminated." << std::endl;
        return 1;
    }
    for (uint32_t i = 0; i < numberOfRun; ++i) {
        std::stringstream ss;
        std::string nEventStr;
        ss << nEventList[i];
        ss >> nEventStr;
        std::string seedStr;
        ss.clear();
        ss << seedList[i];
        ss >> seedStr;
        std::string exe =
            "gnome-terminal --tab -t \"" + (std::string)rootFileNameList[i] + "\" -- \"./MuGridAtTianQin\" \""
            + (std::string)argv[1] + "\" \"" + (std::string)argv[2] + "\" \"-e\" \"" + nEventStr + "\" "
            + "\"-r\" \"" + rootFileNameList[i] + "\" \"-s\" \"" + seedStr + "\"";
        fout << exe << std::endl;
    }
    fout.close();
    
    std::cout << "Parallel script is going to be executed." << std::endl;
    std::system(("sh " + batchFileName).c_str());
    std::cout << "Parallel script is going to be deleted." << std::endl;
    std::system(("rm " + batchFileName).c_str());
    if (autoMerge) {
        std::string numberOfRunStr;
        std::stringstream ss;
        ss << numberOfRun;
        ss >> numberOfRunStr;
        std::cout << "Parallel run merge manager is going to be activated." << std::endl;
        std::system(("./MuTQMergeParallelRun " + outputName + ' ' + numberOfRunStr).c_str());
    }

    return 0;
}