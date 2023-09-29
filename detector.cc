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
	//number += 1;
	Eltis(aStep, ROhist, track);

	return 0;
}
// From Eltis, modified by Tom
void MySensentiveDetector::Eltis(G4Step* aStep, G4TouchableHistory* ROhist, G4Track* track)
{
	//Information of the final step
	G4String Particle_type = track->GetParticleDefinition()->GetParticleType();							//Get the particle type
	G4String Particle_Name = track->GetDefinition()->GetParticleName();									//Get the particle name
	G4String postProcessName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();	//Get the process name
	
	G4double ekin = track->GetKineticEnergy();							//Get Kinetic Energy when it hit the detector
	G4double etot = track->GetTotalEnergy();							//Get Total Energy when it hit the detector
	G4ThreeVector posReal = aStep->GetPreStepPoint()->GetPosition();	//Get Position when it hit the detector

    //Information just touch the detector
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4LogicalVolume *logicVol = physVol->GetLogicalVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();
	G4String logicVol_name = logicVol->GetName();
	G4String physVol_name = physVol->GetName();
	G4int physVol_no = physVol->GetCopyNo();
	
	#ifndef G4MULTITHREADED
	G4cout << "Particle type is: " << Particle_type << G4endl;
	G4cout << "Particle Name is:" << Particle_Name << G4endl;
	G4cout << "Process Name is:" << postProcessName << G4endl;
	G4cout << "Kinetic Energy is:" << ekin/MeV << " MeV" << G4endl;
	G4cout << "Total Energy is:" << etot/MeV << " MeV" << G4endl;
	G4cout << "Real position is:" << posReal << " mm" << G4endl;
	G4cout << "Detector position is:" << posDetector << " mm" << G4endl;
	G4cout << "Detector name is:" << physVol_name+"_" << physVol_no << G4endl;
	G4cout << G4endl;
	#endif
    // CsI Detector
    /*if (logicVol_name == "logicDetector")
	{
		G4cout << "Particle Name is:" << Particle_Name << G4endl;
		G4cout << "Process Name is:" << postProcessName << G4endl;
		G4cout << "Total Energy is:" << etot/MeV << " MeV" << G4endl;
		G4cout << "Real position is:" << posReal/mm << " mm" << G4endl;
		G4cout << "Detector name is:" << physVol_name+"_" << physVol_no << G4endl;
		G4cout << G4endl;

		G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
		G4AnalysisManager* man = G4AnalysisManager::Instance();
		man->FillNtupleIColumn(0, 0, evt);
		man->FillNtupleDColumn(0, 1, posReal[0]);
		man->FillNtupleDColumn(0, 2, posReal[1]);
		man->FillNtupleDColumn(0, 3, posReal[2]);
		man->FillNtupleDColumn(0, 4, etot);
		man->FillNtupleDColumn(0, 5, ekin);
		man->AddNtupleRow(0);

		man->FillNtupleIColumn(1, 0, evt);
		man->FillNtupleDColumn(1, 1, posDetector[0]);
		man->FillNtupleDColumn(1, 2, posDetector[1]);
		man->FillNtupleDColumn(1, 3, posDetector[2]);
		man->AddNtupleRow(1);
	}*/
    // End CsI Detector
    // logicDetector_Shell
    /*
	if (logicVol_name == "logicDetector_Shell")
	{
		G4cout << "Particle Name is:" << Particle_Name << G4endl;
		G4cout << "Process Name is:" << postProcessName << G4endl;
		G4cout << "Total Energy is:" << etot/MeV << " MeV" << G4endl;
        G4cout << "Kinetic Energy is:" << ekin/MeV << " MeV" << G4endl;
		G4cout << "Real position is:" << posReal/cm << " cm" << G4endl;
		G4cout << "Detector name is:" << physVol_name << G4endl;
		G4cout << G4endl;
	}
    */
    // End logicDetector_Shell

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    std::string partical_name[] = {"e+", "e-", "gamma", "nu_e", "Ne22", "Ne22[1274.577]", "Na22" };
    int length_str = sizeof(partical_name)/sizeof(std::string); // length_str = 7

    for (int i = 0; i < length_str; i++)
    {
        if (track->GetDefinition()->GetParticleName() == partical_name[i]) {
			G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
            G4ThreeVector vertex = track->GetPosition(); //Position when it pass the detector
            G4double x = vertex.x(), y = vertex.y(), z = vertex.z();
            G4ThreeVector direction = track->GetMomentumDirection();
            G4double theta = direction.theta(), phi = direction.phi();
            G4ThreeVector momentum = track->GetMomentum();
            G4double px = momentum.x(); G4double py = momentum.y(); G4double pz = momentum.z(); //The momentum when it pass the detector
            man->FillNtupleDColumn(i+1, 0, ekin/MeV);	//MeV
            man->FillNtupleDColumn(i+1, 1, etot/MeV);	//MeV
            man->FillNtupleDColumn(i+1, 2, x/mm);		//mm
            man->FillNtupleDColumn(i+1, 3, y/mm);		//mm
            man->FillNtupleDColumn(i+1, 4, z/mm);		//mm
            man->FillNtupleDColumn(i+1, 5, px);
            man->FillNtupleDColumn(i+1, 6, py);
            man->FillNtupleDColumn(i+1, 7, pz);
            man->FillNtupleDColumn(i+1, 8, theta);
            man->FillNtupleDColumn(i+1, 9, phi);
            man->FillNtupleSColumn(i+1, 10, postProcessName);
            man->AddNtupleRow(i+1);
        }
    }
	man->FillNtupleSColumn(0, 0, Particle_Name);
    man->FillNtupleSColumn(0, 1, postProcessName);
    man->FillNtupleDColumn(0, 2, evt);
    man->FillNtupleSColumn(0, 3, Particle_type);
    man->AddNtupleRow(0);
}
// End Eltis