#include "detector.hh"

MySensentiveDetector::MySensentiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensentiveDetector::~MySensentiveDetector()
{}

G4bool MySensentiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
	/*
	G4Track* track = aStep->GetTrack();
	track->SetTrackStatus(fStopAndKill);
	*/

	//ReadOut(aStep, track);
	//SaveToDataFile(aStep, ROhist, track);

	/*
	G4String detector_Name = track->GetTouchable()->GetVolume()->GetName();
	if (detector_Name == "Detector_Cylinder") {
		SaveToDataFile(aStep, ROhist, track);
		track->SetTrackStatus(fStopAndKill);
	}
	if (detector_Name == "Detector_Cylinder_Ends") {
		SaveToDataFile(aStep, ROhist, track);
		track->SetTrackStatus(fStopAndKill);
	}
	*/
	return 0;
}

// This function store information to a Ntuple then it can be saved in run.cc
void MySensentiveDetector::SaveToDataFile(G4Step* aStep, G4TouchableHistory* ROhist, G4Track* track){
    G4AnalysisManager *man = G4AnalysisManager::Instance();
	G4String particle_type = track->GetParticleDefinition()->GetParticleType();						//Get the particle type
	G4String particle_name = track->GetDefinition()->GetParticleName();								//Get the particle name
	G4String detector_Name = track->GetTouchable()->GetVolume()->GetName();
	G4String creator_process_name = "NULL";
	if (track->GetCreatorProcess())
		creator_process_name = track->GetCreatorProcess()->GetProcessName();				//Get the process name of the vertex of track 

    std::string partical_name_list[] = {"e+", "e-", "gamma", "nu_e" };
    int length_partical_name_list = sizeof(partical_name_list)/sizeof(std::string);					// length_str = 7

    for (int i = 0; i < length_partical_name_list; i++)
    {
        if (particle_name == partical_name_list[i]) {
			G4double ekin = track->GetKineticEnergy();												//Get Kinetic Energy when it hit the detector
			G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
            G4ThreeVector prePosition = preStepPoint->GetPosition();								//Position when it pass the detector
            G4double x = prePosition.x(), y = prePosition.y(), z = prePosition.z();
            G4ThreeVector direction = preStepPoint->GetMomentumDirection();
            G4double theta = direction.theta(), phi = direction.phi(), costheta = direction.cosTheta();
            G4ThreeVector momentum = preStepPoint->GetMomentum();
            G4double px = momentum.x(); G4double py = momentum.y(); G4double pz = momentum.z();		//The momentum when it pass the detector
            man->FillNtupleDColumn(i+1, 0, ekin/MeV);	//MeV
            man->FillNtupleDColumn(i+1, 1, x/mm);		//mm
            man->FillNtupleDColumn(i+1, 2, y/mm);		//mm
            man->FillNtupleDColumn(i+1, 3, z/mm);		//mm
            man->FillNtupleDColumn(i+1, 4, px/MeV);		//MeV/c
            man->FillNtupleDColumn(i+1, 5, py/MeV);		//MeV/c
            man->FillNtupleDColumn(i+1, 6, pz/MeV);		//MeV/c
            man->FillNtupleDColumn(i+1, 7, theta);
			man->FillNtupleDColumn(i+1, 8, costheta);
            man->FillNtupleDColumn(i+1, 9, phi);
            man->FillNtupleSColumn(i+1, 10, creator_process_name);
            man->FillNtupleSColumn(i+1, 11, detector_Name);
            man->AddNtupleRow(i+1);
        }
    }
	man->FillNtupleSColumn(0, 0, particle_name);
    man->FillNtupleSColumn(0, 1, particle_type);
    man->FillNtupleSColumn(0, 2, creator_process_name);
    man->AddNtupleRow(0);
}

// Output Information just touch the detector
void MySensentiveDetector::ReadOut(G4Step* step, G4Track* track) {

	G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	G4int trackID = track->GetTrackID();
	G4int stepID = track->GetCurrentStepNumber();
	G4String particle_name = track->GetDefinition()->GetParticleName();
	G4String creator_process_name = "NULL";
	G4String physVol_name = track->GetTouchable()->GetVolume()->GetName();
	G4ThreeVector postDetectorPosition = track->GetTouchable()->GetVolume()->GetTranslation();

	G4StepPoint* poststep = step->GetPostStepPoint();
	G4ThreeVector postPosition = poststep->GetPosition();
	G4double postKE = poststep->GetKineticEnergy();

	// Get the process name of the vertex of that particle
	if (track->GetCreatorProcess())
		creator_process_name = track->GetCreatorProcess()->GetProcessName();

	G4cout << "----------" << G4endl;
	G4cout << "Particle : " << particle_name << G4endl;
	G4cout << "stepID : " << stepID << G4endl;
	G4cout << "trackID : " << trackID << G4endl;
	G4cout << "eventID : " << eventID << G4endl;
	G4cout << "Creator Process : " << creator_process_name << G4endl;
	G4cout << "Detector name :" << physVol_name << G4endl;/*
	G4cout << "Detector position is:" << postDetectorPosition/cm << " cm" << G4endl;*/
	G4cout << "Position : " << postPosition/mm << "mm" << G4endl;
	G4cout << "Kinetic Energy is:" << postKE/MeV << " MeV" << G4endl;
	G4cout << "----------" << G4endl;
}