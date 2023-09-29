#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
	// define EM_interaction, more physics include more time to simulate
	RegisterPhysics(new G4EmStandardPhysics());
	// define OpticalPhysics to use Optical Photon
	RegisterPhysics(new G4OpticalPhysics());
	RegisterPhysics(new G4DecayPhysics());
	RegisterPhysics(new G4RadioactiveDecayPhysics());
}

MyPhysicsList::~MyPhysicsList()
{}