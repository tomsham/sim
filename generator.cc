#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    //number of particle per event
	//Now one particle per event
    //fParticleGun1 = new G4ParticleGun(1);
    fParticleGun2 = new G4GeneralParticleSource();
    
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    //G4String particleName = "proton";
    //G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
    G4ParticleDefinition *particle = particleTable->FindParticle("geantino");
    
    G4ThreeVector mom(0.,0.,0.);
    //set particle initial position,momentum
    G4ThreeVector pos(0.,0.,0.);
    //fParticleGun2->SetParticlePosition(pos);
    fParticleGun2->SetParticleDefinition(particle);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun1;
    delete fParticleGun2;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleDefinition* particle = fParticleGun2->GetParticleDefinition();
    if (particle == G4Geantino::Geantino()) {
        G4int Z = 11;
        G4int A = 22;
        G4double Q = 0.*eplus;
        //Excitation energy
        G4double E = 0.*keV;
        G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z, A, E);
        fParticleGun2->SetParticleDefinition(ion);
        fParticleGun2->SetParticleCharge(Q);
        fParticleGun2->GeneratePrimaryVertex(anEvent);
    }
    fParticleGun2->GeneratePrimaryVertex(anEvent);
}
