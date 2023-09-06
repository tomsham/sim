#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "detector.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();
    void ConstructScintillator();
    void ConstructDielectric();
    void Construct_Energy_detector_Cylindrical();
    void ConstructSourceScintillator();
    void ConstructAerogel();
    void DefineGeometry();
	void ConstructPET();
	void ConstructCherenkov();
	void ConstructCubic_Detector_Demo();
	void ConstructCubic_Detector();
    /*G4VPhysicalVolume is the return value of the class, and Construct() is the main function construct the detector geometry */
    virtual G4VPhysicalVolume *Construct();
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    
private:
    void DefineMaterials();
    virtual void ConstructSDandField();

    G4Box *solidScint;
    G4Box* solidWorld, * solidRadiator,*solidDetector,*solidPET;
    G4Box* solidDetector_FB, * solidDetector_LR, * solidDetector_UD;
    G4Sphere* solidEnergyDetector;
    //G4Tubs* OuterDielectric, * InnerSource;
    G4UnionSolid* Foil_Geometry;
    G4SubtractionSolid *solidDielectric,* SourceScintillator_SubtractOuter, *AerogelGeometry;

    G4LogicalVolume *logicWorld,* logic_SourceAerogel,*logicRadiator, *logicDetector, *logicScint,*logicPET;
    G4LogicalVolume *logicDielectric,*logicSource,* logicEnergyDetector_cylindrical,*logic_SourceScintillator;
    G4LogicalVolume *logicDetector_F, *logicDetector_B, *logicDetector_L, *logicDetector_R, *logicDetector_U, *logicDetector_D;
	
    G4VPhysicalVolume *physWorld, * phys_SourceAerogel, *physRadiator, *physDetector, *physScint,*physPET;
    G4VPhysicalVolume *physDielectric,*physSource,*physEnergyDetector, * physEnergyDetector_cylindrical,* phys_SourceScintillator;
    G4VPhysicalVolume *physDetector_F, *physDetector_B, *physDetector_L, *physDetector_R, *physDetector_U, *physDetector_D;
	
    
    G4Material* SiO2, * H2O, * Aerogel, * worldMat, * NaI, * Ti_, * NaCl, * Kapton, * Polystyrene, * foil_material,* scintillator_material;
    G4Element *C,*Na,*I,*Ti,*Cl;
    
    //add field/sensitive detector
    
    G4GenericMessenger *fMessenger;
    G4int nRows, nCols;
    G4double xWorld, yWorld, zWorld, outer_size_of_height, size_inner_radius, size_outer_radius, inner_size_of_height, size_Scintillator, height_of_Scintillator, height_of_Detector;
    G4bool cherenkov ,dielectricMaterial, EnergyDetector_Cylindrical, SourceScintillator , SourceAerogel, isCubic_Detector_Demo, isCubic_Detector,isPET;
    G4LogicalVolume*  fScoringVolume;
    G4OpticalSurface* mirrorSurface;

};

#endif
