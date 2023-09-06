#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    quEff = new G4PhysicsOrderedFreeVector();
    /*
    //Setup detector efficiency
    std::ifstream datafile;
    datafile.open("eff.dat");
    
    while(1)
    {
        G4double wlen, queff;
        
        datafile >> wlen >> queff;
        
        if(datafile.eof())
            break;
        
        G4cout << wlen << " " << queff << std::endl;
        
        quEff->InsertValues(wlen, queff/100.);
    }
    
    datafile.close();
    */
}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    //Information of the final step
    G4Track *track = aStep->GetTrack();
    G4int pid = track->GetDynamicParticle()->GetPDGcode();
    G4String Particle_type = track->GetParticleDefinition()->GetParticleType();
    G4String Particle_Name = track->GetDefinition()->GetParticleName(); //Get the name of the particle
    //Information just touch the detector
    //G4double theta = direction.theta(), phi = direction.phi();
    G4String postProcessName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();//The process name
    G4double ekin = track->GetKineticEnergy();
    G4double etot = track->GetTotalEnergy();
    G4ThreeVector posReal = aStep->GetPreStepPoint()->GetPosition();
    //Find how many particle hit detector
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();
    
    #ifndef G4MULTITHREADED
	G4cout << "Particle type is: " << Particle_type << G4endl;
	G4cout << "Particle Name is:" << Particle_Name << G4endl;
	G4cout << "Process Name is:" << postProcessName << G4endl;
	G4cout << "Kinetic Energy is:" << ekin << G4endl;
	G4cout << "Total Energy is:" << etot << G4endl;
	G4cout << "Real position is:" << posReal << G4endl;
	G4cout << "Detector position is:" << posDetector << G4endl;
	G4cout << G4endl;
    #endif
    
    //Write inside the data
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4AnalysisManager *man = G4AnalysisManager::Instance();  
    
    man->FillNtupleSColumn(0, 0, Particle_Name);
    if (track->GetDefinition()->GetParticleName() == "e+") {
        G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
        G4double ekin = track->GetKineticEnergy(); //Kinetic Energy when it pass the detector
        G4double etot = track->GetTotalEnergy(); //Total Energy when it pass the detector
        G4ThreeVector vertex = track->GetPosition(); //Position when it pass the detector
        G4double x = vertex.x(), y = vertex.y(), z = vertex.z();
        G4ThreeVector direction = track->GetMomentumDirection();
        G4double theta = direction.theta(), phi = direction.phi();
        G4ThreeVector momentum = track->GetMomentum();
        G4double px = momentum.x(); G4double py = momentum.y(); G4double pz = momentum.z(); //The momentum when it pass the detector
        man->FillNtupleDColumn(1, 0, ekin);
        man->FillNtupleDColumn(1, 1, etot);
        man->FillNtupleDColumn(1, 2, x);
        man->FillNtupleDColumn(1, 3, y);
        man->FillNtupleDColumn(1, 4, z);
        man->FillNtupleDColumn(1, 5, px);
        man->FillNtupleDColumn(1, 6, py);
        man->FillNtupleDColumn(1, 7, pz);
        man->FillNtupleDColumn(1, 8, theta);
        man->FillNtupleDColumn(1, 9, phi);
        man->FillNtupleSColumn(1, 10, postProcessName);
        man->AddNtupleRow(1);
    }
    if (track->GetDefinition()->GetParticleName() == "e-") {
            G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
            G4double ekin = track->GetKineticEnergy(); //Kinetic Energy when it pass the detector
            G4double etot = track->GetTotalEnergy(); //Total Energy when it pass the detector
            G4ThreeVector vertex = track->GetPosition(); //Position when it pass the detector
            G4double x = vertex.x(), y = vertex.y(), z = vertex.z();
            G4ThreeVector direction = track->GetMomentumDirection();
            G4double theta = direction.theta(), phi = direction.phi();
            G4ThreeVector momentum = track->GetMomentum();
            G4double px = momentum.x(); G4double py = momentum.y(); G4double pz = momentum.z(); //The momentum when it pass the detector
            man->FillNtupleDColumn(2, 0, ekin);
            man->FillNtupleDColumn(2, 1, etot);
            man->FillNtupleDColumn(2, 2, x);
            man->FillNtupleDColumn(2, 3, y);
            man->FillNtupleDColumn(2, 4, z);
            man->FillNtupleDColumn(2, 5, px);
            man->FillNtupleDColumn(2, 6, py);
            man->FillNtupleDColumn(2, 7, pz);
            man->FillNtupleDColumn(2, 8, theta);
            man->FillNtupleDColumn(2, 9, phi);
            man->FillNtupleSColumn(2, 10, postProcessName);
            man->AddNtupleRow(2);
        }
    if (track->GetDefinition()->GetParticleName() == "gamma") {
            G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
            G4double ekin = track->GetKineticEnergy(); //Kinetic Energy when it pass the detector
            G4double etot = track->GetTotalEnergy(); //Total Energy when it pass the detector
            G4ThreeVector vertex = track->GetPosition(); //Position when it pass the detector
            G4double x = vertex.x(), y = vertex.y(), z = vertex.z();
            G4ThreeVector direction = track->GetMomentumDirection();
            G4double theta = direction.theta(), phi = direction.phi();
            G4ThreeVector momentum = track->GetMomentum();
            G4double px = momentum.x(); G4double py = momentum.y(); G4double pz = momentum.z(); //The momentum when it pass the detector
            man->FillNtupleDColumn(3, 0, ekin);
            man->FillNtupleDColumn(3, 1, etot);
            man->FillNtupleDColumn(3, 2, x);
            man->FillNtupleDColumn(3, 3, y);
            man->FillNtupleDColumn(3, 4, z);
            man->FillNtupleDColumn(3, 5, px);
            man->FillNtupleDColumn(3, 6, py);
            man->FillNtupleDColumn(3, 7, pz);
            man->FillNtupleDColumn(3, 8, theta);
            man->FillNtupleDColumn(3, 9, phi);
            man->FillNtupleSColumn(3, 10, postProcessName);
            man->AddNtupleRow(3);
        }
    if (track->GetDefinition()->GetParticleName() == "nu_e") {
            G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
            G4double ekin = track->GetKineticEnergy(); //Kinetic Energy when it pass the detector
            G4double etot = track->GetTotalEnergy(); //Total Energy when it pass the detector
            G4ThreeVector vertex = track->GetPosition(); //Position when it pass the detector
            G4double x = vertex.x(), y = vertex.y(), z = vertex.z();
            G4ThreeVector direction = track->GetMomentumDirection();
            G4double theta = direction.theta(), phi = direction.phi();
            G4ThreeVector momentum = track->GetMomentum();
            G4double px = momentum.x(); G4double py = momentum.y(); G4double pz = momentum.z(); //The momentum when it pass the detector
            man->FillNtupleDColumn(4, 0, ekin);
            man->FillNtupleDColumn(4, 1, etot);
            man->FillNtupleDColumn(4, 2, x);
            man->FillNtupleDColumn(4, 3, y);
            man->FillNtupleDColumn(4, 4, z);
            man->FillNtupleDColumn(4, 5, px);
            man->FillNtupleDColumn(4, 6, py);
            man->FillNtupleDColumn(4, 7, pz);
            man->FillNtupleDColumn(4, 8, theta);
            man->FillNtupleDColumn(4, 9, phi);
            man->FillNtupleSColumn(4, 10, postProcessName);
            man->AddNtupleRow(4);
        }
    if (track->GetDefinition()->GetParticleName() == "Ne22") {
            G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
            G4double ekin = track->GetKineticEnergy(); //Kinetic Energy when it pass the detector
            G4double etot = track->GetTotalEnergy(); //Total Energy when it pass the detector
            G4ThreeVector vertex = track->GetPosition(); //Position when it pass the detector
            G4double x = vertex.x(), y = vertex.y(), z = vertex.z();
            G4ThreeVector direction = track->GetMomentumDirection();
            G4double theta = direction.theta(), phi = direction.phi();
            G4ThreeVector momentum = track->GetMomentum();
            G4double px = momentum.x(); G4double py = momentum.y(); G4double pz = momentum.z(); //The momentum when it pass the detector
            man->FillNtupleDColumn(5, 0, ekin);
            man->FillNtupleDColumn(5, 1, etot);
            man->FillNtupleDColumn(5, 2, x);
            man->FillNtupleDColumn(5, 3, y);
            man->FillNtupleDColumn(5, 4, z);
            man->FillNtupleDColumn(5, 5, px);
            man->FillNtupleDColumn(5, 6, py);
            man->FillNtupleDColumn(5, 7, pz);
            man->FillNtupleDColumn(5, 8, theta);
            man->FillNtupleDColumn(5, 9, phi);
            man->FillNtupleSColumn(5, 10, postProcessName);
            man->AddNtupleRow(5);
        }
    if (track->GetDefinition()->GetParticleName() == "Ne22[1274.577]") {
            G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
            G4double ekin = track->GetKineticEnergy(); //Kinetic Energy when it pass the detector
            G4double etot = track->GetTotalEnergy(); //Total Energy when it pass the detector
            G4ThreeVector vertex = track->GetPosition(); //Position when it pass the detector
            G4double x = vertex.x(), y = vertex.y(), z = vertex.z();
            G4ThreeVector direction = track->GetMomentumDirection();
            G4double theta = direction.theta(), phi = direction.phi();
            G4ThreeVector momentum = track->GetMomentum();
            G4double px = momentum.x(); G4double py = momentum.y(); G4double pz = momentum.z(); //The momentum when it pass the detector
            man->FillNtupleDColumn(6, 0, ekin);
            man->FillNtupleDColumn(6, 1, etot);
            man->FillNtupleDColumn(6, 2, x);
            man->FillNtupleDColumn(6, 3, y);
            man->FillNtupleDColumn(6, 4, z);
            man->FillNtupleDColumn(6, 5, px);
            man->FillNtupleDColumn(6, 6, py);
            man->FillNtupleDColumn(6, 7, pz);
            man->FillNtupleDColumn(6, 8, theta);
            man->FillNtupleDColumn(6, 9, phi);
            man->FillNtupleSColumn(6, 10, postProcessName);
            man->AddNtupleRow(6);
        }
    if (track->GetDefinition()->GetParticleName() == "Na22") {
            G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
            G4double ekin = track->GetKineticEnergy(); //Kinetic Energy when it pass the detector
            G4double etot = track->GetTotalEnergy(); //Total Energy when it pass the detector
            G4ThreeVector vertex = track->GetPosition(); //Position when it pass the detector
            G4double x = vertex.x(), y = vertex.y(), z = vertex.z();
            G4ThreeVector direction = track->GetMomentumDirection();
            G4double theta = direction.theta(), phi = direction.phi();
            G4ThreeVector momentum = track->GetMomentum();
            G4double px = momentum.x(); G4double py = momentum.y(); G4double pz = momentum.z(); //The momentum when it pass the detector
            man->FillNtupleDColumn(7, 0, ekin);
            man->FillNtupleDColumn(7, 1, etot);
            man->FillNtupleDColumn(7, 2, x);
            man->FillNtupleDColumn(7, 3, y);
            man->FillNtupleDColumn(7, 4, z);
            man->FillNtupleDColumn(7, 5, px);
            man->FillNtupleDColumn(7, 6, py);
            man->FillNtupleDColumn(7, 7, pz);
            man->FillNtupleDColumn(7, 8, theta);
            man->FillNtupleDColumn(7, 9, phi);
            man->FillNtupleSColumn(7, 10, postProcessName);
            man->AddNtupleRow(7);
        }
    track->SetTrackStatus(fStopAndKill);
    man->FillNtupleSColumn(0, 1, postProcessName);
    man->FillNtupleDColumn(0, 2, evt);
    man->FillNtupleSColumn(0, 3, Particle_type);
    man->AddNtupleRow(0);
    
    
    
    
    
    
    
  
    return true;
}
