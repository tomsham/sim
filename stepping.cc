#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction* eventAction)
{
	fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step* step)
{
    G4String detector_Name = step->GetTrack()->GetTouchable()->GetVolume()->GetName();
    //ReadOut(step);
    SaveToDataFile(step);
    if (detector_Name == "Detector_Shell") {
        ReadOut(step);
        //SaveToDataFile(step);
    }
}

// This function store information to a Ntuple then it can be saved to root file in run.cc
void MySteppingAction::SaveToDataFile(const G4Step* step){

    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4Track* track = step->GetTrack();
    G4String particle_name = track->GetDefinition()->GetParticleName();
    G4String creator_process_name = "NULL";

    // Get the process name of the vertex of that particle
    if (track->GetCreatorProcess())
        creator_process_name = track->GetCreatorProcess()->GetProcessName();

    std::string partical_name_list[] = {"e+", "e-", "gamma", "nu_e" };
    int length_partical_name_list = sizeof(partical_name_list)/sizeof(std::string);					// length_str = 4

    // This part records names of all simulated particles into Row 0 of Ntuple in man
    man->FillNtupleSColumn(0, 0, particle_name);
    man->AddNtupleRow(0);

    // This part records info. of particles in the partical_name_list
    for (int i = 0; i < length_partical_name_list; i++)
    {
        if (particle_name == partical_name_list[i]) {
            G4int trackID = track->GetTrackID();
            G4int stepID = track->GetCurrentStepNumber();
            G4String physVol_name = track->GetTouchable()->GetVolume()->GetName();

            // This part is for the information of initial step point
            G4StepPoint* preStep = step->GetPreStepPoint();
            G4double preKE = preStep->GetKineticEnergy();
            G4ThreeVector prePosition = preStep->GetPosition();
            G4ThreeVector preMomentumDirection = preStep->GetMomentumDirection();
            G4double pre_x = prePosition.x(), pre_y = prePosition.y(), pre_z = prePosition.z();
            G4double pre_theta = preMomentumDirection.theta(), pre_phi = preMomentumDirection.phi(), pre_costheta = preMomentumDirection.cosTheta();
            G4double preMomentum = preStep->GetMomentum().mag();
            G4double preLocalTime = preStep->GetLocalTime();
            G4double preGlobalTime = preStep->GetGlobalTime();

            // This part is for the information of following step points
            G4StepPoint* postStep = step->GetPostStepPoint();
            G4double postKE = postStep->GetKineticEnergy();
            G4ThreeVector postPosition = postStep->GetPosition();
            G4ThreeVector postMomentumDirection = postStep->GetMomentumDirection();
            G4double post_x = postPosition.x(), post_y = postPosition.y(), post_z = postPosition.z();
            G4double post_theta = postMomentumDirection.theta(), post_phi = postMomentumDirection.phi(), post_costheta = postMomentumDirection.cosTheta();
            G4double postMomentum = postStep->GetMomentum().mag();
            G4double postLocalTime = postStep->GetLocalTime();
            G4double postGlobalTime = postStep->GetGlobalTime();

            //
            // Initial Point which is made up by get prestep info. of Initial Step (stepID = 1)
            //
            if (stepID == 1) {
                man->FillNtupleDColumn(i+1, 0, preKE/MeV);
                man->FillNtupleDColumn(i+1, 1, pre_x/mm);
                man->FillNtupleDColumn(i+1, 2, pre_y/mm);
                man->FillNtupleDColumn(i+1, 3, pre_z/mm);
                man->FillNtupleDColumn(i+1, 4, preMomentum/MeV);        // Unit in MeV/c
                man->FillNtupleDColumn(i+1, 5, pre_theta);
                man->FillNtupleDColumn(i+1, 6, pre_costheta);
                man->FillNtupleDColumn(i+1, 7, pre_phi);
                man->FillNtupleDColumn(i+1, 8, preLocalTime/ns);
                man->FillNtupleDColumn(i+1, 9, preGlobalTime/ns);
                man->FillNtupleIColumn(i+1, 10, 0);
                man->FillNtupleIColumn(i+1, 11, trackID);
                man->FillNtupleIColumn(i+1, 12, eventID);
                man->FillNtupleSColumn(i+1, 13, creator_process_name);
                man->FillNtupleSColumn(i+1, 14, physVol_name);
                man->AddNtupleRow(i+1);
            }
            //
            // PostStepPoint or TrackStepPoint
            //
            man->FillNtupleDColumn(i+1, 0, postKE/MeV);
            man->FillNtupleDColumn(i+1, 1, post_x/mm);
            man->FillNtupleDColumn(i+1, 2, post_y/mm);
            man->FillNtupleDColumn(i+1, 3, post_z/mm);
            man->FillNtupleDColumn(i+1, 4, postMomentum/MeV);           // Unit in MeV/c
            man->FillNtupleDColumn(i+1, 5, post_theta);
            man->FillNtupleDColumn(i+1, 6, post_costheta);
            man->FillNtupleDColumn(i+1, 7, post_phi);
            man->FillNtupleDColumn(i+1, 8, postLocalTime/ns);
            man->FillNtupleDColumn(i+1, 9, postGlobalTime/ns);
            man->FillNtupleIColumn(i+1, 10, stepID);
            man->FillNtupleIColumn(i+1, 11, trackID);
            man->FillNtupleIColumn(i+1, 12, eventID);
            man->FillNtupleSColumn(i+1, 13, creator_process_name);
            man->FillNtupleSColumn(i+1, 14, physVol_name);
            man->AddNtupleRow(i+1);
        }
    }
}

