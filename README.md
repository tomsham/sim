# positronium-sim-project
 
This page is about the simulation project for study positronium.

The main branch of this github repository is the source code.

Some background information:<br>
In Geant4, a simulation operation is called a "Run". Each "Run" contains 1 or more "Event". Each "Event" contains 1 or more "Track". Each "Track" contains 1 or more "Step".<br>
For this project, 1 "Run" create 1 Sodium-22 and it will decay.

In graphical user interface(GUI), you can use commands under @MyDetector branch, descriptions are included.<br>
All the output .root files will store in the root directory of sim.exe

1. action.hh & action.cc<br>
These files are used to initialize all allowed user operations, such as tracking particles or events.

3. construction.hh & construction.cc<br>
These files are used to construct designed physical objects, such as detector or scintillator.

4. detector.hh & detector.cc<br>
These files are used to define operations for detectors taking actions, for example, save the energy of detected particles into an Ntuple .

5. event.hh & event.cc<br>
These files are used to count the energy deposition of detectors from the start to the end of an event.

6. generator.hh & generator.cc<br>
These files are used to create particles at the beginning of an "Event", for example create a particle gun to shot some photons.

7. physics.hh & physics.cc<br>
These files are used to define physical rules to be involved during an "Event", such as radioactive decay.

8. run.hh & run.cc<br>
These files are used to define operations during a "Run", for example, create an Ntuple and a file at the beginning of a "Run" then output Ntuple into the file and close it at the end of a "Run".

9. stepping.hh & stepping.cc<br>
These files are used to define operations for a "Step", for example, output the energy loss of each "Step" of a particle then it allows to count energy deposition of a material for an "Event".

10. sim.cc<br>
This is the main file of the simulation project. It is used to initialize the whole simulation including whether using GUI or using multi-thread mode.

11. eff.dat<br>
This file is for setting the energy response of detector. Is is not used yet.

12. rebuild.mac<br>
This is for rebuilding selected physical volumes when using GUI.

13. region_setup.mac<br>
This is for setting the active region when using GUI.

14. vis.mac<br>
This is for visualization and is called in "sim.cc"

15. run_CsI_t16_100k.mac<br>
This is for running the simulation in batch mode without GUI. It includes "region_setup.mac" and will run the simulation 100k times (create 100k "Run") using 16 threads when it is called.

16. default_GPS_setup.mac<br>
This is for setting the kinetic energy distribution of the G4GeneralParticleSource, which are Mono and 0*keV.

17. CMakeLists.txt & CMakeSettings.json<br>
The "CMakeLists.txt" contains configuration for CMake compiling.<br>
The "CMakeSettings.json" contains information that Visual Studio uses for IntelliSense and to construct the command-line arguments that it passes to CMake for a specified configuration and compiler environment.

18. LICENSE & README.md<br>
License and readme files.
