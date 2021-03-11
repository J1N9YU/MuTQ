#ifndef MuTQ_ROOT_MANAGER
#define MuTQ_ROOT_MANAGER 1

#include "g4analysis.hh"

class MuTQAnalysisManager {
public:
    static MuTQAnalysisManager& Instance() {
        static MuTQAnalysisManager instance;
        return instance;
    }
private:
    MuTQAnalysisManager();
    ~MuTQAnalysisManager() {}
    MuTQAnalysisManager(const MuTQAnalysisManager&) = delete;
    MuTQAnalysisManager& operator=(const MuTQAnalysisManager&) = delete;

private:
    const char* fRootFileName;

public:
    void Initialize() const;
    void Open() const;
    void Fill(G4int&& id, const G4double& energy, const G4double& phi, const G4double& theta) const;
    void WriteAndClose() const;

    void SetRootFileName(const char* rootFileName) { fRootFileName = rootFileName; }
};

#endif

