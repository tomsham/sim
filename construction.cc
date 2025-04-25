#include "construction.hh"

// Construction options
MyDetectorConstruction::MyDetectorConstruction(){
	// Define required materials
	DefineMaterials();

	// Material Test
	isMaterialTest = true;

	// Positron Source: isSource includes the Ring, Disk and BareSource
	isRing = false;
	isDisk = false;
	isBareSource = false;
	isSource = false;
	// Dt = Detector, only need to construct one of them
	isBSD_Ring_Dt = false;

	// Positronium Source
	isLAB_Acrylic = false;

	// CsI Detector: Offset or Cross
	isOffset = false;
	isCross = false;

	// Set the material for each logical volume
	matWorld = Air;				// Air or Vacuum;
	matTestBlock = Acrylic;		// LAB, Acrylic
	matRing = Ti, matDisk = Ti, matSource = NaCl;

	// Material Test Block
	block_sideLength_half = 0.74*m;
	block_thickness_half = 0.74*m;

	// Parameters for Positron Source
	ring_radius = 19.1/2*mm;
	// Supported by two 0.254 mm Ti disks
	ring_height_half = 0.254*mm;
	disk_radius = 9.53/2*mm;
	// Two layers of 0.00508*mm Ti foil which is 0.0102*mm in total
	disk_height_half = 0.00508*mm;
	bare_source_radius = disk_radius;
	// The thickness of bare source is not provide so this is a made up value
	bare_source_height_half = 0.0001*mm;	
	// The wrapping thickness of CsI crystals, 13.e-6 m = 13. um
	wrapping_thickness = 13.*um;
	DefineMessenger();
}
MyDetectorConstruction::~MyDetectorConstruction(){}

G4String MyDetectorConstruction::file_name = "";
G4String MyDetectorConstruction::file_name0 = "";
G4String MyDetectorConstruction::file_name1 = "";

void MyDetectorConstruction::DefineMaterials() {
	G4NistManager* nist = G4NistManager::Instance();
	G4double hc = 1239.841939 * eV;

	// World
	Air = nist->FindOrBuildMaterial("G4_AIR");
	Vacuum = nist->FindOrBuildMaterial("G4_Galactic");

	// NaCl
	// From p. 4-89 in doi.org/10.1201/b17118
	NaCl = new G4Material("NaCl", 2.17*g/cm3, 2);
	NaCl->AddElement(nist->FindOrBuildElement("Na"), 1);
	NaCl->AddElement(nist->FindOrBuildElement("Cl"), 4);

	// CsI
	CsI = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");

	// Mylar
	Mylar = nist->FindOrBuildMaterial("G4_MYLAR");

	// Ti
	Ti = nist->FindOrBuildMaterial("G4_Ti");

	// LAB
	C16H26 = new G4Material("C16H26", 0.856*g/cm3, 2);
	C16H26 -> AddElement(nist->FindOrBuildElement("C"), 16);
	C16H26 -> AddElement(nist->FindOrBuildElement("H"), 26);

	C17H28 = new G4Material("C17H28", 0.855*g/cm3, 2);
	C17H28 -> AddElement(nist->FindOrBuildElement("C"), 17);
	C17H28 -> AddElement(nist->FindOrBuildElement("H"), 28);

	C18H30 = new G4Material("C18H28", 0.856*g/cm3, 2);
	C18H30 -> AddElement(nist->FindOrBuildElement("C"), 18);
	C18H30 -> AddElement(nist->FindOrBuildElement("H"), 30);

	C19H32 = new G4Material("C19H32", 0.881*g/cm3, 2);
	C19H32 -> AddElement(nist->FindOrBuildElement("C"), 19);
	C19H32 -> AddElement(nist->FindOrBuildElement("H"), 32);

	LAB = new G4Material("LAB",  0.86*g/cm3, 4);
	LAB -> AddMaterial(C16H26, 6.98*perCent);
	LAB -> AddMaterial(C17H28, 30.6*perCent);
	LAB -> AddMaterial(C18H30, 45.0*perCent);
	LAB -> AddMaterial(C19H32, 17.4*perCent);

	// From 10.1088/0031-8949/82/06/065706
	// hc/wavelength(in nm)
	G4double energyLAB[6] = { hc/627.3, hc/579.8, hc/547.7, hc/493.9, hc/438.5, hc/405.1};		
	G4double rindexLAB[6] = { 1.4786, 1.48118, 1.48329, 1.48786, 1.49505, 1.50061 };
	G4MaterialPropertiesTable* mptLAB = new G4MaterialPropertiesTable();
	mptLAB->AddProperty("RINDEX", energyLAB, rindexLAB, 2);
	LAB->SetMaterialPropertiesTable(mptLAB);
	// End LAB

	// Acrylic, from Geant4>examples>advanced>air_shower>UltraDetectorConstruction.cc
	Acrylic = new G4Material("Acrylic", 1.19*g/cm3, 3);
	Acrylic->AddElement(nist->FindOrBuildElement("C"), 5);
	Acrylic->AddElement(nist->FindOrBuildElement("H"), 8);
	Acrylic->AddElement(nist->FindOrBuildElement("O"), 2);
}

