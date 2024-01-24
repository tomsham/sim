#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction* eventAction)
{
	fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step* step)
{
    G4String particle_name = step->GetTrack()->GetDefinition()->GetParticleName();
    G4double postKE = step->GetTrack()->GetKineticEnergy();
    //ReadOut(step);
    SaveToDataFile(step);
    //G4Track *track = step->GetTrack();
}

// This function store information to a Ntuple then it can be saved to root file in run.cc
void MySteppingAction::SaveToDataFile(const G4Step* step){
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4Track* track = step->GetTrack();
    G4String particle_type = track->GetParticleDefinition()->GetParticleType();						//Get the particle type
    G4String particle_name = track->GetDefinition()->GetParticleName();								//Get the particle name
    G4String creator_process_name = "NULL";
    if (track->IsGoodForTracking())
        creator_process_name = track->GetCreatorProcess()->GetProcessName();				        //Get the process name of the vertex of track 

    std::string partical_name_list[] = {"e+", "e-", "gamma", "nu_e"};
    int length_partical_name_list = sizeof(partical_name_list)/sizeof(std::string);					// length_str = 4

    for (int i = 0; i < length_partical_name_list; i++)
    {
        if (particle_name == partical_name_list[i]) {

            man->FillNtupleSColumn(0, 0, particle_name);
            man->FillNtupleSColumn(0, 1, particle_type);
            man->FillNtupleSColumn(0, 2, creator_process_name);
            man->AddNtupleRow(0);

            G4int trackId = track->GetTrackID();
            G4int stepID = track->GetCurrentStepNumber();
            G4StepPoint* prestep = step->GetPreStepPoint();
            G4StepPoint* poststep = step->GetPostStepPoint();

            // This part is for the information of initial step point
            G4double pre_ekin = prestep->GetKineticEnergy();											    //Get the Kinetic Energy
            G4ThreeVector pre_Position = prestep->GetPosition();                                            //Position
            G4ThreeVector pre_Direction = prestep->GetMomentumDirection();                                  //Momentum Direction
            G4double pre_x = pre_Position.x(), pre_y = pre_Position.y(), pre_z = pre_Position.z();
            G4double pre_theta = pre_Direction.theta(), pre_phi = pre_Direction.phi(), pre_costheta = pre_Direction.cosTheta();
            G4ThreeVector pre_Momentum = prestep->GetMomentum();
            G4double pre_px = pre_Momentum.x(); G4double pre_py = pre_Momentum.y(); G4double pre_pz = pre_Momentum.z();		//The momentum when it pass the detector

                                                                                                                            // This part is for the information of following step points
            G4double ekin = poststep->GetKineticEnergy();                                               //Get the Kinetic Energy
            G4ThreeVector trackPosition = poststep->GetPosition();                                      //Position
            G4ThreeVector trackDirection = poststep->GetMomentumDirection();                                 //Momentum Direction
            G4double x = trackPosition.x(), y = trackPosition.y(), z = trackPosition.z();
            G4double theta = trackDirection.theta(), phi = trackDirection.phi(), costheta = trackDirection.cosTheta();
            G4ThreeVector trackMomentum = track->GetMomentum();
            G4double px = trackMomentum.x(); G4double py = trackMomentum.y(); G4double pz = trackMomentum.z();         //The momentum when it pass the detector

                                                                                                                       // PreStepPoint
            // Initial Point
            if (stepID == 1) {
                man->FillNtupleDColumn(i+1, 0, pre_ekin/MeV);	//MeV
                man->FillNtupleDColumn(i+1, 1, pre_x/mm);		//mm
                man->FillNtupleDColumn(i+1, 2, pre_y/mm);		//mm
                man->FillNtupleDColumn(i+1, 3, pre_z/mm);		//mm
                man->FillNtupleDColumn(i+1, 4, pre_px/MeV);		//MeV/c
                man->FillNtupleDColumn(i+1, 5, pre_py/MeV);		//MeV/c
                man->FillNtupleDColumn(i+1, 6, pre_pz/MeV);		//MeV/c
                man->FillNtupleDColumn(i+1, 7, pre_theta);
                man->FillNtupleDColumn(i+1, 8, pre_costheta);
                man->FillNtupleDColumn(i+1, 9, pre_phi);
                man->FillNtupleIColumn(i+1, 10, 0);
                man->FillNtupleIColumn(i+1, 11, trackId);
                man->FillNtupleIColumn(i+1, 12, evt);
                man->FillNtupleSColumn(i+1, 13, creator_process_name);
                man->AddNtupleRow(i+1);
            }

            // PostStepPoint or TrackStepPoint
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
            man->FillNtupleIColumn(i+1, 10, stepID);
            man->FillNtupleIColumn(i+1, 11, trackId);
            man->FillNtupleIColumn(i+1, 12, evt);
            man->FillNtupleSColumn(i+1, 13, creator_process_name);
            man->AddNtupleRow(i+1);
        }
    }
}

