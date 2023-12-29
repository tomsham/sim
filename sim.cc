#include <iostream>

#include "globals.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"

#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include "G4GeometryManager.hh"

int main(int argc, char** argv)	// argc = argument count, argv = argument vector, https://www.ibm.com/docs/en/i/7.1?topic=functions-main-function
{
	G4Random::setTheSeed(1);	// Set the random seed for whole simulation

	#ifdef G4MULTITHREADED		//Check Installed Geant4 version
		G4MTRunManager* runManager = new G4MTRunManager();				// Assign Heap (Dynamic Memory) for RunManager
	#else
		G4RunManager* runManager = new G4RunManager();
	#endif

	runManager->SetUserInitialization(new MyDetectorConstruction());	// Set Construction of the RunManager using MyDetectorConstruction
	runManager->SetUserInitialization(new MyPhysicsList());				// Set PhysicsList of the RunManager using MyPhysicsList
	runManager->SetUserInitialization(new MyActionInitialization());	// Set ActionInitialization of the RunManager using MyActionInitialization
	//runManager->Initialize();

	if (argc == 1)
	{
		//runManager->Initialize();
		G4UIExecutive* ui = new G4UIExecutive(argc, argv);

		G4VisManager* visManager = new G4VisExecutive();
		visManager->Initialize();

		G4UImanager* UImanager = G4UImanager::GetUIpointer();
		UImanager->ApplyCommand("/control/execute vis.mac");					// Visualize the Construction of Simulation
		UImanager->ApplyCommand("/control/execute default_GPS_setup.mac");		// Set the energy distribution of GeneralParticleSource(GPS) to be mono and 0*keV
		ui->SessionStart();
	}
	else
	{
		G4UImanager* UImanager = G4UImanager::GetUIpointer();
		G4String command = "/control/execute ";
		G4String filename = argv[1];
		UImanager->ApplyCommand(command + filename);
		G4cout << command + filename << G4endl;
		G4cout << "Random Seed: " << G4Random::getTheSeed() << G4endl;
		printf("Command is executed");
	}

	// Don't know why but it works
	// Reference:https://geant4-forum.web.cern.ch/t/warning-about-deleting-store-while-geometry-closed/5382
	G4GeometryManager::GetInstance()->OpenGeometry();

	return 0;
}