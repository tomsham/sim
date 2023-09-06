#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
    //Define physicslist
    // EM interaction
    RegisterPhysics (new G4EmStandardPhysics());
    // Optical Photon
    //RegisterPhysics (new G4OpticalPhysics());
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());
}

MyPhysicsList::~MyPhysicsList()
{}
