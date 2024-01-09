#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH


#include "G4VUserDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
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

	/*The construction functions with underline are the functions of ideal detectors*/

	// Positron source
	void ConstructRing();
	void ConstructDisk();
	void ConstructBareSource(G4bool);
	void ConstructBareSource_Detector();
	void ConstructBS_Disk_Detector();
	void ConstructBSD_Ring_Detector();
	// End Positron source
	
	// Liquid scintillator
	void ConstructLiquidScintillator();
	void ConstructCup_Detector();
	void ConstructLAB_Acrylic();
	void ConstructAcrylicBlock();
	// End Liquid scintillator

	// Ideal Cylinder Detector
	void ConstructCylinder_Detector();
	// End Ideal Cylinder Detector

	// Ideal Detector
	void ConstructShell_Detector();
	// End Ideal Detector

	// CsI Detector
	void ConstructCsI();
	void ConstructCsI_2();
	// End CsI Detector

	// From Eltis
	void ConstructSourceScintillator();
	//void ConstructAerogel();
	// End Eltis

	// For counting energy deposition
	G4LogicalVolume* GetScoringVolume() const {return fScoringVolume;};

private:
	G4GenericMessenger* fMessenger;

	// Aerogel
	G4Material *Aerogel, *SiO2, *H2O;
	G4Element *C;
	// Radioactive Source (Positron Source)
	G4Material *Ti_, *NaCl;
	G4Element *Ti;
	
	// Design-1 CsI Detector
	G4Material *CsI, *Mylar;
	G4Material *Kapton, *Polystyrene;

	// World
	G4Material *Air, *Vacuum, *matWorld;
	G4Box *solidWorld;
	G4LogicalVolume *logicWorld;
	G4VPhysicalVolume *physWorld;
	// End World

	// Radioactive Source (Positron Source)
	G4Material *matRing, *matDisk, *matSource;
	G4bool isRing, isDisk, isBareSource, isSource;
	G4double ring_radius, ring_height_half, disk_radius, disk_height_half, bare_source_radius, bare_source_height_half;
	G4Tubs *solidRing, *solidDisk, *solidBareSource;
	G4LogicalVolume *logicRing, *logicDisk, *logicBareSource;
	G4VPhysicalVolume *physRing, *physDisk, *physBareSource;
	G4bool isBareSource_Dt, isBS_Disk_Dt, isBSD_Ring_Dt;						//BS = Bare Source, D = Disk, Dt = Detector
	G4LogicalVolume *logicBareSource_Dt, *logicBS_Disk_Dt, *logicBSD_Ring_Dt;
	G4VPhysicalVolume *physBareSource_Dt, *physBS_Disk_Dt, *physBSD_Ring_Dt;
	// End Radioactive Source

	// Liquid scintillator
	G4Material *matContainer, *matLiquid;
	G4bool isLiquid, isCupDetector;
	G4double container_radius, container_height_half, container_thickness, d_pos_z;
	G4Tubs *solidContainer, *solidLiquid, *solidPlaneDetector, *solidRingDetector, *solidTubeDetector;
	G4LogicalVolume *logiContainer_F, *logiContainer_B, *logicLiquid_F, *logicLiquid_B;		// F = Front, B = Back
	G4LogicalVolume *logicPlaneDetector_W, *logicPlaneDetector_C, *logicPlaneDetector_L;	// W = World, C = Container, L = Liquid
	G4LogicalVolume *logicRingDetector, *logicTubeDetector;
	G4VPhysicalVolume *physContainer_F, *physContainer_B, *physLiquid_F, *physLiquid_B, *physPlaneDetector, *physRingDetector, *physTubeDetector;

	G4Material *C16H26, *C17H28, *C18H30, *C19H32, *LAB, *Acrylic;
	G4bool isLAB_Acrylic, isAcrylicBlock;
	// End Liquid scintillator

	// Ideal Detector
	G4bool isDetector_Shell, isDetector_Cylinder;
	G4Sphere *solidDetector_Shell;
	G4LogicalVolume *logicDetector_Shell, *logicDetector_Cylinder;
	G4VPhysicalVolume *physDetector_Shell, *physDetector_Cylinder;
	// End Ideal Detector

	// CsI Detector
	G4bool isCsI, isCsI_2;
	G4double wrapping_thickness;
	G4Box *solidDetector_H, *solidDetector_V;
	G4Box *solidCsI;
	G4LogicalVolume *fScoringVolume;
	G4LogicalVolume *logicCsI;
	// F = Front, B = Back, L = Left, R = Right, U = Up, D = Down
	G4LogicalVolume *logicDetector_F, *logicDetector_B, *logicDetector_L, *logicDetector_R, *logicDetector_U, *logicDetector_D;
	G4VPhysicalVolume *physDetector_F, *physDetector_B, *physDetector_L, *physDetector_R, *physDetector_U, *physDetector_D;
	// End CsI Detector
	
	// From Eltis, edited by Tom
	G4bool isSourceScintillator;
	G4UnionSolid *Foil_Geometry;
	G4LogicalVolume *logic_SourceScintillator;
	G4VPhysicalVolume *phys_SourceScintillator;
	// End Eltis
};

#endif
