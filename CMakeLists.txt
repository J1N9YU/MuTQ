cmake_minimum_required (VERSION 2.6)
project (MuGridAtTianQin)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(MuGridAtTianQin MuGridAtTianQin.cc ${sources} ${headers})
target_link_libraries(MuGridAtTianQin ${Geant4_LIBRARIES})
add_executable(MuTQStartParallelRun MuTQStartParallelRun.cc)
add_executable(MuTQMergeParallelRun MuTQMergeParallelRun.cc ${sources} ${headers})
target_link_libraries(MuTQMergeParallelRun ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build MuGridAtTianQin. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(SCRIPTS
  ./macros/init_vis.mac
  ./macros/run.mac
  ./macros/vis.mac
  ./datafiles/terrainPoints.csv
  ./datafiles/terrainMeshDef.csv
  ./datafiles/terrainBoundaryPoints.csv
  )

foreach(_script ${SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS MuGridAtTianQin DESTINATION bin)
