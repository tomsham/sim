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


class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	MyPrimaryGenerator();
	~MyPrimaryGenerator();
	virtual void GeneratePrimaries(G4Event*);
	void ParticleDefinition();

private:
	G4ParticleDefinition *fGamma, *fPositron, *fGeantino, *fIon;
	G4ParticleDefinition *pdParticleGun, *pdParticleSource;			// pd = ParticleDefinition
	G4ParticleGun *fParticleGun;
	G4GeneralParticleSource *fParticleSource;
	G4bool PS_or_PG;
	G4ThreeVector posParticleGun, momDirectionParticleGun;
	G4int Z, A;														// Z = Atomic number (Proton Number), A =  Mass number
	G4double ex_energy;												// ex_energy = Excitation energy
	G4double momParticleGun, chargeParticleGun, chargeParticleSource;
};

#endif