void MyDetectorConstruction::DefineMessenger() {
	// The A is a placeholder for the user defined commands (fMessenger)
	G4int placeHolder = 0;
	// User defined commands for use in User-Interface(UI)
	fMessenger = new G4GenericMessenger(this, "/PositroniumProject/", "Macros");
	fMessenger->DeclareProperty("control/execute region_setup.mac", placeHolder, 
								"Set the active region");
	fMessenger->DeclareProperty("control/execute vis.mac",placeHolder,"Revisualization");
	fMessenger->DeclareProperty("control/execute rebuild.mac",placeHolder,"Reconstruction");
	fMessenger->DeclareProperty("setFileName0", file_name0, 
								"Set the name of output root file");
	fMessenger->DeclareProperty("setFileName1", file_name1, 
								"Set the name of output root file");
	fMessenger->DeclarePropertyWithUnit("block_thickness_half", "mm", block_thickness_half,
										"Set the block_thickness_half");

	fMessenger = new G4GenericMessenger(this, "/PositroniumProject/MaterialTest/", "Material Test");
	fMessenger->DeclareProperty("isMaterialTest", isMaterialTest, "Turn on/off MaterialTest");

	fMessenger = new G4GenericMessenger(this, "/PositroniumProject/Source/", "Source control");
	fMessenger->DeclareProperty("isSource", isSource, "Construct the whole Souce");
	fMessenger->DeclareProperty("isBareSource", isBareSource, "Construct the BareSouce");
	fMessenger->DeclareProperty("isDisk", isDisk, "Construct the Disk");
	fMessenger->DeclareProperty("isRing", isRing, "Construct the Ring");

	fMessenger = new G4GenericMessenger(this, "/PositroniumProject/PositroniumSource/",
	"PositroniumSource control");
	fMessenger->DeclareProperty("isLAB_Acrylic", isLAB_Acrylic, 
	"Construct the acrylic with liquid scintillator LAB inside.");

	fMessenger = new G4GenericMessenger(this, "/PositroniumProject/CsI/", "CsI control");
	fMessenger->DeclareProperty("isOffset", isOffset, "Turn on/off Offset-type");
	fMessenger->DeclareProperty("isCross", isCross, "Turn on/off Cross-type");
}

// Construct All physical volumes
G4VPhysicalVolume* MyDetectorConstruction::Construct() {
	G4double xWorld = 0.75*m;
	G4double yWorld = 0.75*m;
	G4double zWorld = 0.75*m;

	// A cubic world with volume 1.5 m*1.5 m*1.5 m
	G4Box* solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
	logicWorld = new G4LogicalVolume(solidWorld, matWorld, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld,
								  "physWorld", 0, false, 0, true);

	// Material Test
	if (isMaterialTest)
		ConstructMaterialTest();

	// Positron Source
	if (isSource) {
		ConstructRing();
		ConstructDisk();
		ConstructBareSource(isSource);
	}
	else {
		if (isRing)
			ConstructRing();
		if (isDisk)
			ConstructDisk();
		if (isBareSource)
			ConstructBareSource(isDisk);
	}
	if (isBSD_Ring_Dt)
		ConstructBSD_Ring_Detector();

	// Positronium Source
	if (isLAB_Acrylic)
		ConstructLAB_Acrylic();

	// CsI Detector
	if (isOffset)
		ConstructOffset();
	if (isCross)
		ConstructCross();

	return physWorld;
}

