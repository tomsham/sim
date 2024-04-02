#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator(){
	ParticleDefinition();

	// G4ParticleGun(number of particles for 1 event), here set 1 particle for 1 event
	fParticleGun = new G4ParticleGun(1);
	fParticleSource = new G4GeneralParticleSource();

	// Define fIon to be Ground state Na-22
	Z = 11;					// Atomic number (Proton Number)
	A = 22;					// Mass number
	ex_energy = 0.*keV;		// Excitation energy

	// Select particle generator. Options: 0 = fParticleSource, 1 = fParticleGun
	PS_or_PG = 0;

	// Set the default parameters for the fParticleSource
	pdParticleSource = fGeantino;								//options: fGamma, fPositron, fGeantino = fIon
	chargeParticleSource = 0.*eplus;
	fParticleSource->SetParticleCharge(chargeParticleSource);
	fParticleSource->SetParticleDefinition(pdParticleSource);

	// Set the default parameters for the fParticleGun
	pdParticleGun = fGamma;									//options: fGamma, fPositron, fGeantino = fIon, fo_Ps, fp_Ps
	//posParticleGun = G4ThreeVector(0.*cm, 0.*cm, 0.*cm);
	posParticleGun = G4ThreeVector(0.*cm, 0.*cm, 0.*cm-1*mm);
	momDirectionParticleGun = G4ThreeVector(0., 0., 1.);
	kinParticleGun = 1.2746*MeV;
	chargeParticleGun = 0.*eplus;
	fParticleGun->SetParticlePosition(posParticleGun);
	fParticleGun->SetParticleMomentumDirection(momDirectionParticleGun);
	//fParticleGun->SetParticleMomentum(momParticleGun);
	fParticleGun->SetParticleEnergy(kinParticleGun);			//set the KineticEnergy of particle
	fParticleGun->SetParticleCharge(chargeParticleGun);
	fParticleGun->SetParticleDefinition(pdParticleGun);
}

MyPrimaryGenerator::~MyPrimaryGenerator(){
	delete fParticleGun;
	delete fParticleSource;
}

// Define Particle Definition
void MyPrimaryGenerator::ParticleDefinition(){
	fGamma = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
	fPositron = G4ParticleTable::GetParticleTable()->FindParticle("e+");
	fGeantino = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
	fo_Ps = G4OrthoPositronium::Definition();
	fp_Ps = G4ParaPositronium::Definition();
}

//This function is called at the begining of every event
void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent){
	G4ParticleDefinition* pd_ParticleSource = fParticleSource->GetParticleDefinition();
	G4ParticleDefinition* pd_ParticleGun = fParticleGun->GetParticleDefinition();

	if (PS_or_PG == 0) {
		if (pd_ParticleSource == G4Geantino::Geantino()) {
			fIon = G4IonTable::GetIonTable()->GetIon(Z, A, ex_energy);
			fParticleSource->SetParticleDefinition(fIon);

			//fParticleSource->SetVerbosity(2);				// Turn on verbose mode and set the level 2, for debugging
			fParticleSource->GeneratePrimaryVertex(anEvent);
		}else
			fParticleSource->GeneratePrimaryVertex(anEvent);
	}
	else {
		if (pd_ParticleGun == G4Geantino::Geantino()) {
			fIon = G4IonTable::GetIonTable()->GetIon(Z, A, ex_energy);
			fParticleGun->SetParticleDefinition(fIon);
			fParticleGun->GeneratePrimaryVertex(anEvent);
		}else
			fParticleGun->GeneratePrimaryVertex(anEvent);
	}
}
