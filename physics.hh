#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessManager.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4IonConstructor.hh"

#include "G4Gamma.hh"
#include "G4Positron.hh"
#include "G4ParaPositronium.hh"
#include "G4OrthoPositronium.hh"

#include "G4eeToPositroniumModel.hh"
#include "G4eeToPositronium.hh"

// G4OpticalPhysics::ConstructProcess() is protected, so using MyOpticalPhysics to inherit from G4OpticalPhysics to call the function ConstructProcess().
class MyOpticalPhysics : public G4OpticalPhysics
{
public:
	MyOpticalPhysics() {};
	~MyOpticalPhysics() {};
	void GetProcess()
	{
		ConstructProcess();
	}
};

class MyPhysicsList : public G4VModularPhysicsList
{
public:
	MyPhysicsList();
	~MyPhysicsList();

	virtual void ConstructParticle();
	virtual void ConstructProcess();
	virtual void SetCuts();

private:
};

#endif