// TestBlock and Detector
void MyDetectorConstruction::ConstructMaterialTest() {
	// TestBlock
	G4Box* solidTestBlock = new G4Box("solidTestBlock", block_sideLength_half,
									  block_sideLength_half, block_thickness_half);
	logicTestBlock = new G4LogicalVolume(solidTestBlock, matTestBlock, "logicTestBlock");
	physTestBlock = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m),
									  logicTestBlock, "TestBlock", logicWorld, false, 0, true);
	// End TestBlock
}

// Positron Source
void MyDetectorConstruction::ConstructRing() {
	G4Tubs* solidRing = new G4Tubs("solidRing", disk_radius, ring_radius, 
								   ring_height_half, 0.*deg, 360.*deg);
	logicRing = new G4LogicalVolume(solidRing, matRing, "logicRing");
	physRing = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m),
								 logicRing, "Ring", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructDisk() {
	G4Tubs* solidDisk = new G4Tubs("solidDisk", 0.*nm, disk_radius, 
								   disk_height_half, 0.*deg, 360.*deg);
	logicDisk = new G4LogicalVolume(solidDisk, matDisk, "logicDisk");
	physDisk = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m),
								 logicDisk, "Disk", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructBareSource(G4bool matSelect) {
	G4LogicalVolume* logicMother = logicWorld;
	if (matSelect)
		logicMother = logicDisk;
	G4Tubs* solidBareSource = new G4Tubs("solidBareSource", 0.*nm,bare_source_radius, 
										 bare_source_height_half, 0.*deg, 360.*deg);
	logicBareSource = new G4LogicalVolume(solidBareSource, matSource, "logicBareSource");
	physBareSource = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicBareSource,
									   "BareSource", logicMother, false, 0, true);
}
void MyDetectorConstruction::ConstructBSD_Ring_Detector() {
	G4double thickness = 1.*nm;
	G4Tubs *solidA = new G4Tubs("A", disk_radius-thickness/2, ring_radius+thickness/2, 
								ring_height_half+thickness/2, 0.*deg, 360.*deg);
	G4Tubs *solidB = new G4Tubs("B", 0.*nm, disk_radius, disk_height_half+thickness/2, 0.*deg, 360.*deg);
	G4UnionSolid *solidInside = new G4UnionSolid("solidInside", solidA, solidB);
	G4Tubs *solidC = new G4Tubs("C", disk_radius-thickness, ring_radius+thickness, 
								ring_height_half+thickness, 0.*deg, 360.*deg);
	G4Tubs *solidD = new G4Tubs("D", 0.*nm, disk_radius, disk_height_half+thickness, 0.*deg, 360.*deg);
	G4UnionSolid *solidOutside = new G4UnionSolid("solidInside", solidC, solidD);

	G4SubtractionSolid *solidBSD_Ring_Dt = new G4SubtractionSolid("solidBSD_Ring_Dt",
	solidOutside, solidInside);
	logicBSD_Ring_Dt = new G4LogicalVolume(solidBSD_Ring_Dt, matWorld, "logicBSD_Ring_Dt");
	physBSD_Ring_Dt = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicBSD_Ring_Dt,
										"BSD_Ring_Dt", logicWorld, false, 0, true);
}