void MySteppingAction::ReadOut(const G4Step* step) {
    G4Track* track = step->GetTrack();
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int trackId = track->GetTrackID();
    G4int stepID = track->GetCurrentStepNumber();
    G4String particle_name = track->GetDefinition()->GetParticleName();
    G4StepPoint* prestep = step->GetPreStepPoint();
    G4StepPoint* poststep = step->GetPostStepPoint();
    G4ThreeVector prePosition = prestep->GetPosition();
    G4double preKE = prestep->GetKineticEnergy();
    G4ThreeVector postPosition = poststep->GetPosition();
    G4double postKE = poststep->GetKineticEnergy();
    G4String logic_Volume_Name = track->GetTouchable()->GetVolume()->GetLogicalVolume()->GetName();
    G4String creator_process_name = "NULL";
    G4double preGlobalTime = prestep->GetGlobalTime();
    G4double postGlobalTime = poststep->GetGlobalTime();
    G4double preLocalTime = prestep->GetLocalTime();
    G4double postLocalTime = poststep->GetLocalTime();

    if (track->IsGoodForTracking())
        creator_process_name = track->GetCreatorProcess()->GetProcessName();				        //Get the process name of the vertex of track 

    if (trackId != current_track) {
        current_track = trackId;
        G4cout << G4endl;
    }

    if (stepID == 1) {
        G4cout << "----------" << G4endl;
        G4cout << "particle_name : " << particle_name << G4endl;
        G4cout << "stepID : " << "0" << G4endl;
        G4cout << "trackId : " << trackId << G4endl;
        G4cout << "evt : " << evt << G4endl;
        G4cout << "trackPosition : " << prePosition/mm << "mm" << G4endl;
        G4cout << "trackMom : " << prestep->GetMomentum().mag()/MeV << "MeV/c" << G4endl;
        if (preKE/MeV > 1)
            G4cout << "trackKE : " << preKE/MeV << "MeV" << G4endl;
        else if (preKE/keV > 1)
            G4cout << "trackKE : " << preKE/keV << "keV" << G4endl;
        else
            G4cout << "trackKE : " << preKE/eV << "eV" << G4endl;
        G4cout << "creator_process_name : " << creator_process_name << G4endl;
        G4cout << "DeltaTime : " << preLocalTime/ns << "ns" << G4endl;
        G4cout << "GlobalTime : " << preGlobalTime/ns << "ns" << G4endl;
        G4cout << "----------" << G4endl;
    }
    G4cout << "particle_name : " << particle_name << G4endl;
    G4cout << "stepID : " << stepID << G4endl;
    G4cout << "trackId : " << trackId << G4endl;
    //G4cout << "evt : " << evt << G4endl;
    G4cout << "prePosition : " << prePosition/mm << "mm" << G4endl;
    G4cout << "trackPosition : " << postPosition/mm << "mm" << G4endl;
    if (postKE/MeV > 1)
        G4cout << "trackKE : " << postKE/MeV << "MeV" << G4endl;
    else if (postKE/keV > 1)
        G4cout << "trackKE : " << postKE/keV << "keV" << G4endl;
    else
        G4cout << "trackKE : " << postKE/eV << "eV" << G4endl;
    G4cout << "creator_process_name : " << creator_process_name << G4endl;
    G4cout << "DeltaTime : " << (postGlobalTime-preGlobalTime)/ns << "ns" << G4endl;
    G4cout << "GlobalTime : " << postGlobalTime/ns << "ns" << G4endl;
    G4cout << "----------" << G4endl;
}