#include "detector.hh"

MySensentiveDetector::MySensentiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensentiveDetector::~MySensentiveDetector()
{}

/*	
In Geant4, 1 run contains number of events, 1 event contains number of tracks and steps.
The track stores current information of particle and updated after each step, the step stores changing information of particle.
Vertex that store the initial information of a particle.
New vertex can be created by existed track from physical processes.
*/

G4bool MySensentiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
	G4Track* track = aStep->GetTrack();
	//track->SetTrackStatus(fStopAndKill);

	//ReadOut(aStep, track);
	//ReadOut_Vertex(aStep, track);
	//SaveToDataFile(aStep, ROhist, track);
	//SaveToDataFile_Vertex(aStep, ROhist, track);

	//G4String Self_Name = track->GetTouchable()->GetVolume()->GetName();
	////G4String Self_Name = "0";
	//G4double mom_z = track->GetMomentum().z();
	//if (Self_Name == "Detector_Cylinder") {
	//	SaveToDataFile(aStep, ROhist, track);
	//	track->SetTrackStatus(fStopAndKill);
	//}
	//if (Self_Name == "Detector_Cylinder_Ends") {
	//	SaveToDataFile(aStep, ROhist, track);
	//	track->SetTrackStatus(fStopAndKill);
	//}
	//if (Self_Name == "BareSource_Dt") {
	//	SaveToDataFile_Vertex(aStep, ROhist, track);
	//	track->SetTrackStatus(fStopAndKill);
	//}
	//if (Self_Name == "BS_Disk_Dt" || Self_Name == "BSD_Ring_Dt") {
	//	track->SetTrackStatus(fStopAndKill);
	//	SaveToDataFile(aStep, ROhist, track);
	//}
	//if (Self_Name == "RingDetector_F" || Self_Name == "PlaneDetector_F") {
	//	if (mom_z > 0)
	//		SaveToDataFile(aStep, ROhist, track);
	//	else
	//		track->SetTrackStatus(fStopAndKill);
	//}
	//if (Self_Name == "RingDetector_B" || Self_Name == "PlaneDetector_B") {
	//	if (mom_z < 0)
	//		SaveToDataFile(aStep, ROhist, track);
	//	else
	//		track->SetTrackStatus(fStopAndKill);
	//}
	//if (Self_Name == "TubeDetector") {
	//	SaveToDataFile(aStep, ROhist, track);
	//	track->SetTrackStatus(fStopAndKill);
	//}
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

void MySensentiveDetector::SaveToDataFile_Vertex(G4Step* aStep, G4TouchableHistory* ROhist, G4Track* track){
    G4AnalysisManager *man = G4AnalysisManager::Instance();
	G4String particle_type = track->GetParticleDefinition()->GetParticleType();						//Get the particle type
	G4String particle_name = track->GetDefinition()->GetParticleName();								//Get the particle name
	G4String creator_process_name = "NULL";
	if (track->GetCreatorProcess())
		creator_process_name = track->GetCreatorProcess()->GetProcessName();						//Get the process name of the vertex of track 

    std::string partical_name_list[] = {"e+", "e-", "gamma", "nu_e" };
    int length_partical_name_list = sizeof(partical_name_list)/sizeof(std::string);					// length_str = 7

    for (int i = 0; i < length_partical_name_list; i++)
    {
        if (particle_name == partical_name_list[i]) {
			G4double ekin = track->GetVertexKineticEnergy();										//Get Vertex Kinetic Energy when it hit the detector
			G4ThreeVector Vertex_Position = track->GetVertexPosition();								//Get Vertex Position when it pass the detector
			G4ThreeVector Vertex_Momentum_Direction = track->GetVertexMomentumDirection();			//Get Vertex Momentum Direction when it pass the detector
			G4double Vertex_Velocity = track->CalculateVelocity();									//Get Vertex Momentum Velocity when it pass the detector
			G4double Vertex_Mass = track->GetDefinition()->GetPDGMass();							//Get Vertex Mass when it pass the detector
			G4double A = Vertex_Mass/(Vertex_Mass+ekin);
			G4double Vertex_Momentum = 0.;															//Get Vertex Momentum when it pass the detector
			if (Vertex_Mass == 0)
				Vertex_Momentum = ekin;
			else
				Vertex_Momentum = 1/A*Vertex_Mass*sqrt(1-A*A);										// Vertex_Velocity = sqrt(1-A*A)*c_light;
			G4double x = Vertex_Position.x(),  y = Vertex_Position.y(), z = Vertex_Position.z();
            G4double px = Vertex_Momentum*Vertex_Momentum_Direction.x();
			G4double py = Vertex_Momentum*Vertex_Momentum_Direction.y();
			G4double pz = Vertex_Momentum*Vertex_Momentum_Direction.z();
			G4double theta = Vertex_Momentum_Direction.theta();
			G4double costheta = Vertex_Momentum_Direction.cosTheta();
			G4double phi = Vertex_Momentum_Direction.phi();
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
            man->AddNtupleRow(i+1);
        }
    }
	man->FillNtupleSColumn(0, 0, particle_name);
    man->FillNtupleSColumn(0, 1, particle_type);
    man->FillNtupleSColumn(0, 2, creator_process_name);
    man->AddNtupleRow(0);
}

