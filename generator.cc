#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	// In Geant4, 1 run contains number of events, 1 event can contain number of particles.
	// G4ParticleGun(number of particles for 1 event), here set 1 particle for 1 event
	//fParticleGun = new G4ParticleGun(1);
	fParticleGun = new G4GeneralParticleSource();
	
	//geantino is a virtual particle in simulation, it juse take a place for further definition, eg. can be replaced in macro file.
	//if user do not define the particle, then the radioactive decay will occur
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *particle = particleTable->FindParticle("geantino");
	//G4ParticleDefinition *particle = particleTable->FindParticle("gamma");

	G4ThreeVector pos(0., 0., 0.);
	//G4ThreeVector mom(0., 0., 1.);
	G4ThreeVector mom(0., 0., 0.);

	// Default particle kinematic, Set the parameters for the particle
	//fParticleGun->SetParticleMomentumDirection(mom);	//For G4ParticleGun
	//fParticleGun->SetParticleMomentum(1.*MeV);		//For gamma
	fParticleGun->SetParticleDefinition(particle);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

//This function is called at the begining of every event
void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
	G4ParticleDefinition *Particle = fParticleGun->GetParticleDefinition();
	if (Particle == G4Geantino::Geantino())
	{
		//G4int Z = 27;
		//G4int A = 60;
		G4int Z = 11;
		G4int A = 22;

		G4double charge = 0.*eplus;
		//Excitation energy
		G4double energy = 0.*keV;

		G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);
		fParticleGun->SetParticleDefinition(ion);
		fParticleGun->SetParticleCharge(charge);
	}
	/*
	else
	{
		//4 pi solid angle
		G4double pi  = 3.14159265358979323846;
		G4double cosTheta = 2*G4UniformRand()-1;	//cosTheta = (-1,1), G4UniformRand() = (0,1) = ]0,1[
		G4double phi = 2*pi*G4UniformRand();		//phi = ]0,2pi[
		G4double sinTheta = std::sqrt(1.-cosTheta*cosTheta);
		G4double ux = sinTheta*std::cos(phi), uy = sinTheta*std::sin(phi), uz = cosTheta;
		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));	//For G4ParticleGun
		G4double etot = fParticleGun->GetParticleEnergy();

		G4AnalysisManager* man = G4AnalysisManager::Instance();
		man->FillNtupleDColumn(0, 0, ux);			//cm
		man->FillNtupleDColumn(0, 1, uy);			//cm
		man->FillNtupleDColumn(0, 2, uz);			//cm
		man->FillNtupleDColumn(0, 3, etot/MeV);		//MeV
		man->AddNtupleRow(0);
		//4 pi solid angle
	}
	*/
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