void MySteppingAction::ReadOut(const G4Step* step) {

    G4Track* track = step->GetTrack();
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int trackID = track->GetTrackID();
    G4int stepID = track->GetCurrentStepNumber();
    G4String particle_name = track->GetDefinition()->GetParticleName();
    G4String physVol_name = track->GetTouchable()->GetVolume()->GetName();
    G4String creator_process_name = "NULL";

    G4StepPoint* preStep = step->GetPreStepPoint();
    G4ThreeVector prePosition = preStep->GetPosition();
    G4double preKE = preStep->GetKineticEnergy();
    G4double preLocalTime = preStep->GetLocalTime();
    G4double preGlobalTime = preStep->GetGlobalTime();

    G4StepPoint* postStep = step->GetPostStepPoint();
    G4ThreeVector postPosition = postStep->GetPosition();
    G4double postKE = postStep->GetKineticEnergy();
    G4double postLocalTime = postStep->GetLocalTime();
    G4double postGlobalTime = postStep->GetGlobalTime();

    // Get the process name of the vertex of that particle
    if (track->GetCreatorProcess())
        creator_process_name = track->GetCreatorProcess()->GetProcessName();

    if (trackID != current_track) {
        current_track = trackID;
        G4cout << G4endl;
    }

    if (stepID == 1) {
        G4cout << "----------" << G4endl;
        G4cout << "Particle : " << particle_name << G4endl;
        G4cout << "stepID : " << "0" << G4endl;
        G4cout << "trackID : " << trackID << G4endl;
        G4cout << "eventID : " << eventID << G4endl;
        G4cout << "Creator Process : " << creator_process_name << G4endl;
        G4cout << "Physical Volume :" << physVol_name << G4endl;
        G4cout << "Position : " << prePosition/mm << "mm" << G4endl;
        //G4cout << "Momentum : " << preStep->GetMomentum().mag()/MeV << "MeV/c" << G4endl;
        //==================================================================
        if (preKE/MeV > 1)
            G4cout << "KineticEnergy : " << preKE/MeV << "MeV" << G4endl;
        else if (preKE/keV > 1)
            G4cout << "KineticEnergy : " << preKE/keV << "keV" << G4endl;
        else
            G4cout << "KineticEnergy : " << preKE/eV << "eV" << G4endl;
        //==================================================================
        G4cout << "LocalTime : " << preLocalTime/ns << "ns" << G4endl;
        //==================================================================
        if (preGlobalTime/s > 1)
            G4cout << "GlobalTime : " << preGlobalTime/s << "s" << G4endl;
        else if (preGlobalTime/ms > 1)
            G4cout << "GlobalTime : " << preGlobalTime/ms << "ms" << G4endl;
        else if (preGlobalTime/us > 1)
            G4cout << "GlobalTime : " << preGlobalTime/us << "us" << G4endl;
        else if (preGlobalTime/ns > 1)
            G4cout << "GlobalTime : " << preGlobalTime/ns << "ns" << G4endl;
        else
            G4cout << "GlobalTime : " << preGlobalTime/ps << "ps" << G4endl;
        G4cout << "----------" << G4endl;
    }

    //==================================================================
    G4cout << "Particle : " << particle_name << G4endl;
    G4cout << "stepID : " << stepID << G4endl;
    G4cout << "trackID : " << trackID << G4endl;
    G4cout << "eventID : " << eventID << G4endl;
    G4cout << "Physical Volume:" << physVol_name << G4endl;
    G4cout << "Position : " << postPosition/mm << "mm" << G4endl;
    //G4cout << "Momentum : " << postStep->GetMomentum().mag()/MeV << "MeV/c" << G4endl;
    //==================================================================
    if (postKE/MeV > 1)
        G4cout << "KineticEnergy : " << postKE/MeV << "MeV" << G4endl;
    else if (postKE/keV > 1)
        G4cout << "KineticEnergy : " << postKE/keV << "keV" << G4endl;
    else
        G4cout << "KineticEnergy : " << postKE/eV << "eV" << G4endl;
    //==================================================================
    if (postLocalTime/s > 1)
        G4cout << "LocalTime : " << postLocalTime/s << "s" << G4endl;
    else if (postLocalTime/ms > 1)
        G4cout << "LocalTime : " << postLocalTime/ms << "ms" << G4endl;
    else if (postLocalTime/us > 1)
        G4cout << "LocalTime : " << postLocalTime/us << "us" << G4endl;
    else if (postLocalTime/ns > 1)
        G4cout << "LocalTime : " << postLocalTime/ns << "ns" << G4endl;
    else
        G4cout << "LocalTime : " << postLocalTime/ps << "ps" << G4endl;
    //==================================================================
    if (postGlobalTime/s > 1)
        G4cout << "GlobalTime : " << postGlobalTime/s << "s" << G4endl;
    else if (postGlobalTime/ms > 1)
        G4cout << "GlobalTime : " << postGlobalTime/ms << "ms" << G4endl;
    else if (postGlobalTime/us > 1)
        G4cout << "GlobalTime : " << postGlobalTime/us << "us" << G4endl;
    else if (postGlobalTime/ns > 1)
        G4cout << "GlobalTime : " << postGlobalTime/ns << "ns" << G4endl;
    else
        G4cout << "GlobalTime : " << postGlobalTime/ps << "ps" << G4endl;
    G4cout << "----------" << G4endl;
}