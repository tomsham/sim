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

	void ConstructBareSource();
	void ConstructBareSourceInDisk();
	void ConstructDisk();
	void ConstructRing();

	void Construct_Ideal_Shell_Detector();
	void ConstructCsI();

	// From Eltis
	void ConstructSourceScintillator();
	void ConstructSourceAerogel();
	// End Eltis

	// For counting energy deposition
	G4LogicalVolume* GetScoringVolume() const {return fScoringVolume;};

private:
	G4GenericMessenger* fMessenger;
	
	G4double thick, Aerogel_size, World_size;
	G4bool isCsI, isShell_Detector, isBareSource, isDisk, isRing;

	G4Material *worldMat, *vacuumMat, *SiO2, *H2O, *Aerogel, *CsI, *Mylar,*Aerogel_Material,*Scintillator_Material;
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
	
	// From Eltis, edited by Tom
	G4bool isSourceScintillator,isSourceAerogel;
	G4Material *Ti_,*NaCl,*Kapton,*Polystyrene;
	G4Element *Ti,*Cl;
	G4double disk_radius, ring_radius, disk_height_half, ring_height_half, Scintillator_radius, Scintillator_height;
	G4Tubs *solidRing, *solidDisk, *solidBareSource;
	G4UnionSolid *Foil_Geometry;
	G4SubtractionSolid* BaseSourceAerogel_NoScintillator;
	G4LogicalVolume *logicRing,*logicDisk,*logicSource, *logic_SourceScintillator,*logic_SourceAerogel;
	G4VPhysicalVolume *physRing,*physDisk,*physSource, *phys_SourceScintillator,*phys_SourceAerogel;
	// End Eltis

};

#endif
