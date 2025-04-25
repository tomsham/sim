#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{}

MyPhysicsList::~MyPhysicsList()
{}

void MyPhysicsList::ConstructParticle()
{
	G4LeptonConstructor pLeptonConstructor;
	pLeptonConstructor.ConstructParticle();
	G4MesonConstructor pMesonConstructor;
	pMesonConstructor.ConstructParticle();
	G4BaryonConstructor pBaryonConstructor;
	pBaryonConstructor.ConstructParticle();
	G4BosonConstructor  pBosonConstructor;
	pBosonConstructor.ConstructParticle();
	G4ShortLivedConstructor pShortLivedConstructor;
	pShortLivedConstructor.ConstructParticle();  
	G4IonConstructor pIonConstructor;
	pIonConstructor.ConstructParticle();

	G4int ver = 1;
	G4HadronPhysicsFTFP_BERT pHadronPhysicsFTFP_BERT(ver);	// HadronPhysicsFTFP_BERT

	G4ParaPositronium::ParaPositronium();
	G4OrthoPositronium::OrthoPositronium();
}

void MyPhysicsList::ConstructProcess()
{
	// AddTransportation for all particles, similar to command "AddProcess(new Transportation, -1, 0, 0)
	AddTransportation();

	// Add all physics processes in TFTFP_BERT
	G4int ver = 1;
	G4EmStandardPhysics pEM_Opt0(ver);						// EmPhysics_Opt0, including p-Ps, o-Ps
	pEM_Opt0.ConstructProcess();
	G4EmExtraPhysics pEmExtraPhysics(ver);					// Synchroton Radiation & GN Physics
	pEmExtraPhysics.ConstructProcess();
	G4DecayPhysics pDecayPhysics(ver);						// Decays, including p-Ps, o-Ps
	pDecayPhysics.ConstructProcess();
	G4HadronElasticPhysics pHadronElasticPhysics(ver);		// Hadron Elastic Physics
	pHadronElasticPhysics.ConstructProcess();
	G4HadronPhysicsFTFP_BERT pHadronPhysicsFTFP_BERT(ver);	// HadronPhysicsFTFP_BERT
	pHadronPhysicsFTFP_BERT.ConstructProcess();
	G4StoppingPhysics pStoppingPhysics(ver);				// Stopping Physics
	pStoppingPhysics.ConstructProcess();
	G4IonPhysics pIonPhysics(ver);							// Ion Physics
	pIonPhysics.ConstructProcess();
	G4NeutronTrackingCut pNeutronTrackingCut(ver);			// Neutron Tracking Cut
	pNeutronTrackingCut.ConstructProcess();

	// Required Physics for this Project
	G4RadioactiveDecayPhysics pRadioactiveDecayPhysics;		// RadioactiveDecayPhysics
	pRadioactiveDecayPhysics.ConstructProcess();
	MyOpticalPhysics pMyOpticalPhysics;						// OpticalPhysics
	pMyOpticalPhysics.GetProcess();
	
	// Assign allowed process for positron
	G4ProcessManager *positronManager = G4Positron::Positron()->GetProcessManager();
	// Remove the G4eplusAnnihilation, the number 4 is from the order in G4EmStandardPhysics
	positronManager->RemoveProcess(4);
	//positronManager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
	//positronManager->AddProcess(new G4eIonisation,         -1, 2, 2);
	//positronManager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);
	positronManager->AddProcess(new G4eeToPositronium,      0,-1, 4);
	//positronManager->DumpInfo();		// Output to check the process list of Positron
}

void MyPhysicsList::SetCuts()
{
	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(0.*eV, 100.*GeV);
	//SetCutsWithDefault();		// default cut value  (1.0mm)
}