// Positronium Source
void MyDetectorConstruction::ConstructLAB_Acrylic() {
	G4double angle_end = 360*degree;
	G4double thickness = 1*mm;

	G4Tubs* solidA = new G4Tubs("solidA", 0.*mm, 29.55*mm, 10/2*mm, 0.*deg, angle_end);
	G4Tubs* solidB = new G4Tubs("solidB", 0.*mm, 29.55*mm-thickness, 
								(10/2-thickness)/2*mm, 0.*deg, angle_end);
	G4Cons* solidC = new G4Cons("solidC", 0.*mm, 4.765*mm, 0.*mm, 9.55*mm, 
								3.746/2*mm, 0.*deg, angle_end);
	G4Tubs* solidD = new G4Tubs("solidD", 0.*mm, 4.765*mm, 0.5/2*mm, 0.*deg, angle_end);
	G4Tubs* solidE = new G4Tubs("solidE", 0.*mm, 9.55*mm, 0.256/2*mm, 0.*deg, angle_end);

	G4Translate3D transC(G4ThreeVector(0.*mm, 0.*mm, -(4/2+3.746/2)*mm));
	G4UnionSolid* solidBC = new G4UnionSolid("solidBC", solidB, solidC, transC);

	G4Translate3D transD(G4ThreeVector(0.*mm, 0.*mm, -(4/2+0.5/2+3.746)*mm));
	G4UnionSolid* solidBCD = new G4UnionSolid("solidBCD", solidBC, solidD, transD);

	G4Translate3D transE(G4ThreeVector(0.*mm, 0.*mm, (0.256/2-0.002-5)*mm));
	G4SubtractionSolid* solidA_E = new G4SubtractionSolid("solidA_E", solidA, solidE, transE);

	G4LogicalVolume* logicContainer = new G4LogicalVolume(solidA_E, Acrylic, "logicContainer");
	G4LogicalVolume* logicLAB = new G4LogicalVolume(solidBCD, LAB, "logicBCD");

	G4Translate3D transZ(G4ThreeVector(0.*m, 0.*m, 5.*mm));
	G4Rotate3D rotY_90_2(90.*2*deg, G4ThreeVector(0.,1.,0.));

	physContainer_F = new G4PVPlacement(transZ, logicContainer, "physContainer_F", 
										logicWorld, false, 0, true);
	physContainer_B = new G4PVPlacement(rotY_90_2*transZ, logicContainer, 
										"physContainer_B", logicWorld, false, 0, true);
	physLAB = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 2.*mm), logicLAB, 
							    "physLAB", logicContainer, false, 0, true);
}

