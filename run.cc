#include "run.hh"
#include "construction.hh"

MyRunAction::MyRunAction(){
	G4AnalysisManager *man = G4AnalysisManager::Instance();
    CreateDataFile_Stepping(man);
}

MyRunAction::~MyRunAction(){}

void MyRunAction::BeginOfRunAction(const G4Run* run){
    /*G4RunManager* runManager = G4RunManager::GetRunManager();
    runManager->GeometryHasBeenModified();
    runManager->ReinitializeGeometry();*/
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->SetNtupleMerging(true);
	G4int runID = run->GetRunID();
    G4String file_name = MyDetectorConstruction::file_name0 + MyDetectorConstruction::file_name1;

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

void MyRunAction::CreateDataFile_Stepping(G4AnalysisManager* man)
{
    // CreateNtuple(Name, Title)
    man->CreateNtuple("Particles", "Particle Info");
    man->CreateNtupleSColumn("ParticleName");
    man->CreateNtupleIColumn("Event_ID");
    man->CreateNtupleSColumn("PhysVolumeName");
    man->FinishNtuple(0);

    //std::string partical_name[] = {"positron", "electron", "gamma", "nu_e"};
    std::string partical_name[] = {"positron", "electron", "_gamma", "nu_e", "oPs", "pPs"};
    int length_partical_name = sizeof(partical_name)/sizeof(std::string); // length_str = 6
    for (int i = 0; i < length_partical_name; i++)    {
        man->CreateNtuple(partical_name[i], "Particle Info");
        man->CreateNtupleDColumn("KineticEnergy_MeV");
        man->CreateNtupleDColumn("PositionX_mm");
        man->CreateNtupleDColumn("PositionY_mm");
        man->CreateNtupleDColumn("PositionZ_mm");
        man->CreateNtupleDColumn("DeltaPosition_mm");
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