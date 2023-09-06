#include "run.hh"
//Tell what output file to output
MyRunAction::MyRunAction()
{   
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->CreateNtuple("Particles", "Particle Property");
    man->CreateNtupleSColumn("Particle_Name");
    man->CreateNtupleSColumn("Process");
    man->CreateNtupleDColumn("fEvent");
    man->CreateNtupleSColumn("ParticleType");
    man->FinishNtuple(0);
    man->CreateNtuple("positron", "Particle Property");
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
    man->FinishNtuple(1);
    man->CreateNtuple("electron", "Particle Property");
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
    man->FinishNtuple(2);
    man->CreateNtuple("gamma", "Particle Property");
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
    man->FinishNtuple(3);
    man->CreateNtuple("nu_e", "Particle Property");
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
    man->FinishNtuple(4);
    man->CreateNtuple("Ne22", "Particle Property");
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
    man->FinishNtuple(5);
    man->CreateNtuple("Ne22_excited", "Particle Property");
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
    man->FinishNtuple(6);
    man->CreateNtuple("Na22", "Particle Property");
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
    man->FinishNtuple(7);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->SetNtupleMerging(true);
    G4int runID = run->GetRunID();
    
    std::stringstream strRunID;
    strRunID << runID;
    //Tell What kind of information to store
    man->OpenFile("output"+strRunID.str()+".root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    man->Write();
    man->CloseFile();
}
