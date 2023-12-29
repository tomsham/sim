#include "G4ParaPositronium.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4PhysicalConstants.hh"

G4ParaPositronium* G4ParaPositronium::theInstance = 0;

G4ParaPositronium* G4ParaPositronium::Definition() {
	if (theInstance !=0) return theInstance;

	const G4String name = "p-Ps";
	// search in particle table
	G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* anInstance = pTable->FindParticle(name);
	if (anInstance == 0) {
	// create particle
	//	Arguments for constructor are as follows 
	//		name			mass					width			charge
	//		2*spin			parity					C-conjugation
	//		2*Isospin		2*Isospin3				G-parity
	//		type			lepton number			baryon number	PDG encoding
	//		stable			lifetime				decay table 
	//		shortlived		subType					anti_encoding
		anInstance = new G4ParticleDefinition(
			name,			2*electron_mass_c2,		0.0*MeV,		0.0, 
			0,				-1,						1,
			0,				0,						0,
			"boson",		0,						0,				80,
			false,			125.0*ps,				NULL,
			false,			"positronium",			80
		);
	//create Decay Table
	G4DecayTable* table = new G4DecayTable();
	
	// create a decay channel
	G4VDecayChannel* mode;
	
	// p-Ps -> gamma + gamma
	mode = new G4PhaseSpaceDecayChannel(name, 1.0, 2, "gamma", "gamma");
	table->Insert(mode);
	
	anInstance->SetDecayTable(table);
	}
	theInstance = reinterpret_cast<G4ParaPositronium*>(anInstance);
	return theInstance;
}

G4ParaPositronium* G4ParaPositronium::ParaPositroniumDefinition() {
	return Definition();
}

G4ParaPositronium* G4ParaPositronium::ParaPositronium() {
	return Definition();
}