// CsI Detector
void MyDetectorConstruction::ConstructOffset() {
	G4double CsI_sidelength_half = 0.05/2*m;
	G4double CsI_height_half = 0.5/2*m;
	G4double Cable_gap = 1*cm;

	G4Box* solidCsI = new G4Box("solidCsI", CsI_sidelength_half, CsI_sidelength_half,  
								CsI_height_half);
	logicCsI = new G4LogicalVolume(solidCsI, CsI, "logicCsI");

	G4double Mylar_sidelength_half = CsI_sidelength_half + wrapping_thickness;
	G4Box* solidMylar = new G4Box("solidMylar", Mylar_sidelength_half,
								  Mylar_sidelength_half,  CsI_height_half);
	logicMylar = new G4LogicalVolume(solidMylar, Mylar, "logicMylar");

	// Define parameters
	G4int number_of_side = 4;
	G4int number_at_ends = 10;
	G4double delta_theta = 360.*deg/number_of_side;
	G4double shift_X_Layer1 = 10*cm;
	G4double shift_Y_Layer1 = 10*Mylar_sidelength_half - (shift_X_Layer1 - Cable_gap);
	G4double shift_X_Layer2 = 2*Mylar_sidelength_half;
	G4double shift_distance = Mylar_sidelength_half*(number_at_ends-1.);

	// Transformation
	G4Rotate3D rotX_90(90.*deg, G4ThreeVector(1., 0., 0.));
	G4Rotate3D rotY_90(90.*deg, G4ThreeVector(0., 1., 0.));
	G4Translate3D transX_shift(G4ThreeVector(shift_X_Layer1, 0*m, 0*m));
	G4Translate3D transX_Mylar_half(G4ThreeVector(Mylar_sidelength_half, 0*m, 0*m));
	G4Translate3D transY(G4ThreeVector(0*m, shift_Y_Layer1, 0*m));

	G4Transform3D T_Layer1, T_Layer2, T_Layer3;
	T_Layer1 = transX_Mylar_half*transY*transX_shift*rotX_90;
	T_Layer2 = transX_Mylar_half*transX_Mylar_half*T_Layer1;
	T_Layer3 = transX_Mylar_half*transX_Mylar_half*T_Layer2;
	// Transformation

	// Curved 4-surfaces of the Cuboid
	for (G4int i = 0; i < number_of_side; i++) {
		G4Rotate3D rotZ_theta(delta_theta*i, G4ThreeVector(0., 0., 1.));
		for (G4int j = 0; j < number_at_ends; j++) {
			G4Translate3D stacking(G4ThreeVector(0*m, -shift_distance+2*Mylar_sidelength_half*j, 0*m));
			physMylar = new G4PVPlacement(rotZ_theta*T_Layer1*stacking, logicMylar, 
										  "physMylar_1", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(rotZ_theta*T_Layer3*stacking, logicMylar, 
										  "physMylar_3", logicWorld, false, i, true);
			if (j > 0) {
				G4Translate3D stacking(G4ThreeVector(0*m, -shift_distance+2*Mylar_sidelength_half*(j-.5), 0*m));
				physMylar = new G4PVPlacement(rotZ_theta*T_Layer2*stacking, logicMylar, 
											  "physMylar_2", logicWorld, false, i, true);
			}
		}
	}

	// Define parameters
	G4double shift_Z_Layer1 = CsI_height_half + Cable_gap;
	shift_distance = Mylar_sidelength_half*(number_at_ends-1.);

	//Transformation
	G4Translate3D transZ_shift(G4ThreeVector(0*m, 0*m, shift_Z_Layer1));
	G4Translate3D transZ_Mylar_half(G4ThreeVector(0*m, 0*m, Mylar_sidelength_half));
	T_Layer1 = transZ_Mylar_half*transZ_shift*rotY_90*rotX_90;
	T_Layer2 = transZ_Mylar_half*transZ_Mylar_half*T_Layer1;
	T_Layer3 = transZ_Mylar_half*transZ_Mylar_half*T_Layer2;

	// 2 ends of the Cuboid
	for (G4int i = 0; i < 2; i++) {
		G4Rotate3D rotY(180.*i*deg, G4ThreeVector(0., 1., 0.));
		for (G4int j = 0; j <= number_at_ends; j++) {
			if (j < number_at_ends) {
				G4Translate3D stacking(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*j, 0*m, 0*m));
				physMylar = new G4PVPlacement(rotY*stacking*T_Layer1, logicMylar, 
											  "physMylar_End_1", logicWorld, false, i, true);
				physMylar = new G4PVPlacement(rotY*stacking*T_Layer3, logicMylar,
											  "physMylar_End_3", logicWorld, false, i, true);
			}
			G4Translate3D stacking(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*(j-.5), 0*m, 0*m));
			physMylar = new G4PVPlacement(rotY*stacking*T_Layer2, logicMylar,
										  "physMylar_End_2", logicWorld, false, i, true);
		}
	}
	physCsI = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicCsI,
								"physCsI", logicMylar, false, 0, true);
}
void MyDetectorConstruction::ConstructCross() {
	G4double CsI_sidelength_half = 0.05/2*m;
	G4double CsI_height_half = 0.5/2*m;
	G4double Cable_gap = 1*cm;

	G4Box* solidCsI = new G4Box("solidCsI", CsI_sidelength_half, CsI_sidelength_half,  CsI_height_half);
	logicCsI = new G4LogicalVolume(solidCsI, CsI, "logicCsI");

	G4double Mylar_sidelength_half = CsI_sidelength_half + wrapping_thickness;
	G4Box* solidMylar = new G4Box("solidMylar", Mylar_sidelength_half, 
								  Mylar_sidelength_half,  CsI_height_half);
	logicMylar = new G4LogicalVolume(solidMylar, Mylar, "logicMylar");

	// Define parameters
	G4int number_of_side = 4;
	G4int number_at_ends = 10;
	G4double delta_theta = 360.*deg/number_of_side;
	G4double shift_X_Layer1 = 10*cm;
	G4double shift_Y_Layer1 = 10*Mylar_sidelength_half - (shift_X_Layer1 - Cable_gap);
	G4double shift_X_Layer2 = 2*Mylar_sidelength_half;
	G4double shift_distance = Mylar_sidelength_half*(number_at_ends-1.);
	//Transformation
	G4Rotate3D rotX_90(90.*deg, G4ThreeVector(1., 0., 0.));
	G4Translate3D transX_Layer1(G4ThreeVector(shift_X_Layer1 + Mylar_sidelength_half, 0*m, 0*m));
	G4Translate3D transX_Layer2(G4ThreeVector(shift_X_Layer2, 0*m, 0*m));
	G4Translate3D transY(G4ThreeVector(0*m, shift_Y_Layer1, 0*m));
	G4Transform3D T_Layer1 = transX_Layer1;
	G4Transform3D T_Layer2 = transX_Layer2*transX_Layer1;
	G4Transform3D T_Layer3 = transX_Layer2*transX_Layer2*T_Layer1;

	// Curved surface of the Cylinder
	for (G4int i = 0; i < number_of_side; i++) {
		for (G4int j = 0; j < number_at_ends; j++) {
			G4Translate3D stacking(G4ThreeVector(0*m, -shift_distance+2*Mylar_sidelength_half*j, 0*m));
			G4Rotate3D rotZ_theta(delta_theta*i, G4ThreeVector(0., 0., 1.));
			physMylar = new G4PVPlacement(rotZ_theta*transY*stacking*T_Layer1, logicMylar,
										  "physMylar_1", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(rotZ_theta*transY*rotX_90*stacking*T_Layer2, logicMylar,
										  "physMylar_2", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(rotZ_theta*transY*stacking*T_Layer3, logicMylar,
										  "physMylar_3", logicWorld, false, i, true);
		}
	}

	// Define parameters
	G4double shift_Z_Layer1 = CsI_height_half + Cable_gap + Mylar_sidelength_half;
	G4double shift_Z_Layer2 = 2*Mylar_sidelength_half;
	shift_distance = Mylar_sidelength_half*(number_at_ends-1.);
	//Transformation
	G4Rotate3D rotZ_90(90*deg, G4ThreeVector(0., 0., 1.));
	G4Translate3D transZ_Layer1(G4ThreeVector(0*m, 0*m, shift_Z_Layer1));
	G4Translate3D transZ_Layer2(G4ThreeVector(0*m, 0*m, shift_Z_Layer2));
	T_Layer1 = transZ_Layer1;
	T_Layer2 = transZ_Layer2*transZ_Layer1;
	T_Layer3 = transZ_Layer2*transZ_Layer2*transZ_Layer1;

	// 2 ends of the Cuboid
	for (G4int i = 0; i < 2; i++) {
		G4Rotate3D rotY(180.*i*deg, G4ThreeVector(0., 1., 0.));
		for (G4int j = 0; j < number_at_ends; j++) {
			G4Translate3D stacking(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*j,
								   0*m, 0*m));
			physMylar = new G4PVPlacement(rotY*stacking*T_Layer1*rotX_90, logicMylar,
										  "physMylar_End_1", logicWorld, false, j, true);
			physMylar = new G4PVPlacement(rotY*rotZ_90*stacking*T_Layer2*rotX_90, logicMylar,
										  "physMylar_End_2", logicWorld, false, j, true);
			physMylar = new G4PVPlacement(rotY*stacking*T_Layer3*rotX_90, logicMylar,
										  "physMylar_End_3", logicWorld, false, j, true);
		}
	}
	physCsI = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicCsI, 
								"physCsI", logicMylar, false, 0, true);
}