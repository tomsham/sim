// Tut08, This file is actually running the simulation and taking results.
#include "run.hh"

MyRunAction::MyRunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	// Monte Carlo data (Real position)
	// CreateNtuple(Name, Title)
	/*// For CsI Detector
	man->CreateNtuple("Real_Hits", "Real_Hits");
	man->CreateNtupleIColumn("Event_ID");		//Intege
	man->CreateNtupleDColumn("posReal_X");		//Double
	man->CreateNtupleDColumn("posReal_Y");
	man->CreateNtupleDColumn("posReal_Z");
	man->CreateNtupleDColumn("e_tot");
	man->CreateNtupleDColumn("e_kin");
	man->FinishNtuple(0);

	man->CreateNtuple("Detector_Hits", "Detector_Hits");
	man->CreateNtupleIColumn("Event_ID");
	man->CreateNtupleDColumn("posDetector_X");
	man->CreateNtupleDColumn("posDetector_Y");
	man->CreateNtupleDColumn("posDetector_Z");
	man->FinishNtuple(1);

	man->CreateNtuple("Scoring_Crystal", "Scoring_Crystal");
	man->CreateNtupleIColumn("fEdep");
	man->FinishNtuple(2);
	*/

	/*// My Ntuple for Gamma leakage
	man->CreateNtuple("Generator", "1MeV_gamma");
	man->CreateNtupleDColumn("Direction_X");
	man->CreateNtupleDColumn("Direction_Y");
	man->CreateNtupleDColumn("Direction_Z");
	man->CreateNtupleDColumn("e_tot_MeV");
	man->FinishNtuple(0);

	man->CreateNtuple("logicDetector_Shell", "Real_Hits");
	man->CreateNtupleIColumn("particle_name");
	man->CreateNtupleDColumn("posReal_X_cm");
	man->CreateNtupleDColumn("posReal_Y_cm");
	man->CreateNtupleDColumn("posReal_Z_cm");
	man->CreateNtupleDColumn("e_tot_MeV");
	man->FinishNtuple(1);
	*/

    EltisFinal(man);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->SetNtupleMerging(true);
	G4int runID = run->GetRunID();

	std::stringstream strRunID;
	strRunID << runID;

	man->OpenFile("output_"+strRunID.str()+".root");
}

void MyRunAction::EndOfRunAction(const G4Run* run)
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();

	man->Write();
	man->CloseFile();
}

void MyRunAction::EltisFinal(G4AnalysisManager* man)
{
    man->CreateNtuple("Particles", "Particle Property");
    man->CreateNtupleSColumn("Particle_Name");
    man->CreateNtupleSColumn("Process");
    man->CreateNtupleDColumn("fEvent");
    man->CreateNtupleSColumn("ParticleType");
    man->FinishNtuple(0);

    std::string partical_name[] = {"positron", "electron", "gamma", "nu_e", "Ne22", "Ne22_excited", "Na22" };
    int length_str = sizeof(partical_name)/sizeof(std::string); // length_str = 7
    for (int i = 0; i < length_str; i++)
    {
        man->CreateNtuple(partical_name[i], "Particle Property");
        man->CreateNtupleDColumn("KineticEnergy");
        man->CreateNtupleDColumn("TotalEnergy");
        man->CreateNtupleDColumn("PositionX");
        man->CreateNtupleDColumn("PositionY");
        man->CreateNtupleDColumn("PositionZ");
        man->CreateNtupleDColumn("MomentumX");
        man->CreateNtupleDColumn("MomentumY");
        man->CreateNtupleDColumn("MomentumZ");
        man->CreateNtupleDColumn("Theta");
        man->CreateNtupleDColumn("phi");
        man->CreateNtupleSColumn("Process");
        man->FinishNtuple(i+1);
    }
}