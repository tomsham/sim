run1.mac:
description: The macro file that simulate the point source decay,
which is used in detecting the decay information at or near the origin

run2.mac:
description: The macro file that simulate the Real experiment.

Remark 1: Comment or uncomment line 40 in sim.cc to visualize the simulation
Remark 2: No visualization allowed when using run1.mac or run2.mac, it crashed.
Remark 3: Every time you finish the simulation, you need to close the simulation and rerun it again to start another simulation

Bekiw refering construction.cc
Remark 4: create the detector to detect energy by setting true in line 26
Remark 5: create the source geometry by setting true in line 25
Remark 5: set EnergyCheck be false in line 107 if you do the real experiment(run2.mac)
Remark 6: Set para in line 112 to 0 if you want to detect the point source energy At the center