// Output Information just touch the detector
void MySensentiveDetector::ReadOut(G4Step* aStep, G4Track* track) {
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	G4String Particle_Name = track->GetDefinition()->GetParticleName();
	G4String creator_process_name = "NULL";
	if (track->GetCreatorProcess())
		creator_process_name = track->GetCreatorProcess()->GetProcessName();				//Get the process name of the vertex of track 
	G4String physVol_name = touchable->GetVolume()->GetName();
	G4String logicVol_name = touchable->GetVolume()->GetLogicalVolume()->GetName();
	G4String motherlogicVol_name = touchable->GetVolume()->GetMotherLogical()->GetName();
	G4ThreeVector posReal = aStep->GetPreStepPoint()->GetPosition();
	G4ThreeVector posDetector = touchable->GetVolume()->GetTranslation();
	G4double ekin = track->GetKineticEnergy();
	G4double theta = track->GetMomentumDirection().theta(), phi = track->GetMomentumDirection().phi();

	G4cout << "Particle Name is:" << Particle_Name << G4endl;
	G4cout << "CreatorProcess is:" << creator_process_name << G4endl;
	G4cout << "Kinetic Energy is:" << ekin/MeV << " MeV" << G4endl;
	G4cout << "Theta is:" << theta/degree << " degree" << G4endl;
	G4cout << "Phi is:" << phi/degree << " degree" << G4endl;
	G4cout << "Real position is:" << posReal/cm << " cm" << G4endl;
	G4cout << "Relative Detector_T is:" << posReal.perp()/mm << " mm" << G4endl;
	G4cout << "Relative Detector_B is:" << posReal.z()/mm+0.254 << " mm" << G4endl;
	G4cout << "Relative Detector_F is:" << posReal.z()/mm-0.254 << " mm" << G4endl;
	G4cout << "Detector name is:" << physVol_name << G4endl;
	G4cout << "Mother Name is:" << track->GetTouchable()->GetVolume(1)->GetName() << G4endl;
	//G4cout << "Mother Logic is:" << motherlogicVol_name << G4endl;
	G4cout << "Pr_st Mom is:" << aStep->GetPreStepPoint()->GetMomentum() << G4endl;
	G4cout << G4endl;
}

void MySensentiveDetector::ReadOut_Vertex(G4Step* aStep, G4Track* track) {
	G4String Particle_Name = track->GetDefinition()->GetParticleName();
	G4String Vertex_Process = "NULL";
	if (track->GetCreatorProcess())
		Vertex_Process = track->GetCreatorProcess()->GetProcessName();
	G4ThreeVector Vertex_Position = track->GetVertexPosition();
	G4ThreeVector Vertex_Momentum_Direction = track->GetVertexMomentumDirection();
	G4double Vertex_Mass = track->GetDefinition()->GetPDGMass();
	G4double Vertex_KE = track->GetVertexKineticEnergy();
	G4double A = Vertex_Mass/(Vertex_Mass+Vertex_KE);
	G4double c_light = 2.99792458e+2;						// Mm/s
	G4double Vertex_Momentum = 0.;
	if (Vertex_Mass == 0)
		Vertex_Momentum = Vertex_KE;
	else
		Vertex_Momentum = 1/A*Vertex_Mass*sqrt(1-A*A);		// Vertex_Velocity = sqrt(1-A*A)*c_light;

	G4cout << "Particle_Name: " << Particle_Name << G4endl;
	G4cout << "Detector_Name: " << track->GetTouchable()->GetVolume()->GetName() << G4endl;
	G4cout << "Vertex_Process: " << Vertex_Process << G4endl;
	G4cout << "Vertex_Position: " << Vertex_Position << G4endl;
	G4cout << "Vertex_Position.mag: " << Vertex_Position.mag() << G4endl;
	G4cout << "Vertex_Momentum_Direction: " << Vertex_Momentum_Direction << G4endl;
	G4cout << "Vertex_Mass: " << Vertex_Mass << G4endl;
	G4cout << "Cal_Vertex_Momentum.mag: " << Vertex_Momentum/MeV << "MeV/c" << G4endl;
	G4cout << "Vertex_KE: " << Vertex_KE/MeV << "MeV" << G4endl;
	G4cout << G4endl;
}