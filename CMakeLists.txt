#create a Cmake project, this line tells Cmake which is the minimum Cmake version that required to compile the code
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#set the name of this project
project(Simulation)

#tell Cmake to find Geant4 and use it with UI input and visualization
find_package(Geant4 REQUIRED ui_all vis_all)

#tell Cmake should include the files in this Path
include(${Geant4_USE_FILE})

#tell Cmake compile all the files in the source folder (files type: .cc and .hh)
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

file(GLOB MACRO_FILES
	"*.mac"
)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

#tell Cmake to create executable, sim is the name (output sim.exe), sim.cc is the main file, 
#${sources} and ${headers} are all files that set in line 10 and 11
add_executable(sim sim.cc ${sources} ${headers})

#tell Cmake use the library in "sim" and path ${Geant4_LIBRARIES}
target_link_libraries(sim ${Geant4_LIBRARIES})

#tell Cmake the custom target is this project (Simulation), and run all the commands that is needed
#(This function is not well understood, learn CMake for more information)
add_custom_target(Simulation DEPENDS sim)