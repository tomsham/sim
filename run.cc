// Tut08, This file is actually running the simulation and taking results.
#include "run.hh"

MyRunAction::MyRunAction(){
	G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Here to select which type of Data File to be created. Options: Normal, Vertex
    G4bool select = 0;
    if (select == 0)
        CreateDataFile(man);
    else
        CreateDataFile_Vertex(man);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run){
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->SetNtupleMerging(true);
	G4int runID = run->GetRunID();

	std::stringstream strRunID;
	strRunID << runID;

	man->OpenFile("output_"+strRunID.str()+".root");
}

void MyRunAction::EndOfRunAction(const G4Run* run){
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->Write();
	man->CloseFile();
}

// Save data to root file, from Eltis
void MyRunAction::CreateDataFile(G4AnalysisManager* man)
{
    // CreateNtuple(Name, Title)
    man->CreateNtuple("Particles", "Particle Property");
    man->CreateNtupleSColumn("ParticleName");
    man->CreateNtupleSColumn("ParticleType");
    man->CreateNtupleSColumn("CreatorProcessName");
    man->FinishNtuple(0);

    std::string partical_name[] = {"positron", "electron", "gamma", "nu_e", "Ne22", "Ne22_excited", "Na22" };
    int length_partical_name = sizeof(partical_name)/sizeof(std::string); // length_str = 7
    for (int i = 0; i < length_partical_name; i++)
    {
        man->CreateNtuple(partical_name[i], "Particle Property");
        man->CreateNtupleDColumn("KineticEnergy_MeV");
        man->CreateNtupleDColumn("PositionX_mm");
        man->CreateNtupleDColumn("PositionY_mm");
        man->CreateNtupleDColumn("PositionZ_mm");
        man->CreateNtupleDColumn("MomentumX_MeV_c");
        man->CreateNtupleDColumn("MomentumY_MeV_c");
        man->CreateNtupleDColumn("MomentumZ_MeV_c");
        man->CreateNtupleDColumn("Theta");
        man->CreateNtupleDColumn("cosTheta");
        man->CreateNtupleDColumn("Phi");
        man->CreateNtupleSColumn("CreatorProcessName");
        man->FinishNtuple(i+1);
    }
}

void MyRunAction::CreateDataFile_Vertex(G4AnalysisManager* man)
{
    // CreateNtuple(Name, Title)
    man->CreateNtuple("Particles", "Particle Property");
    man->CreateNtupleSColumn("ParticleName");
    man->CreateNtupleSColumn("ParticleType");
    man->CreateNtupleSColumn("CreatorProcessName");
    man->FinishNtuple(0);

    std::string partical_name[] = {"positron", "electron", "gamma", "nu_e", "Ne22", "Ne22_excited", "Na22" };
    int length_partical_name = sizeof(partical_name)/sizeof(std::string); // length_str = 7
    for (int i = 0; i < length_partical_name; i++)
    {
        man->CreateNtuple(partical_name[i], "Particle Property");
        man->CreateNtupleDColumn("VertexKineticEnergy_MeV");
        man->CreateNtupleDColumn("VertexPositionX_mm");
        man->CreateNtupleDColumn("VertexPositionY_mm");
        man->CreateNtupleDColumn("VertexPositionZ_mm");
        man->CreateNtupleDColumn("VertexMomentumX_MeV_c");
        man->CreateNtupleDColumn("VertexMomentumY_MeV_c");
        man->CreateNtupleDColumn("VertexMomentumZ_MeV_c");
        man->CreateNtupleDColumn("VertexTheta");
        man->CreateNtupleDColumn("cosVertexTheta");
        man->CreateNtupleDColumn("VertexPhi");
        man->CreateNtupleSColumn("CreatorProcessName");
        man->FinishNtuple(i+1);
    }
}