#include "physics.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

MyPhysicsList::MyPhysicsList() {
	//// define EM_interaction, more physics include more time to simulate
	//RegisterPhysics(new G4EmStandardPhysics());
	//// define OpticalPhysics to use Optical Photon
	//RegisterPhysics(new G4OpticalPhysics());
	//RegisterPhysics(new G4DecayPhysics());
	//RegisterPhysics(new G4RadioactiveDecayPhysics());
}

MyPhysicsList::~MyPhysicsList()
{}

void MyPhysicsList::ConstructParticle()
{
	/*
	G4LeptonConstructor leptonConstructor;
	leptonConstructor.ConstructParticle();
	G4BosonConstructor bosonConstructor;
	leptonConstructor.ConstructParticle();
	G4IonConstructor ionConstructor;
	ionConstructor.ConstructParticle();

	G4MesonConstructor mesonConstructor;
	mesonConstructor.ConstructParticle();
	G4ExcitedMesonConstructor excitedMesonConstructor;
	excitedMesonConstructor.ConstructParticle();

	G4BaryonConstructor baryonConstructor;
	baryonConstructor.ConstructParticle();
	G4ExcitedBaryonConstructor excitedBaryonConstructor;
	excitedBaryonConstructor.ConstructParticle();
	*/
    G4Gamma::Gamma();
	G4Positron::Positron();
	G4ParaPositronium::ParaPositronium();
	G4OrthoPositronium::OrthoPositronium();
}

void MyPhysicsList::ConstructProcess()
{
	// Add to all particles similar to command "AddProcess(new Transportation, -1, 0, 0)
	AddTransportation();	
	
	// Assign allowed process for positron
	G4ProcessManager *positronManager = G4Positron::Positron()->GetProcessManager();
	positronManager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
	positronManager->AddProcess(new G4eIonisation,         -1, 2, 2);
	positronManager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);
	//positronManager->AddProcess(new G4eplusAnnihilation,    0,-1, 4);
	//positronManager->AddProcess(new G4eeToPositronium,     -1,-1, 4);
	positronManager->AddProcess(new G4eeToPositronium,      0,-1, 4);

	// Assign allowed process for para-positronium
	G4ProcessManager *pPsManager = G4ParaPositronium::ParaPositronium()->GetProcessManager();
	pPsManager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
	pPsManager->AddProcess(new G4eIonisation,         -1, 2, 2);
	pPsManager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);
	pPsManager->AddProcess(new G4Decay,                0,-1, 4);

	// Assign allowed process for ortho-positronium
	G4ProcessManager *oPsManager = G4OrthoPositronium::OrthoPositronium()->GetProcessManager();
	oPsManager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
	oPsManager->AddProcess(new G4eIonisation,         -1, 2, 2);
	oPsManager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);
	oPsManager->AddProcess(new G4Decay,                0,-1, 4);
}

//void MyPhysicsList::AddPhysicsList(const G4String& name)
//{
//    //G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
//    if (name == fEmName) { 
//        return; 
//
//    } else if (name == "emstandard_opt0") {
//        fEmName = name;
//        delete fEmPhysicsList;
//        fEmPhysicsList = new G4EmStandardPhysics();
//
//    } else if (name == "emstandard_opt1") {
//        fEmName = name;
//        delete fEmPhysicsList;
//        fEmPhysicsList = new G4EmStandardPhysics_option1();
//
//    } else if (name == "emstandard_opt2") {
//        fEmName = name;
//        delete fEmPhysicsList;
//        fEmPhysicsList = new G4EmStandardPhysics_option2();
//
//    } else if (name == "emstandard_opt3") {
//        fEmName = name;
//        delete fEmPhysicsList;
//        fEmPhysicsList = new G4EmStandardPhysics_option3();
//
//    } else if (name == "emstandard_opt4") {
//        fEmName = name;
//        delete fEmPhysicsList;
//        fEmPhysicsList = new G4EmStandardPhysics_option4();
//
//    } else if (name == "emstandardWVI") {
//        fEmName = name;
//        delete fEmPhysicsList;
//        fEmPhysicsList = new G4EmStandardPhysicsWVI();
//
//    } else if (name == "emstandardSS") {
//        fEmName = name;
//        delete fEmPhysicsList;
//        fEmPhysicsList = new G4EmStandardPhysicsSS();
//
//    } else if (name == "emstandardGS") {
//        fEmName = name;
//        delete fEmPhysicsList;
//
//        fEmPhysicsList = new G4EmStandardPhysicsGS();
//
//    } else if (name == "emlivermore") {
//        fEmName = name;    
//        delete fEmPhysicsList;
//        fEmPhysicsList = new G4EmLivermorePhysics();
//
//    } else if (name == "empenelope") {
//        fEmName = name;
//        delete fEmPhysicsList;    
//        fEmPhysicsList = new G4EmPenelopePhysics();
//
//    } else if (name == "emlowenergy") {
//        fEmName = name;
//        delete fEmPhysicsList;    
//        fEmPhysicsList = new G4EmLowEPPhysics();
//
//    } else {
//        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
//            << " is not defined"
//            << G4endl;
//        return;
//    }
//}

void MyPhysicsList::SetCuts()
{
	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(0.*eV, 100.*GeV);
	
	//SetCutsWithDefault();		// default cut value  (1.0mm)
}

