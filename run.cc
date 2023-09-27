// Tut08, This file is actually running the simulation and taking results.
#include "run.hh"

MyRunAction::MyRunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
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
    // CreateNtuple(Name, Title)
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
        man->CreateNtupleDColumn("KineticEnergy_MeV");
        man->CreateNtupleDColumn("TotalEnergy_MeV");
        man->CreateNtupleDColumn("PositionX_mm");
        man->CreateNtupleDColumn("PositionY_mm");
        man->CreateNtupleDColumn("PositionZ_mm");
        man->CreateNtupleDColumn("MomentumX");
        man->CreateNtupleDColumn("MomentumY");
        man->CreateNtupleDColumn("MomentumZ");
        man->CreateNtupleDColumn("Theta");
        man->CreateNtupleDColumn("phi");
        man->CreateNtupleSColumn("Process");
        man->FinishNtuple(i+1);
    }
}