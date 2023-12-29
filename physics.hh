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
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmStandardPhysicsSS.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4VPhysicsConstructor.hh"

//#include "G4LeptonConstructor.hh"
#include "G4Gamma.hh"
#include "G4Positron.hh"
#include "G4ParaPositronium.hh"
#include "G4OrthoPositronium.hh"

#include "G4eeToPositroniumModel.hh"
#include "G4eeToPositronium.hh"

class MyPhysicsList : public G4VModularPhysicsList
{
public:
	MyPhysicsList();
	~MyPhysicsList();

	virtual void ConstructParticle();
	virtual void ConstructProcess();
	//virtual void AddPhysicsList(const G4String& name);
	virtual void SetCuts();

private:
};

#endif