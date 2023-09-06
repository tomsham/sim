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
#include "G4PVParameterised.hh"
#include "G4GenericMessenger.hh"

#include "G4UnionSolid.hh"
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

	void ConstructCsI();

	G4LogicalVolume* GetScoringVolume() const {return fScoringVolume;};

	void Construct_Ideal_Shell_Detector();

	// From Eltis
	void ConstructDielectric();
	void ConstructSourceScintillator();
	//void ConstructAerogel();
	// End Eltis

private:
	G4GenericMessenger* fMessenger;
	
	G4double thick;
	G4bool isShell_Detector;
	G4bool isCsI, isCsI__with_mother;

	G4Material *worldMat, *SiO2, *H2O, *Aerogel, *CsI, *Mylar;
	G4Element *C, *Na, *I;

	G4Box *solidWorld;
	G4Box *solidDetector_H, *solidDetector_V;
	G4Box *solidCsI;

	G4Sphere *solidDetector_Shell;

	G4LogicalVolume *fScoringVolume;
	G4LogicalVolume *logicWorld, *logicDetector, *logicCsI, *logicDetector_Shell;
	G4LogicalVolume *logicDetector_F, *logicDetector_B, *logicDetector_L, *logicDetector_R, *logicDetector_U, *logicDetector_D;
	
	G4VPhysicalVolume *physWorld, *physDetector, *physDetector_Shell;
	G4VPhysicalVolume *physDetector_F, *physDetector_B, *physDetector_L, *physDetector_R, *physDetector_U, *physDetector_D;
	
	// From Eltis
	G4bool dielectricMaterial, SourceScintillator;
	G4Material *Ti_,*NaCl,*Kapton,*Polystyrene;
	G4Element *Ti,*Cl;
	G4double disk_radius, ring_radius, disk_height_half, ring_height_half;
	G4Tubs *RingDielectric, *DiskDielectric, *SourceDielectric;
	G4UnionSolid *Foil_Geometry;

	G4LogicalVolume *logicRing,*logicDisk,*logicSource, *logic_SourceScintillator;
	G4VPhysicalVolume *physRing,*physDisk,*physSource, *phys_SourceScintillator;
	// End Eltis

};

#endif
