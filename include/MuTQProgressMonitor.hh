#ifndef MuTQ_PROGRESS_MANAGER
#define MuTQ_PROGRESS_MANAGER 1

#include <ctime>
#include "globals.hh"

// Class for computing time management and estimation.
//
class MuTQProgressMonitor {
public:
    static MuTQProgressMonitor* Instance();
private:
    static MuTQProgressMonitor* instance;
    MuTQProgressMonitor();
    ~MuTQProgressMonitor() {}
    MuTQProgressMonitor(const MuTQProgressMonitor&) = delete;
    MuTQProgressMonitor& operator=(const MuTQProgressMonitor&) = delete;

private:
    time_t  fRunStartTime;
    time_t  fRunEndTime;
    G4bool  fTimerStarted;
    G4int   fTotalEvents;
    G4int   fProcessedEvents;
    G4int   fEventsPerReport;
    clock_t fCPUTime;

public:
    void SetNumberOfEventsPerReport(G4int n) { fEventsPerReport = n; }
    void StartRun(G4int nEvent);
    void CompleteAnEvent();
    void EndRun();
};

#endif

