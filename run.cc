#include "run.hh"
#include "construction.hh"

MyRunAction::MyRunAction(){
	G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Here to select which type of Data File to be created. Options: Normal, Vertex, Step
    G4int select = 1;
    if (select == 0)
        CreateDataFile_SensitiveDetector(man);
    else if (select == 1)
        CreateDataFile_Stepping(man);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run){
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->SetNtupleMerging(true);
	G4int runID = run->GetRunID();
    G4String file_name = MyDetectorConstruction::file_name;

	std::stringstream strRunID;
	strRunID << runID;

    if (file_name == "")
	    man->OpenFile("output_"+strRunID.str()+".root");
    else
        man->OpenFile(file_name+".root");
}

void MyRunAction::EndOfRunAction(const G4Run* run){
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->Write();
	man->CloseFile();
}

void MyRunAction::CreateDataFile_SensitiveDetector(G4AnalysisManager* man)
{
    // CreateNtuple(Name, Title)
    man->CreateNtuple("Particles", "Particle Property");
    man->CreateNtupleSColumn("ParticleName");
    man->CreateNtupleSColumn("ParticleType");
    man->CreateNtupleSColumn("CreatorProcessName");
    man->FinishNtuple(0);

    std::string partical_name[] = {"positron", "electron", "gamma", "nu_e" };
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
        man->CreateNtupleSColumn("DetectorName");
        man->FinishNtuple(i+1);
    }
}

void MyRunAction::CreateDataFile_Stepping(G4AnalysisManager* man)
{
    // CreateNtuple(Name, Title)
    man->CreateNtuple("Particles", "Particle Info");
    man->CreateNtupleSColumn("ParticleName");
    man->FinishNtuple(0);

    //std::string partical_name[] = {"positron", "electron", "gamma", "nu_e"};
    std::string partical_name[] = {"positron", "electron", "gamma", "nu_e" };
    int length_partical_name = sizeof(partical_name)/sizeof(std::string); // length_str = 4
    for (int i = 0; i < length_partical_name; i++)
    {
        man->CreateNtuple(partical_name[i], "Particle Info");
        man->CreateNtupleDColumn("KineticEnergy_MeV");
        man->CreateNtupleDColumn("PositionX_mm");
        man->CreateNtupleDColumn("PositionY_mm");
        man->CreateNtupleDColumn("PositionZ_mm");
        man->CreateNtupleDColumn("Momentum_MeV_c");
        man->CreateNtupleDColumn("PolarAngle_Theta");
        man->CreateNtupleDColumn("cosTheta");
        man->CreateNtupleDColumn("AzimuthalAngle_Phi");
        man->CreateNtupleDColumn("LocalTime_ns");
        man->CreateNtupleDColumn("GlobalTime_ns");

        man->CreateNtupleIColumn("Step_ID");
        man->CreateNtupleIColumn("Track_ID");
        man->CreateNtupleIColumn("Event_ID");
        man->CreateNtupleSColumn("CreatorProcessName");
        man->CreateNtupleSColumn("PhysVolumeName");
        man->FinishNtuple(i+1);
    }
}