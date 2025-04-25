#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "run.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4Geantino.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"
#include "G4OrthoPositronium.hh"
#include "G4ParaPositronium.hh"


class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction{
public:
	MyPrimaryGenerator();
	~MyPrimaryGenerator();
	virtual void GeneratePrimaries(G4Event*);
	void ParticleDefinition();

private:
	G4ParticleDefinition *fGamma, *fPositron, *fElectron, *fGeantino, *fIon, *fo_Ps, *fp_Ps;
	// pd = ParticleDefinition
	G4ParticleDefinition *pdParticleGun, *pdParticleSource;
	G4ParticleGun *fParticleGun;
	G4GeneralParticleSource *fParticleSource;
	G4bool PS_or_PG;
	G4ThreeVector posParticleGun, momDirectionParticleGun;

	// Z = Atomic number (Proton Number), A =  Mass number
	G4int Z, A;									
	// ex_energy = Excitation energy
	G4double ex_energy;												
	G4double kinParticleGun, chargeParticleGun, chargeParticleSource;
};

#endif