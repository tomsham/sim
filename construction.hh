#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH


#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4GenericMessenger.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();

	virtual G4VPhysicalVolume* Construct();
	void DefineMaterials();
	void DefineMessenger();

	// TestBlock and Detector
	void ConstructMaterialTest();

	// Positron Source
	void ConstructRing();
	void ConstructDisk();
	void ConstructBareSource(G4bool);
	// Ideal Detector for Positron Source
	void ConstructBSD_Ring_Detector();

	// Positronium Source
	void ConstructLAB_Acrylic();

	// CsI Detector
	void ConstructOffset();
	void ConstructCross();

	static G4String file_name;
	static G4String file_name0;
	static G4String file_name1;

private:
	G4GenericMessenger* fMessenger;

	// Radioactive Source (Positron Source)
	G4Material *Ti, *NaCl;

	// CsI Detector
	G4Material *CsI, *Mylar;

	// World
	G4Material *Air, *Vacuum, *matWorld;
	G4LogicalVolume *logicWorld;
	G4VPhysicalVolume *physWorld;

	// TestBlock and Detector
	G4Material *matTestBlock;
	G4bool isMaterialTest;
	G4double block_sideLength_half, block_thickness_half, gap_distance;
	G4LogicalVolume *logicTestBlock;
	G4VPhysicalVolume *physTestBlock;

	// Positron Source
	G4Material *matRing, *matDisk, *matSource;
	G4bool isRing, isDisk, isBareSource, isSource;
	G4double ring_radius, ring_height_half, disk_radius;
	G4double disk_height_half, bare_source_radius, bare_source_height_half;
	G4LogicalVolume *logicRing, *logicDisk, *logicBareSource;
	G4VPhysicalVolume *physRing, *physDisk, *physBareSource;
	G4bool isBareSource_Dt, isBSD_Ring_Dt;		//BS = Bare Source, D = Disk, Dt = Detector
	G4LogicalVolume *logicBareSource_Dt, *logicBSD_Ring_Dt;
	G4VPhysicalVolume *physBareSource_Dt, *physBSD_Ring_Dt;

	// Positronium Source, F = Front, B = Back
	G4VPhysicalVolume *physContainer_F, *physContainer_B, *physLAB;		
	G4Material *C16H26, *C17H28, *C18H30, *C19H32, *LAB, *Acrylic;
	G4bool isLAB_Acrylic;

	// CsI Detector
	G4bool isOffset, isCross;
	G4double wrapping_thickness;
	G4LogicalVolume *logicCsI, *logicMylar;
	G4VPhysicalVolume *physCsI, *physMylar;
	// F = Front, B = Back, L = Left, R = Right, U = Up, D = Down
	G4LogicalVolume *logicDetector_F, *logicDetector_B, *logicDetector_L;
	G4LogicalVolume *logicDetector_R, *logicDetector_U, *logicDetector_D;
	G4VPhysicalVolume *physDetector_F, *physDetector_B, *physDetector_L;
	G4VPhysicalVolume *physDetector_R, *physDetector_U, *physDetector_D;
};

#endif