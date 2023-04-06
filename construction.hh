#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH


#include "G4VUserDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GenericMessenger.hh"

#include "G4SubtractionSolid.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();

	virtual G4VPhysicalVolume* Construct();
	virtual void ConstructSDandField();
	void DefineMaterials();

	void ConstructPET();
	void ConstructCherenkov();
	void ConstructCubic_Detector_Demo();
	void ConstructCubic_Detector();

	G4LogicalVolume* GetScoringVolume() const {return fScoringVolume;};


	// From Eltis
	void ConstructDielectric();
	void ConstructSourceScintillator();
	void Construct_Energy_detector_Cylindrical();
	// End Eltis

private:
	G4GenericMessenger* fMessenger;
	
	G4int nCols, nRows;
	G4bool isPET, isCherenkov, isCubic_Detector_Demo, isCubic_Detector;

	G4Material *worldMat, *SiO2, *H2O, *Aerogel, *NaI;
	G4Element *C, *Na, *I;

	G4Box *solidWorld, *solidRadiator, *solidPET;
	G4Box *solidDetector_FB, *solidDetector_LR, *solidDetector_UD;
	G4Box *solidDetector;

	G4LogicalVolume *fScoringVolume;
	G4LogicalVolume *logicWorld, *logicRadiator, *logicPET;
	G4LogicalVolume *logicDetector_F, *logicDetector_B, *logicDetector_L, *logicDetector_R, *logicDetector_U, *logicDetector_D;
	G4LogicalVolume *logicDetector;
	
	G4VPhysicalVolume *physWorld, *physRadiator, *physPET;
	G4VPhysicalVolume *physDetector_F, *physDetector_B, *physDetector_L, *physDetector_R, *physDetector_U, *physDetector_D;
	
	// From Eltis
	G4bool dielectricMaterial, EnergyDetector_Cylindrical, SourceScintillator;
	G4Material *Ti_,*NaCl,*Kapton,* Polystyrene;
	G4Element *Ti,*Cl;

	G4LogicalVolume *logicDielectric,*logicSource,*logicEnergyDetector_cylindrical,*logic_SourceScintillator;
	G4VPhysicalVolume *physDielectric,*physSource,*physEnergyDetector_cylindrical,*phys_SourceScintillator;
	// End Eltis

};

#endif
