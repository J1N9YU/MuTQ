#ifndef MuTQ_CONFIGS_H
#define MuTQ_CONFIGS_H 1

// Compile-time executed feature settings.
// false = Disable, true = enable.

// Using time for random seed if enabled.
#ifndef MuTQ_USING_TIME_RANDOM_SEED
#define MuTQ_USING_TIME_RANDOM_SEED       false
#endif

// Kill gamma if enabled.
#ifndef MuTQ_KILL_GAMMA
#define MuTQ_KILL_GAMMA                   true
#endif

// Kill electron if enabled.
#ifndef MuTQ_KILL_ELECTRON
#define MuTQ_KILL_ELECTRON                true
#endif

// Using Reyna energy spectrum if enabled.
#ifndef MuTQ_USING_REYNA
#define MuTQ_USING_REYNA                  false
#endif

// Using Tang energy spectrum if enabled.
#ifndef MuTQ_USING_TANG
#define MuTQ_USING_TANG                   true
#endif

#endif

