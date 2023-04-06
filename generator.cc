#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	// In Geant4, 1 run can contain number of events, 1 event can contain number of particles.
	// G4ParticleGun(number of particles for 1 event), here set 1 particle for 1 event
	fParticleGun = new G4ParticleGun(1);

	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	// geantino is a virtual particle in simulation, it juse take a place for further definition, eg. can be replaced in macro file.
	// if user do not define the particle, then the radioactive decay will occur
	G4ParticleDefinition* particle = particleTable->FindParticle("geantino");
	//G4ParticleDefinition* particle = particleTable->FindParticle("proton");

	// Define position vector and momentum direction vector
	G4ThreeVector pos(0., 0., 0.);
	G4ThreeVector mom(0., 0., 1.);

	// Set the parameters for the particle
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(0.*GeV);
	//fParticleGun->SetParticleMomentum(100.*GeV);
	fParticleGun->SetParticleDefinition(particle);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
	G4ParticleDefinition* particle = fParticleGun->GetParticleDefinition();

	if (particle == G4Geantino::Geantino())
	{
		//G4int Z = 27;
		//G4int A = 60;
		G4int Z = 11;
		G4int A = 22;

		G4double charge = 0.*eplus;
		G4double energy = 0.*keV;

		G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);
		fParticleGun->SetParticleDefinition(ion);
		fParticleGun->SetParticleCharge(charge);
	}

	fParticleGun->GeneratePrimaryVertex(anEvent);
}