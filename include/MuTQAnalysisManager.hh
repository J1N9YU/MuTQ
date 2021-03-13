#ifndef MuTQ_ROOT_MANAGER
#define MuTQ_ROOT_MANAGER 1

#include <string>
#include "g4analysis.hh"

class MuTQAnalysisManager {
private:
    static const char* fRootFileName;

public:
    MuTQAnalysisManager() {}
    ~MuTQAnalysisManager() {}

    void Initialize() const;
    void Open() const;
    void Fill(G4int id, const G4double& energy, const G4double& phi, const G4double& theta) const;
    void WriteAndClose() const;

    static void SetRootFileName(const char* rootFileName) { fRootFileName = rootFileName; }
};

#endif

