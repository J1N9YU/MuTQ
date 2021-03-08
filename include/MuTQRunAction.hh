#ifndef MuTQ_RUN_ACTION_H
#define MuTQ_RUN_ACTION_H 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "g4analysis.hh"

class MuTQRunAction : public G4UserRunAction {
private:
    G4ToolsAnalysisManager* fAnalysisManager;

public:
    MuTQRunAction();
    virtual ~MuTQRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

private:
    void CreateTreeAndHistrogram() const;
};

#endif

