#include "MuTQProgressMonitor.hh"

#include <mutex>

MuTQProgressMonitor* MuTQProgressMonitor::instance = nullptr;

MuTQProgressMonitor* MuTQProgressMonitor::Instance() {
    if (!instance) {
        instance = new MuTQProgressMonitor();
    }
    return instance;
}

MuTQProgressMonitor::MuTQProgressMonitor() :
    fRunStartTime(0),
    fRunEndTime(0),
    fTimerStarted(false),
    fTotalEvents(0),
    fProcessedEvents(0),
    fEventsPerReport(100),
    fCPUTime(0) {}

void MuTQProgressMonitor::StartRun(G4int nEvent) {
    if (!fTimerStarted) {
        time(&fRunStartTime);
        fCPUTime = clock();
        fTimerStarted = true;
        fTotalEvents = nEvent;
        fProcessedEvents = 0;
    }
}

#ifdef G4MULTITHREADED
std::mutex mtx;
#endif

void MuTQProgressMonitor::CompleteAnEvent() {
    if (!fTimerStarted) { return; }

    time_t currentTime = time(nullptr);

#ifdef G4MULTITHREADED
    mtx.lock();
#endif
    ++fProcessedEvents;
    G4int lProcessedEvents = fProcessedEvents;
#ifdef G4MULTITHREADED
    mtx.unlock();
#endif

    G4float avgTimeForOneEvent = ((G4float)(currentTime - fRunStartTime)) / lProcessedEvents;
    time_t estTimeRemain = ceilf(avgTimeForOneEvent * (fTotalEvents - lProcessedEvents));

    if (lProcessedEvents % fEventsPerReport == 0) {
        G4cout << ctime(&currentTime)
            << lProcessedEvents << '/' << fTotalEvents << " events processed ("
            << (100.0 * lProcessedEvents) / fTotalEvents << "%). "
            << "Estimated time remaining: " << estTimeRemain << 's'
            << G4endl;
    }
}

void MuTQProgressMonitor::EndRun() {
    if (!fTimerStarted || fProcessedEvents < fTotalEvents) { return; }
    time(&fRunEndTime);
    fCPUTime = clock() - fCPUTime;
    fTimerStarted = false;
    G4cout << '\n'
        << "  Start time: " << ctime(&fRunStartTime)
        << "    End time: " << ctime(&fRunEndTime)
        << "Elapsed time: " << difftime(fRunEndTime, fRunStartTime) << "s\n"
        << "    CPU time: " << fCPUTime / CLOCKS_PER_SEC << 's'
        << G4endl;
}

