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
	pdParticleGun = fPositron;									//options: fGamma, fPositron, fGeantino = fIon, fo_Ps, fp_Ps
	posParticleGun = G4ThreeVector(0.*cm, 0.*cm, 0.*cm);
	momDirectionParticleGun = G4ThreeVector(0., 0., 1.);
	kinParticleGun = 6.9*eV;
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
	//geantino is a virtual particle in simulation, it juse take a place for further definition, eg. can be replaced in macro file.
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
			/*
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
			*/
			fIon = G4IonTable::GetIonTable()->GetIon(Z, A, ex_energy);
			fParticleGun->SetParticleDefinition(fIon);
			fParticleGun->GeneratePrimaryVertex(anEvent);
		}else
			fParticleGun->GeneratePrimaryVertex(anEvent);
	}
}
