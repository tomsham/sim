#include "detector.hh"

MySensentiveDetector::MySensentiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensentiveDetector::~MySensentiveDetector()
{}

G4bool MySensentiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
	G4Track* track = aStep->GetTrack();
	track->SetTrackStatus(fStopAndKill);
	
	//Cherenkov(aStep, ROhist);

	Eltis(aStep, ROhist, track);

	return 0;
}

// From Eltis
void MySensentiveDetector::Eltis(G4Step* aStep, G4TouchableHistory* ROhist, G4Track* track)
{
	//Information of the final step
	G4String Particle_type = track->GetParticleDefinition()->GetParticleType();
	//Get the name of the particle
	G4String Particle_Name = track->GetDefinition()->GetParticleName(); 
	//The process name
	G4String postProcessName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
	//Kinetic Energy when it pass the detector
	G4double ekin = track->GetKineticEnergy();
	//Total Energy when it pass the detector
	G4double etot = track->GetTotalEnergy();
	G4ThreeVector posReal = aStep->GetPreStepPoint()->GetPosition();
	
	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();
	//Information just touch the detector
//#ifndef G4MULTITHREADED
	G4cout << "Particle type is: " << Particle_type << G4endl;
	G4cout << "Particle Name is:" << Particle_Name << G4endl;
	G4cout << "Process Name is:" << postProcessName << G4endl;
	G4cout << "Kinetic Energy is:" << ekin << G4endl;
	G4cout << "Total Energy is:" << etot << G4endl;
	G4cout << "Real position is:" << posReal << G4endl;
	G4cout << "Detector position is:" << posDetector << G4endl;
	G4cout << G4endl;
//#endif
}
// End Eltis

void MySensentiveDetector::Cherenkov(G4Step* aStep, G4TouchableHistory* ROhist)
{
	G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

	// Monte Carlo particle Posistion
	G4ThreeVector posPhoton = preStepPoint->GetPosition();
	G4ThreeVector momPhoton = preStepPoint->GetMomentum();
	// wavelength in nm
	G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;
	G4cout << "Photon position: " << posPhoton << G4endl;

	const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();

	// G4int copyNo = touchable->GetCopyNumber();
	// G4cout << "Copy Number: " << copyNo << G4endl;

	G4VPhysicalVolume* physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();
	G4cout << "Detector position: " << posDetector << G4endl;

	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

	// NtupleColumn created in run.cc, filled in detector.cc
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->FillNtupleIColumn(0, 0, evt);
	man->FillNtupleDColumn(0, 1, posPhoton[0]);
	man->FillNtupleDColumn(0, 2, posPhoton[1]);
	man->FillNtupleDColumn(0, 3, posPhoton[2]);
	man->FillNtupleDColumn(0, 4, wlen);
	man->AddNtupleRow(0);

	man->FillNtupleIColumn(1, 0, evt);
	man->FillNtupleDColumn(1, 1, posDetector[0]);
	man->FillNtupleDColumn(1, 2, posDetector[1]);
	man->FillNtupleDColumn(1, 3, posDetector[2]);
	man->AddNtupleRow(1);
}