#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() {
	// Define required materials
	DefineMaterials();

	// Here to control the construction selector to select which part should be constructed
	// Radioactive Source (Positron Source)
	isRing = false;
	isDisk = false;
	isBareSource = false;
	isSource = true;
	// Dt = Detector, only need to construct one of them
	isBareSource_Dt = false;
	isBS_Disk_Dt = false;
	isBSD_Ring_Dt = false;

	// Positronium Source
	isLiquid = false;
	isCupDetector = false;
	isLAB_Acrylic = false;

	isDetector_Shell = true;
	isDetector_Cylinder = false;

	// CsI crystal detector
	isCsI = false;
	isCsI_2 = false;
	isCsI_3 = false;
	isCsI_4 = false;

	// Set the material for each logical volume
	matWorld = Air; //Vacuum;
	matRing = Ti_, matDisk = Ti_, matSource = NaCl;
	matContainer = Ti_, matLiquid = H2O;

	// Set the default of each logical volume to be NULL so the sensitive detector selector can work well
	logicDetector_Shell = NULL;
	logicDetector_Cylinder = NULL;
	logicDetector_Cylinder_Ends = NULL;

	// Parameters for adjusting size of the shape of predefined objects
	ring_radius = 19.1/2*mm;
	ring_height_half = 0.254*mm;			//Supported by two 0.254 mm Ti disks
	disk_radius = 9.53/2*mm;
	disk_height_half = 0.00508*mm;			//The activity is placed between two layers of 0.00508*mm Ti foil which is 0.0102*mm in total
	bare_source_radius = disk_radius;
	bare_source_height_half = 0.0001*mm;	//The thickness of bare source is not provide so this is a made up value
	container_radius = 2*cm;				//All variables of container are made up.
	container_height_half = 2/2*cm;
	container_thickness = 1.*mm;
	d_pos_z = 0.02*mm;						//Distance between two nearest plane detectors, spacing of two plane detectors

	wrapping_thickness = 13.*um;			//The wrapping thickness of CsI crystals, 13.e-6 m = 13. um

	number_of_side = 10;					// number_of_side should be >= 3
	delta_phi = 0.*deg;						// rotate 0, 15, 30, 45 deg
	end_selector = 0;						// Two ends of the Cylinder for CsI_2, 0 is to not construct; 1 is cross type; 2 is offset type; 3 is offset_plus type; 4 is cross type 3 layer,
	
	DefineMessenger();
}
MyDetectorConstruction::~MyDetectorConstruction()
{}

G4String MyDetectorConstruction::file_name = "";

void MyDetectorConstruction::DefineMaterials() {
	G4NistManager* nist = G4NistManager::Instance();
	G4double hc = 1239.841939 * eV;

	Air = nist->FindOrBuildMaterial("G4_AIR");
	Vacuum = nist->FindOrBuildMaterial("G4_Galactic");

	// Aerogel
	SiO2 = new G4Material("SiO2", 0.125*g/cm3, 2);
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

	H2O = new G4Material("H2O", 1.000*g/cm3, 2);
	H2O->AddElement(nist->FindOrBuildElement("H"), 2);
	H2O->AddElement(nist->FindOrBuildElement("O"), 1);

	C = nist->FindOrBuildElement("C");

	Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
	Aerogel->AddMaterial(SiO2, 62.5*perCent);
	Aerogel->AddMaterial(H2O, 37.4*perCent);
	Aerogel->AddElement(C, 0.1*perCent);

	G4double energyAerogel[2] = { hc/900, hc/200 };		// 900nm, 200nm
	G4double rindexAerogel[2] = { 1.1, 1.1 };
	G4MaterialPropertiesTable* mptAerogel = new G4MaterialPropertiesTable();
	mptAerogel->AddProperty("RINDEX", energyAerogel, rindexAerogel, 2);
	Aerogel->SetMaterialPropertiesTable(mptAerogel);
	// End Aerogel

	G4double rindexWorld[2] = {1.0, 1.0};
	G4MaterialPropertiesTable* mptAir = new G4MaterialPropertiesTable();
	mptAir->AddProperty("RINDEX", energyAerogel, rindexWorld, 2);
	Air->SetMaterialPropertiesTable(mptAir);

	//NaCl
	NaCl = new G4Material("NaCl", 2.16*g/cm3, 2);
	NaCl->AddElement(nist->FindOrBuildElement("Na"), 1);
	NaCl->AddElement(nist->FindOrBuildElement("Cl"), 4);
	// End NaCl

	// CsI
	CsI = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
	G4double energyCsI[1] = { hc/310 };					//310nm
	G4double rindexCsI[1] = { 1.95 };
	//G4MaterialPropertiesTable* mptCsI = new G4MaterialPropertiesTable();
	//mptCsI->AddProperty("RINDEX", energyCsI, rindexCsI, 1);

	/*
	//Scintillation Test (test data for CsI)
	G4double scintCsI[1] = { 0.1 };
	G4double fractionCsI[1] = { 1.0 };						//Intensity fraction of different wavelengths which are the component of scintillation intensity eg 30./keV
	mptCsI->AddConstProperty("RESOLUTIONSCALE", 1.);				// AddConstProperty for a number
	//mptCsI->AddConstProperty("SCINTILLATIONYIELD", 30./keV);
	mptCsI->AddProperty("SCINTILLATIONCOMPONENT1", energyCsI, fractionCsI, 1);	// AddProperty for a array
	mptCsI->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1500.0*ns);	// Time constant is the decay time	
	mptCsI->AddConstProperty("SCINTILLATIONYIELD1", 30./keV);
	//End Scintillation Test
	*/

	//CsI->SetMaterialPropertiesTable(mptCsI);

	Pb = nist->FindOrBuildMaterial("G4_Pb");
	/*Pb_ = nist->FindOrBuildElement("Pb");
	Pb = new G4Material("Pb_", 11.35*g/cm3, 1);
	Pb->AddElement(Pb_, 1.);*/
	// End CsI

	// Mylar
	Mylar = nist->FindOrBuildMaterial("G4_MYLAR");
	// End Mylar
	
	// Kapton
	Kapton = nist->FindOrBuildMaterial("G4_KAPTON");
	// End Kapton

	// Ti_
	Ti = nist->FindOrBuildElement("Ti");
	Ti_ = new G4Material("Ti_", 4.507*g/cm3, 1);	//The density of G4_Ti is 4.54*g/cm3
	Ti_->AddElement(Ti, 1.);
	// End Ti_

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

	G4double energyLAB[6] = { hc/627.3, hc/579.8, hc/547.7, hc/493.9, hc/438.5, hc/405.1};		// hc/wavelength(in nm)
	G4double rindexLAB[6] = { 1.4786, 1.48118, 1.48329, 1.48786, 1.49505, 1.50061 };
	G4MaterialPropertiesTable* mptLAB = new G4MaterialPropertiesTable();
	mptLAB->AddProperty("RINDEX", energyLAB, rindexLAB, 2);
	LAB->SetMaterialPropertiesTable(mptLAB);
	// End LAB

	// Acrylic, from examples>advanced>air_shower>UltraDetectorConstruction.cc
	Acrylic = new G4Material("Acrylic", 1.19*g/cm3, 3);
	Acrylic->AddElement(C, 5);
	Acrylic->AddElement(nist->FindOrBuildElement("H"), 8);
	Acrylic->AddElement(nist->FindOrBuildElement("O"), 2);
	// End Acrylic
}

void MyDetectorConstruction::DefineMessenger() {
	// The A is a placeholder for the user defined commands (fMessenger)
	G4int placeHolder = 0;
	// These are user defined commands for use in User-Interface(UI) mode and batch mode(using macro file)
	fMessenger = new G4GenericMessenger(this, "/MyDetector/", "Macros");
	fMessenger->DeclareProperty("control/execute region_setup.mac", placeHolder, "Set the active region (cylinder locate at origin, radius = 9.53/2*mm, half height = 0.0001*mm)");
	fMessenger->DeclareProperty("control/execute rebuild.mac",placeHolder,"Rebuild Selected Physical Volume inside a 1.5*1.5*1.5 m^3 Cubic World contains Air, its center is the origin");
	fMessenger->DeclareProperty("isDetector_Shell", isDetector_Shell, "Construct Shell Detector (spherical shell locate at origin, inner radius = 3*cm, thickness = 1*nm)");
	fMessenger->DeclareProperty("setFileName", file_name, "Set the name of output root file");

	fMessenger = new G4GenericMessenger(this, "/MyDetector/Source/", "Source control");
	fMessenger->DeclareProperty("isSource", isSource, "Construct the whole Souce");
	fMessenger->DeclareProperty("isBareSource", isBareSource, "Construct the BareSouce (cylinder locate at origin, radius = 9.53/2*mm, half height = 0.0001 mm)");
	fMessenger->DeclareProperty("isDisk", isDisk, "Construct the Disk (cylinder locate at origin, radius = 9.53/2*mm, half height = 0.00508 mm)");
	fMessenger->DeclareProperty("isRing", isRing, "Construct the Ring (tube locate at origin, inner radius = 9.53/2*mm, outer radius = 19.1/2*mm, half height = 0.254 mm)");

	fMessenger = new G4GenericMessenger(this, "/MyDetector/Container/", "Container control");
	fMessenger->DeclareProperty("isLiquid", isLiquid, "Construct the Ti-Container and Water (2 cylinders locate in front and back of the Source, radius = 19.1/2*mm, half height = 1 cm)");
	fMessenger->DeclareProperty("isCupDetector", isCupDetector, "Select the detectors inside the Ti-Container and Water (including Plane Detector and Tube Detector, the Plane Detector has spacing = 0.02 mm)");
	fMessenger->DeclarePropertyWithUnit("container_thickness", "mm", container_thickness, "Set the thickness of container. Default = 1 mm.");
	fMessenger->DeclarePropertyWithUnit("d_pos_z", "mm", d_pos_z,"Set the spacing between two nearest plane detectors. Default = 0.02 mm, Default Amount of Plane Detetors = (int)(2*mm/d_pos_z) = 100");

	fMessenger = new G4GenericMessenger(this, "/MyDetector/CsI/", "CsI control");
	fMessenger->DeclarePropertyWithUnit("self_rotation", "deg", delta_phi, "Set the rotation angle of CsI. Default = 0 deg");
	fMessenger->DeclareProperty("number_of_side", number_of_side, "Set the side number of regular polygon formed by CsI. Default = 10, number_of_side should be >= 3");
	fMessenger->DeclareProperty("isCsI_2", isCsI_2, "Turn on/off");
	fMessenger->DeclareProperty("end_selector", end_selector, "Select two ends of the Cylinder. Default = 0, 0 is to not construct; 1 is cross type; 2 is offset type; 3 is offset_plus type; 4 is cross type 3 layer,");
}

// Construct All physical volumes
G4VPhysicalVolume* MyDetectorConstruction::Construct() {
	G4double xWorld = 0.75*m;
	G4double yWorld = 0.75*m;
	G4double zWorld = 0.75*m;

	// A cubic world with volume 1.5 m*1.5 m*1.5 m
	G4Box* solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
	logicWorld = new G4LogicalVolume(solidWorld, matWorld, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

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
	if (isBareSource_Dt)
		ConstructBareSource_Detector();
	if (isBS_Disk_Dt)
		ConstructBS_Disk_Detector();
	if (isBSD_Ring_Dt)
		ConstructBSD_Ring_Detector();

	if (isLiquid)
		ConstructLiquidScintillator();
	if (isCupDetector)
		ConstructCup_Detector();
	if (isLAB_Acrylic)
		ConstructLAB_Acrylic();

	if (isDetector_Shell)
		ConstructShell_Detector();
	if (isDetector_Cylinder)
		ConstructCylinder_Detector();

	if (isCsI)
		ConstructCsI();
	if (isCsI_2)
		ConstructCsI_2();
	if (isCsI_3)
		ConstructCsI_3();
	if (isCsI_4)
		ConstructCsI_4();

	return physWorld;
}

// Set Sensitive Detector(SD) and Field
void MyDetectorConstruction::ConstructSDandField() {
	MySensentiveDetector* sensDet = new MySensentiveDetector("SensitiveDetector");
	if(logicDetector_Shell != NULL)
		logicDetector_Shell->SetSensitiveDetector(sensDet);
	if(logicDetector_Cylinder != NULL)
		logicDetector_Cylinder->SetSensitiveDetector(sensDet);
	if(logicDetector_Cylinder_Ends != NULL)
		logicDetector_Cylinder_Ends->SetSensitiveDetector(sensDet);
}

// Positron Source
void MyDetectorConstruction::ConstructRing() {
	G4Tubs* solidRing = new G4Tubs("solidRing", disk_radius, ring_radius, ring_height_half, 0.*deg, 360.*deg);
	logicRing = new G4LogicalVolume(solidRing, matRing, "logicRing");

	G4Translate3D trans(G4ThreeVector(0*m, 0*m, 0*cm));
	G4Rotate3D rotY(90.*deg, G4ThreeVector(0.,1.,0.));
	G4Transform3D tranTest = rotY*trans;

	physRing = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicRing, "Ring", logicWorld, false, 0, true);
	//physRing = new G4PVPlacement(tranTest, logicRing, "Ring", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructDisk() {
	G4Tubs* solidDisk = new G4Tubs("solidDisk", 0.*nm, disk_radius, disk_height_half, 0.*deg, 360.*deg);
	logicDisk = new G4LogicalVolume(solidDisk, matDisk, "logicDisk");

	G4Translate3D trans(G4ThreeVector(0*m, 0*m, 0*cm));
	G4Rotate3D rotY(90.*deg, G4ThreeVector(0.,1.,0.));
	G4Transform3D tranTest = rotY*trans;
	physDisk = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicDisk, "Disk", logicWorld, false, 0, true);
	//physDisk = new G4PVPlacement(tranTest, logicDisk, "Disk", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructBareSource(G4bool matSelect) {
	G4LogicalVolume* logicMother = logicWorld;
	if (matSelect)
		logicMother = logicDisk;
	G4Tubs* solidBareSource = new G4Tubs("solidBareSource", 0.*nm, bare_source_radius, bare_source_height_half, 0.*deg, 360.*deg);
	logicBareSource = new G4LogicalVolume(solidBareSource, matSource, "logicBareSource");
	physBareSource = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicBareSource, "BareSource", logicMother, false, 0, true);
}
void MyDetectorConstruction::ConstructBareSource_Detector() {
	G4double thickness = 1.*nm;
	G4Tubs *solidInside = new G4Tubs("solidInside", 0.*nm, bare_source_radius+thickness/2, bare_source_height_half+thickness/2, 0.*deg, 360.*deg);
	G4Tubs *solidOutside = new G4Tubs("solidOutside", 0.*nm, bare_source_radius+thickness, bare_source_height_half+thickness, 0.*deg, 360.*deg);
	G4SubtractionSolid *solidBareSource_Dt = new G4SubtractionSolid("solidBareSource_Dt", solidOutside, solidInside);
	logicBareSource_Dt = new G4LogicalVolume(solidBareSource_Dt, matWorld, "logicBareSource_Dt");
	physBareSource_Dt = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicBareSource_Dt, "BareSource_Dt", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructBS_Disk_Detector() {
	G4double thickness = 1.*nm;
	G4Tubs *solidInside = new G4Tubs("solidInside", 0.*nm, disk_radius+thickness/2, disk_height_half+thickness/2, 0.*deg, 360.*deg);
	G4Tubs *solidOutside = new G4Tubs("solidOutside", 0.*nm, disk_radius+thickness, disk_height_half+thickness, 0.*deg, 360.*deg);
	G4SubtractionSolid *solidBS_Disk_Dt = new G4SubtractionSolid("solidBS_Disk_Dt", solidOutside, solidInside);
	logicBS_Disk_Dt = new G4LogicalVolume(solidBS_Disk_Dt, matWorld, "logicBS_Disk_Dt");
	physBS_Disk_Dt = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicBS_Disk_Dt, "BS_Disk_Dt", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructBSD_Ring_Detector() {
	G4double thickness = 1.*nm;
	G4Tubs *solidA = new G4Tubs("A", disk_radius, ring_radius+thickness/2, ring_height_half+thickness/2, 0.*deg, 360.*deg);				//Same as solidRing
	G4Tubs *solidB = new G4Tubs("B", 0.*nm, disk_radius, disk_height_half+thickness/2, 0.*deg, 360.*deg);								//Same as solidDisk
	G4UnionSolid *solidInside = new G4UnionSolid("solidInside", solidA, solidB);
	G4Tubs *solidOutside = new G4Tubs("solidOutside", 0.*nm, ring_radius+thickness, ring_height_half+thickness, 0.*deg, 360.*deg);

	G4SubtractionSolid *solidBSD_Ring_Dt = new G4SubtractionSolid("solidBSD_Ring_Dt", solidOutside, solidInside);
	logicBSD_Ring_Dt = new G4LogicalVolume(solidBSD_Ring_Dt, matWorld, "logicBSD_Ring_Dt");
	physBSD_Ring_Dt = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicBSD_Ring_Dt, "BSD_Ring_Dt", logicWorld, false, 0, true);
}
// End Positron Source

// Liquid scintillator and CupDetector
void MyDetectorConstruction::ConstructLiquidScintillator(){
	G4Tubs* solidContainer = new G4Tubs("LiquidContainer", 0.*nm, container_radius, container_height_half, 0.*deg, 360.*deg);
	G4Tubs* solidLiquid = new G4Tubs("Liquid", 0.*nm, container_radius-container_thickness, container_height_half-container_thickness, 0.*deg, 360.*deg);

	logiContainer_F = new G4LogicalVolume(solidContainer, matContainer, "logiContainer");
	logiContainer_B = new G4LogicalVolume(solidContainer, matContainer, "logiContainer");
	logicLiquid_F = new G4LogicalVolume(solidLiquid, matLiquid, "logicLiquid");
	logicLiquid_B = new G4LogicalVolume(solidLiquid, matLiquid, "logicLiquid");

	G4double container_z_shift = ring_height_half+container_height_half;

	G4Translate3D transZ(G4ThreeVector(0.*m, 0.*m, container_z_shift));
	G4Rotate3D rotY_90_2(90.*2*deg, G4ThreeVector(0.,1.,0.));

	physContainer_F = new G4PVPlacement(transZ, logiContainer_F, "Container_F", logicWorld, false, 0, true);
	physContainer_B = new G4PVPlacement(rotY_90_2*transZ, logiContainer_B, "Container_B", logicWorld, false, 0, true);
	physLiquid_F = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicLiquid_F, "Liquid", logiContainer_F, false, 0, true);
	physLiquid_B = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicLiquid_B, "Liquid", logiContainer_B, false, 0, true);
}
void MyDetectorConstruction::ConstructCup_Detector() {
	// TubeDetector
	G4Tubs* solidTubeDetector = new G4Tubs("solidTubeDetector", container_radius, container_radius+1.*nm, ring_height_half+2*container_height_half, 0.*deg, 360.*deg);
	logicTubeDetector = new G4LogicalVolume(solidTubeDetector, matWorld, "logicTubeDetector");
	physTubeDetector = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0*m), logicTubeDetector, "TubeDetector", logicWorld, false, 0, true);
	// End TubeDetector

	// PlaneDetector
	G4double plane_radius = container_radius-container_thickness;
	G4double plane_height_half = 1*nm/2;

	G4Tubs* solidPlaneDetector = new G4Tubs("solidPlaneDetector", 0.*nm, plane_radius, plane_height_half, 0.*deg, 360.*deg);
	logicPlaneDetector_W = new G4LogicalVolume(solidPlaneDetector, matWorld, "logicPlaneDetector_W");
	logicPlaneDetector_C = new G4LogicalVolume(solidPlaneDetector, matContainer, "logicPlaneDetector_C");
	logicPlaneDetector_L = new G4LogicalVolume(solidPlaneDetector, matLiquid, "logicPlaneDetector_L");
	G4Tubs* solidRingDetector = new G4Tubs("solidRingDetector", plane_radius+1.*nm, container_radius, plane_height_half, 0.*deg, 360.*deg);
	logicRingDetector = new G4LogicalVolume(solidRingDetector, matContainer, "logicRingDetector");
	
	// To generate 1 planes (index = 0, 1, 2, ...,11) plane and ring detectors
	for (int i = 0; i <= (int)(2*mm/d_pos_z); i++) {
		G4double surf_shift = d_pos_z*i;
		//G4double pos_z = -container_height_half+plane_height_half+surf_shift;
		G4double pos_z = -container_height_half+plane_height_half+surf_shift+0.5*nm;				// The "+0.5*nm" is for avoiding overlaping surface by shifting the plane
		G4Translate3D transZ(G4ThreeVector(0.*m, 0.*m, pos_z));
		G4double pos_z_world = ring_height_half+plane_height_half+surf_shift;
		G4Translate3D transZ_F(G4ThreeVector(0.*m, 0.*m, pos_z_world));
		G4Translate3D transZ_B(G4ThreeVector(0.*m, 0.*m, -pos_z_world));

		// If container_thickness = 1*mm, container_height_half = 2/2*cm, Range of surf_shift = (-inf, 1*mm) [1*mm, 19*mm) [19*mm, 20*mm) [20*mm, inf)
		if (surf_shift < container_thickness) {
			physRingDetector = new G4PVPlacement(transZ, logicRingDetector, "RingDetector_F", logiContainer_F, false, i, true);
			physRingDetector = new G4PVPlacement(transZ, logicRingDetector, "RingDetector_B", logiContainer_B, false, i, true);
			physPlaneDetector = new G4PVPlacement(transZ, logicPlaneDetector_C, "PlaneDetector_F", logiContainer_F, false, i, true);
			physPlaneDetector = new G4PVPlacement(transZ, logicPlaneDetector_C, "PlaneDetector_B", logiContainer_B, false, i, true);
		}
		else if (surf_shift >= container_thickness && surf_shift < 2*container_height_half-container_thickness) {
			physRingDetector = new G4PVPlacement(transZ, logicRingDetector, "RingDetector_F", logiContainer_F, false, i, true);
			physRingDetector = new G4PVPlacement(transZ, logicRingDetector, "RingDetector_B", logiContainer_B, false, i, true);
			physPlaneDetector = new G4PVPlacement(transZ, logicPlaneDetector_L, "PlaneDetector_F", logicLiquid_F, false, i, true);
			physPlaneDetector = new G4PVPlacement(transZ, logicPlaneDetector_L, "PlaneDetector_B", logicLiquid_B, false, i, true);
		}
		else if (surf_shift >= 2*container_height_half-container_thickness && surf_shift < 2*container_height_half) {
			physRingDetector = new G4PVPlacement(transZ, logicRingDetector, "RingDetector_F", logiContainer_F, false, i, true);
			physRingDetector = new G4PVPlacement(transZ, logicRingDetector, "RingDetector_B", logiContainer_B, false, i, true);
			physPlaneDetector = new G4PVPlacement(transZ, logicPlaneDetector_C, "PlaneDetector_F", logiContainer_F, false, i, true);
			physPlaneDetector = new G4PVPlacement(transZ, logicPlaneDetector_C, "PlaneDetector_B", logiContainer_B, false, i, true);
		}
		else if (surf_shift >= 2*container_height_half) {
			physRingDetector = new G4PVPlacement(transZ_F, logicRingDetector, "RingDetector_F", logicWorld, false, i, true);
			physRingDetector = new G4PVPlacement(transZ_B, logicRingDetector, "RingDetector_B", logicWorld, false, i, true);
			physPlaneDetector = new G4PVPlacement(transZ_F, logicPlaneDetector_W, "PlaneDetector_F", logicWorld, false, i, true);
			physPlaneDetector = new G4PVPlacement(transZ_B, logicPlaneDetector_W, "PlaneDetector_B", logicWorld, false, i, true);
		}
		// End PlaneDetector
	}
}
void MyDetectorConstruction::ConstructLAB_Acrylic() {
	//G4double angle_end = 360*degree;
	G4double angle_end = 360*degree;
	G4double thickness = 1*mm;

	G4Tubs* solidA = new G4Tubs("solidA", 0.*mm, 29.55*mm, 10/2*mm, 0.*deg, angle_end);
	G4Tubs* solidB = new G4Tubs("solidB", 0.*mm, 29.55*mm-thickness, (10/2-thickness)/2*mm, 0.*deg, angle_end);
	G4Cons* solidC = new G4Cons("solidC", 0.*mm, 4.765*mm, 0.*mm, 9.55*mm, 3.746/2*mm, 0.*deg, angle_end);
	G4Tubs* solidD = new G4Tubs("solidD", 0.*mm, 4.765*mm, 0.5/2*mm, 0.*deg, angle_end);
	G4Tubs* solidE = new G4Tubs("solidE", 0.*mm, 9.55*mm, 0.256/2*mm, 0.*deg, angle_end);		// slightly increase the half_height to ensure full subtraction of solid

	G4Translate3D transC(G4ThreeVector(0.*mm, 0.*mm, -(4/2+3.746/2)*mm));
	G4UnionSolid* solidBC = new G4UnionSolid("solidBC", solidB, solidC, transC);

	G4Translate3D transD(G4ThreeVector(0.*mm, 0.*mm, -(4/2+0.5/2+3.746)*mm));
	G4UnionSolid* solidBCD = new G4UnionSolid("solidBCD", solidBC, solidD, transD);

	G4Translate3D transE(G4ThreeVector(0.*mm, 0.*mm, (0.256/2-0.002-5)*mm));					// 0.002 is the complementary translation for solidE
	G4SubtractionSolid* solidA_E = new G4SubtractionSolid("solidA_E", solidA, solidE, transE);

	G4LogicalVolume* logicContainer = new G4LogicalVolume(solidA_E, Acrylic, "logicContainer");
	G4LogicalVolume* logicLAB = new G4LogicalVolume(solidBCD, LAB, "logicBCD");

	G4Translate3D transZ(G4ThreeVector(0.*m, 0.*m, 5.*mm));
	G4Rotate3D rotY_90_2(90.*2*deg, G4ThreeVector(0.,1.,0.));

	physContainer_F = new G4PVPlacement(transZ, logicContainer, "physContainer_F", logicWorld, false, 0, true);
	physContainer_B = new G4PVPlacement(rotY_90_2*transZ, logicContainer, "physContainer_B", logicWorld, false, 0, true);
	G4VPhysicalVolume* physLAB = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 2.*mm), logicLAB, "physLAB", logicContainer, false, 0, true);
}
// End Liquid scintillator and CupDetector

// Ideal Detector
void MyDetectorConstruction::ConstructCylinder_Detector() {
	G4double cylinder_thickness = 1.*mm;
	G4double cylinder_radius = 55*cm;
	G4double cylinder_height_half = cylinder_radius;
	G4cout << "cylinder_height_half = " << cylinder_height_half << G4endl;

	G4Tubs *solidDetector_Cylinder = new G4Tubs("solidDetector_Cylinder", cylinder_radius, cylinder_radius + cylinder_thickness, cylinder_height_half, 0.*deg, 360.*deg);
	G4Tubs *solidTwo_Ends = new G4Tubs("solidTwo_Ends", 0.*nm, cylinder_radius, cylinder_thickness, 0.*deg, 360.*deg);
	logicDetector_Cylinder = new G4LogicalVolume(solidDetector_Cylinder, matWorld, "logicDetector_Cylinder");
	logicDetector_Cylinder_Ends = new G4LogicalVolume(solidTwo_Ends, matWorld, "logicDetector_Cylinder_Ends");

	physDetector_Cylinder = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicDetector_Cylinder, "Detector_Cylinder", logicWorld, false, 0, true);
	physDetector_Cylinder_Ends = new G4PVPlacement(0, G4ThreeVector(0., 0., cylinder_height_half + cylinder_thickness/2), logicDetector_Cylinder_Ends, "Detector_Cylinder_Ends", logicWorld, false, 0, true);
	physDetector_Cylinder_Ends = new G4PVPlacement(0, G4ThreeVector(0., 0., -(cylinder_height_half + cylinder_thickness/2)), logicDetector_Cylinder_Ends, "Detector_Cylinder_Ends", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructShell_Detector() {
	G4double shell_thickness = 1.*mm;
	//Large sphere
	G4double inner_radius = 10*cm;
	//G4double inner_radius = 70*cm;
	G4double outer_radius = inner_radius + shell_thickness;
	G4Sphere* solidDetector_Shell = new G4Sphere("solidDetector_Shell", inner_radius, outer_radius, 0.*deg, 360.*deg, 0.*deg, 360.*deg);
	logicDetector_Shell = new G4LogicalVolume(solidDetector_Shell, matWorld, "logicDetector_Shell");
	physDetector_Shell = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicDetector_Shell, "Detector_Shell", logicWorld, false, 0, true);
}
// End Ideal Detector

// CsI Detector
void MyDetectorConstruction::ConstructCsI() {
	G4double CsI_sidelength_half = 0.05/2*m;
	G4double CsI_height_half = 0.5/2*m;
	G4Box* solidCsI = new G4Box("solidCsI", CsI_sidelength_half, CsI_sidelength_half,  CsI_height_half);
	logicCsI = new G4LogicalVolume(solidCsI, CsI, "logicCsI");
	//fScoringVolume = logicCsI;

	G4double d = 2*(CsI_sidelength_half+wrapping_thickness);	//Distance between 2 CsI (m)
	G4double nCS = 4;							//Number of Crystals on the Side of Cube
	G4double nExtra = 1;
	G4double nCS_H = nCS+nExtra;				//Horizontal, Front, Back, Left, Right
	G4double nCS_V = nCS+nExtra*2;				//Vertical, Up, Down
	G4double Cube_side_length = nCS*CsI_sidelength_half+(nCS-0.5)*wrapping_thickness+CsI_height_half;
	G4double Cube_side_height = nCS*(CsI_sidelength_half+wrapping_thickness)+CsI_height_half;

	//Create Mylar Wrapping as the mother logicVolume, Add CsI(pure) as the daughter logicVolume
	//Detector_H, H means Horizontal
	G4Box* solidDetector_H = new G4Box("solidDetector_H", (CsI_sidelength_half+wrapping_thickness)*nCS_H,(CsI_sidelength_half+wrapping_thickness)*nCS, CsI_height_half);
	G4LogicalVolume* logicDetector_F = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_F");
	G4LogicalVolume* logicDetector_L = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_L");
	G4LogicalVolume* logicDetector_B = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_B");
	G4LogicalVolume* logicDetector_R = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_R");
	//Transformation
	G4Translate3D transZ_H(G4ThreeVector(-(nCS*CsI_sidelength_half+(nCS-0.5)*wrapping_thickness)+(CsI_sidelength_half+wrapping_thickness)*nCS_H, 0*m, Cube_side_length));
	G4Rotate3D rotY_90(90.*deg, G4ThreeVector(0.,1.,0.));
	G4Rotate3D rotY_90_2(90.*2*deg, G4ThreeVector(0.,1.,0.));
	G4Rotate3D rotY_90_3(90.*3*deg, G4ThreeVector(0.,1.,0.));
	//physDetector_H, F = Front, B = Back, L = Left, R = Right
	physDetector_F = new G4PVPlacement(transZ_H, logicDetector_F, "Detector_F", logicWorld, false, 0, true);
	physDetector_L = new G4PVPlacement((rotY_90)*(transZ_H), logicDetector_L, "Detector_L", logicWorld, false, 0, true);
	physDetector_B = new G4PVPlacement((rotY_90_2)*(transZ_H), logicDetector_B, "Detector_B", logicWorld, false, 0, true);
	physDetector_R = new G4PVPlacement((rotY_90_3)*(transZ_H), logicDetector_R, "Detector_R", logicWorld, false, 0, true);
	
	G4double StartPoint_X = -nCS_H*(CsI_sidelength_half+wrapping_thickness)+CsI_sidelength_half+wrapping_thickness;		//-Half_length+Half_d
	G4double StartPoint_Y = -nCS*(CsI_sidelength_half+wrapping_thickness)+CsI_sidelength_half+wrapping_thickness;			//-Half_length+Half_d
	for (G4int i = 0; i < nCS_H; i++)	//Column
	{
		for (G4int j = 0; j < nCS; j++)	//Row
		{
			G4Translate3D transZ_CsI(G4ThreeVector(StartPoint_X+i*d, StartPoint_Y+j*d, 0.*m));
			G4VPhysicalVolume* physDetector_f = new G4PVPlacement(transZ_CsI, logicCsI, "Detector_f", logicDetector_F, false, i*10+j, true);
			G4VPhysicalVolume* physDetector_l = new G4PVPlacement(transZ_CsI, logicCsI, "Detector_l", logicDetector_L, false, i*10+j, true);
			G4VPhysicalVolume* physDetector_b = new G4PVPlacement(transZ_CsI, logicCsI, "Detector_b", logicDetector_B, false, i*10+j, true);
			G4VPhysicalVolume* physDetector_r = new G4PVPlacement(transZ_CsI, logicCsI, "Detector_r", logicDetector_R, false, i*10+j, true);
		}
	}

	//Detector_V, V means Vertical
	G4Box* solidDetector_V = new G4Box("solidDetector_V", (CsI_sidelength_half+wrapping_thickness)*nCS_V,(CsI_sidelength_half+wrapping_thickness)*nCS_V, CsI_height_half);
	G4LogicalVolume* logicDetector_U = new G4LogicalVolume(solidDetector_V, Mylar, "logicDetector_U");
	G4LogicalVolume* logicDetector_D = new G4LogicalVolume(solidDetector_V, Mylar, "logicDetector_D");
	//Transformation
	G4Translate3D transZ_V(G4ThreeVector(0*m, 0*m, Cube_side_height));
	G4Rotate3D rotX_90(90.*deg, G4ThreeVector(1.,0.,0.));
	G4Rotate3D rotX_270(-90.*deg, G4ThreeVector(1.,0.,0.));
	//physDetector_H, U = Up, D = Down
	physDetector_U = new G4PVPlacement((rotX_270)*(transZ_V), logicDetector_U, "Detector_U", logicWorld, false, 0, true);
	physDetector_D = new G4PVPlacement((rotX_90)*(transZ_V), logicDetector_D, "Detector_D", logicWorld, false, 0, true);
	
	G4double StartPoint_V = -nCS_V*(CsI_sidelength_half+wrapping_thickness)+CsI_sidelength_half+wrapping_thickness;		//-Half_length+Half_d
	for (G4int i = 0; i < nCS_V; i++)
	{
		for (G4int j = 0; j < nCS_V; j++)
		{
			G4Translate3D transZ_CsI(G4ThreeVector(StartPoint_V+i*d, StartPoint_V+j*d, 0*m));
			G4VPhysicalVolume* physDetector_u = new G4PVPlacement(transZ_CsI, logicCsI, "Detector_u", logicDetector_U, false, i*10+j, true);
			G4VPhysicalVolume* physDetector_d = new G4PVPlacement(transZ_CsI, logicCsI, "Detector_d", logicDetector_D, false, i*10+j, true);
		}
	}
}
void MyDetectorConstruction::ConstructCsI_2() {
	G4double CsI_sidelength_half = 0.05/2*m;
	G4double CsI_height_half = 0.5/2*m;
	G4Box* solidCsI = new G4Box("solidCsI", CsI_sidelength_half, CsI_sidelength_half,  CsI_height_half);
	logicCsI = new G4LogicalVolume(solidCsI, CsI, "logicCsI");

	G4double Mylar_sidelength_half = CsI_sidelength_half + wrapping_thickness;
	//G4double Mylar_sidelength_half = CsI_sidelength_half + 1*cm;
	G4Box* solidMylar = new G4Box("solidMylar", Mylar_sidelength_half, Mylar_sidelength_half,  CsI_height_half);
	logicMylar = new G4LogicalVolume(solidMylar, Mylar, "logicMylar");
	//fScoringVolume = logicCsI;

	// Define parameters
	G4double delta_theta = 360.*deg/number_of_side;
	G4double shift_X_Layer1 = Mylar_sidelength_half/(cos(delta_phi)*tan(delta_theta/2)) + Mylar_sidelength_half*sqrt(2)*sin(45*deg-delta_phi);
	G4double shift_Y_Layer1 = Mylar_sidelength_half*sin(delta_phi)*(1-tan(delta_phi));
	G4double shift_X_Layer2 = 2*Mylar_sidelength_half;
	G4double shift_X_Layer3 = Mylar_sidelength_half/sin(delta_theta/2) + Mylar_sidelength_half/tan(delta_theta/2) + Mylar_sidelength_half;
	//Transformation
	G4Rotate3D rotZ_phi(delta_phi, G4ThreeVector(0., 0., 1.));
	G4Rotate3D rotZ_theta(delta_theta, G4ThreeVector(0., 0., 1.));
	G4Rotate3D rotZ_Layer3(delta_theta/2, G4ThreeVector(0., 0., 1.));
	G4Translate3D transX_Layer1(G4ThreeVector(shift_X_Layer1, shift_Y_Layer1, 0*m));
	G4Translate3D transX_Layer2(G4ThreeVector(shift_X_Layer2, 0*m, 0*m));
	G4Translate3D transX_Layer3(G4ThreeVector(shift_X_Layer3, 0*m, 0*m));
	G4Transform3D T_Layer1 = transX_Layer1*rotZ_phi;
	G4Transform3D T_Layer2 = T_Layer1*transX_Layer2;
	G4Transform3D T_Layer3 = rotZ_Layer3*transX_Layer3;

	// Curved surface of the Cylinder
	for (G4int i = 0; i < number_of_side; i++) {
		physMylar = new G4PVPlacement(T_Layer1, logicMylar, "physMylar_1", logicWorld, false, i, true);
		physMylar = new G4PVPlacement(T_Layer2, logicMylar, "physMylar_2", logicWorld, false, i, true);
		physMylar = new G4PVPlacement(T_Layer3, logicMylar, "physMylar_3", logicWorld, false, i, true);
		T_Layer1 = rotZ_theta*T_Layer1;
		T_Layer2 = rotZ_theta*T_Layer2;
		T_Layer3 = rotZ_theta*T_Layer3;
	}

	// Define parameters
	//G4int number_at_ends = round((shift_X_Layer3 + Mylar_sidelength_half) / Mylar_sidelength_half + 0.5);		// Minimized
	G4int number_at_ends = round((shift_X_Layer3 + Mylar_sidelength_half) / Mylar_sidelength_half + 0.5)+1;
	G4double shift_Z_Layer1 = CsI_height_half + 1*cm + Mylar_sidelength_half;
	G4double shift_Z_Layer2 = 2*Mylar_sidelength_half;
	G4double shift_distance = Mylar_sidelength_half*(number_at_ends-1.);
	//Transformation
	G4Rotate3D rotX_90(90*deg, G4ThreeVector(1., 0., 0.));
	G4Rotate3D rotY_90(90*deg, G4ThreeVector(0., 1., 0.));
	G4Translate3D transZ_plus_Layer1(G4ThreeVector(0*m, 0*m, shift_Z_Layer1));
	G4Translate3D transZ_plus_Layer2(G4ThreeVector(0*m, 0*m, shift_Z_Layer2));
	G4Translate3D transZ_minus_Layer1(G4ThreeVector(0*m, 0*m, -shift_Z_Layer1));
	G4Translate3D transZ_minus_Layer2(G4ThreeVector(0*m, 0*m, -shift_Z_Layer2));
	G4Transform3D T_plus_Layer1 = transZ_plus_Layer1;
	G4Transform3D T_plus_Layer2 = transZ_plus_Layer2*transZ_plus_Layer1;
	G4Transform3D T_plus_Layer3 = transZ_plus_Layer2*transZ_plus_Layer2*transZ_plus_Layer1;
	G4Transform3D T_minus_Layer1 = transZ_minus_Layer1;
	G4Transform3D T_minus_Layer2 = transZ_minus_Layer2*transZ_minus_Layer1;
	G4Transform3D T_minus_Layer3 = transZ_minus_Layer2*transZ_minus_Layer2*transZ_minus_Layer1;

	// Two ends of the Cylinder, 0 is to not construct; 1 is cross type; 2 is offset type; 3 is offset_plus type; 4 is cross type 3 layer,
	//end_selector = 3;
	if (end_selector == 0) {
		G4cout << "Did not construct CsI at two ends." << G4endl;
	}
	else if (end_selector == 1) {
		for (G4int i = 0; i < number_at_ends; i++) {
			G4Translate3D transX(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*i, 0*m, 0*m));
			physMylar = new G4PVPlacement(transX*T_plus_Layer1*rotX_90, logicMylar, "physMylar_F_1", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(transX*T_minus_Layer1*rotX_90, logicMylar, "physMylar_B_1", logicWorld, false, i, true);
			G4Translate3D transY(G4ThreeVector(0*m, -shift_distance + 2*Mylar_sidelength_half*i, 0*m));
			physMylar = new G4PVPlacement(transY*T_plus_Layer2*rotY_90, logicMylar, "physMylar_F_2", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(transY*T_minus_Layer2*rotY_90, logicMylar, "physMylar_B_2", logicWorld, false, i, true);
		}
	}
	else if (end_selector == 2){
		for (G4int i = 0; i < number_at_ends; i++) {
			G4Translate3D transX(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*i, 0*m, 0*m));
			physMylar = new G4PVPlacement(transX*T_plus_Layer1*rotX_90, logicMylar, "physMylar_F_1", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(transX*T_minus_Layer1*rotX_90, logicMylar, "physMylar_B_1", logicWorld, false, i, true);
			if (i > 0) {
				G4Translate3D transX_(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*(i-.5), 0*m, 0*m));
				physMylar = new G4PVPlacement(transX_*T_plus_Layer2*rotX_90, logicMylar, "physMylar_F_2", logicWorld, false, i, true);
				physMylar = new G4PVPlacement(transX_*T_minus_Layer2*rotX_90, logicMylar, "physMylar_B_2", logicWorld, false, i, true);
			}
		}
	}
	else if (end_selector == 3){
		for (G4int i = 0; i <= number_at_ends; i++) {
			if (i < number_at_ends) {
				G4Translate3D transX(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*i, 0*m, 0*m));
				physMylar = new G4PVPlacement(transX*T_plus_Layer1*rotX_90, logicMylar, "physMylar_F_1", logicWorld, false, i, true);
				physMylar = new G4PVPlacement(transX*T_minus_Layer1*rotX_90, logicMylar, "physMylar_B_1", logicWorld, false, i, true);
			}
			G4Translate3D transX_(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*(i-.5), 0*m, 0*m));
			physMylar = new G4PVPlacement(transX_*T_plus_Layer2*rotX_90, logicMylar, "physMylar_F_2", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(transX_*T_minus_Layer2*rotX_90, logicMylar, "physMylar_B_2", logicWorld, false, i, true);
		}
	}
	else if (end_selector == 4) {
		for (G4int i = 0; i < number_at_ends; i++) {
			G4Translate3D transX(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*i, 0*m, 0*m));
			physMylar = new G4PVPlacement(transX*T_plus_Layer1*rotX_90, logicMylar, "physMylar_F_1", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(transX*T_minus_Layer1*rotX_90, logicMylar, "physMylar_B_1", logicWorld, false, i, true);

			G4Translate3D transY(G4ThreeVector(0*m, -shift_distance + 2*Mylar_sidelength_half*i, 0*m));
			physMylar = new G4PVPlacement(transY*T_plus_Layer2*rotY_90, logicMylar, "physMylar_F_2", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(transY*T_minus_Layer2*rotY_90, logicMylar, "physMylar_B_2", logicWorld, false, i, true);

			physMylar = new G4PVPlacement(transX*T_plus_Layer3*rotX_90, logicMylar, "physMylar_F_2", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(transX*T_minus_Layer3*rotX_90, logicMylar, "physMylar_B_2", logicWorld, false, i, true);
		}
	}
	physCsI = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicCsI, "physCsI", logicMylar, false, 0, true);
}
void MyDetectorConstruction::ConstructCsI_3() {
	G4double CsI_sidelength_half = 0.05/2*m;
	G4double CsI_height_half = 0.5/2*m;
	G4double Pb_thickness_half = 0.5/2*mm;
	//G4double Pb_thickness_half = 1./2*cm;
	G4double Cable_gap = 1*cm;

	G4Box* solidCsI = new G4Box("solidCsI", CsI_sidelength_half, CsI_sidelength_half,  CsI_height_half);
	logicCsI = new G4LogicalVolume(solidCsI, CsI, "logicCsI");

	G4double Mylar_sidelength_half = CsI_sidelength_half + wrapping_thickness;
	//G4double Mylar_sidelength_half = CsI_sidelength_half + 1*cm;
	G4Box* solidMylar = new G4Box("solidMylar", Mylar_sidelength_half, Mylar_sidelength_half,  CsI_height_half);
	logicMylar = new G4LogicalVolume(solidMylar, Mylar, "logicMylar");
	//fScoringVolume = logicCsI;

	G4Box* solidPb = new G4Box("solidPb",  Pb_thickness_half, CsI_height_half, CsI_height_half);
	logicPb = new G4LogicalVolume(solidPb, Pb, "solidPb");

	// Define parameters
	G4int number_of_side = 4;
	G4int number_at_ends = 10;
	G4double delta_theta = 360.*deg/number_of_side;
	G4double shift_X_Layer1 = 10*cm;
	//G4double shift_X_Layer1 = 10*cm + 7.5*cm;
	char insert_place[5] = "";
	G4bool is_1 = (strchr(insert_place, '1') != NULL);
	G4bool is_2 = (strchr(insert_place, '2') != NULL);
	G4bool is_3 = (strchr(insert_place, '3') != NULL);
	G4bool is_4 = (strchr(insert_place, '4') != NULL);
	G4double shift_Y_Layer1 = 10*Mylar_sidelength_half - (shift_X_Layer1 - Cable_gap);
	G4double shift_X_Layer2 = 2*Mylar_sidelength_half;
	G4double shift_distance = Mylar_sidelength_half*(number_at_ends-1.);

	// Transformation
	G4Rotate3D rotX_90(90.*deg, G4ThreeVector(1., 0., 0.));
	G4Rotate3D rotY_90(90.*deg, G4ThreeVector(0., 1., 0.));
	G4Translate3D transX_shift(G4ThreeVector(shift_X_Layer1, 0*m, 0*m));
	G4Translate3D transX_Pb_half(G4ThreeVector(Pb_thickness_half, 0*m, 0*m));
	G4Translate3D transX_Mylar_half(G4ThreeVector(Mylar_sidelength_half, 0*m, 0*m));
	G4Translate3D transY(G4ThreeVector(0*m, shift_Y_Layer1, 0*m));

	G4Transform3D T_Layer1, T_Layer2, T_Layer3, T_Pb1, T_Pb2, T_Pb3, T_Pb4;
	T_Layer1 = transY*transX_shift*rotX_90;
	if (is_1) {
		T_Pb1 = transX_Pb_half*T_Layer1;
		T_Layer1 = transX_Pb_half*T_Pb1;
	}
	T_Layer1 = transX_Mylar_half*T_Layer1;
	T_Layer2 = transX_Mylar_half*T_Layer1;
	if (is_2) {
		T_Pb2 = transX_Pb_half*T_Layer2;
		T_Layer2 = transX_Pb_half*T_Pb2;
	}
	T_Layer2 = transX_Mylar_half*T_Layer2;
	T_Layer3 = transX_Mylar_half*T_Layer2;
	if (is_3) {
		T_Pb3 = transX_Pb_half*T_Layer3;
		T_Layer3 = transX_Pb_half*T_Pb3;
	}
	T_Layer3 = transX_Mylar_half*T_Layer3;
	T_Pb4 = transX_Mylar_half*T_Layer3;
	if (is_4) {
		T_Pb4 = transX_Pb_half*T_Pb4;
	}
	// Transformation

	// Curved 4-surfaces of the Cuboid
	for (G4int i = 0; i < number_of_side; i++) {
		G4Rotate3D rotZ_theta(delta_theta*i, G4ThreeVector(0., 0., 1.));
		if (is_1)
			physPb = new G4PVPlacement(rotZ_theta*T_Pb1, logicPb, "logicPb_1", logicWorld, false, i, true);
		if (is_2)
			physPb = new G4PVPlacement(rotZ_theta*T_Pb2, logicPb, "logicPb_2", logicWorld, false, i, true);
		if (is_3)
			physPb = new G4PVPlacement(rotZ_theta*T_Pb3, logicPb, "logicPb_3", logicWorld, false, i, true);
		if (is_4)
			physPb = new G4PVPlacement(rotZ_theta*T_Pb4, logicPb, "logicPb_4", logicWorld, false, i, true);
		for (G4int j = 0; j < number_at_ends; j++) {
			G4Translate3D stacking(G4ThreeVector(0*m, -shift_distance+2*Mylar_sidelength_half*j, 0*m));
			physMylar = new G4PVPlacement(rotZ_theta*T_Layer1*stacking, logicMylar, "physMylar_1", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(rotZ_theta*T_Layer3*stacking, logicMylar, "physMylar_3", logicWorld, false, i, true);
			if (j > 0) {
				G4Translate3D stacking(G4ThreeVector(0*m, -shift_distance+2*Mylar_sidelength_half*(j-.5), 0*m));
				physMylar = new G4PVPlacement(rotZ_theta*T_Layer2*stacking, logicMylar, "physMylar_2", logicWorld, false, i, true);
			}
		}
	}

	// Define parameters
	G4double shift_Z_Layer1 = CsI_height_half + Cable_gap;
	shift_distance = Mylar_sidelength_half*(number_at_ends-1.);

	//Transformation
	G4Translate3D transZ_shift(G4ThreeVector(0*m, 0*m, shift_Z_Layer1));
	G4Translate3D transZ_Pb_half(G4ThreeVector(0*m, 0*m, Pb_thickness_half));
	G4Translate3D transZ_Mylar_half(G4ThreeVector(0*m, 0*m, Mylar_sidelength_half));
	T_Layer1 = transZ_shift*rotY_90*rotX_90;
	if (is_1) {
		T_Pb1 = transZ_Pb_half*T_Layer1;
		T_Layer1 = transZ_Pb_half*T_Pb1;
	}
	T_Layer1 = transZ_Mylar_half*T_Layer1;
	T_Layer2 = transZ_Mylar_half*T_Layer1;
	if (is_2) {
		T_Pb2 = transZ_Pb_half*T_Layer2;
		T_Layer2 = transZ_Pb_half*T_Pb2;
	}
	T_Layer2 = transZ_Mylar_half*T_Layer2;
	T_Layer3 = transZ_Mylar_half*T_Layer2;
	if (is_3) {
		T_Pb3 = transZ_Pb_half*T_Layer3;
		T_Layer3 = transZ_Pb_half*T_Pb3;
	}
	T_Layer3 = transZ_Mylar_half*T_Layer3;
	T_Pb4 = transZ_Mylar_half*T_Layer3;
	if (is_4) {
		T_Pb4 = transZ_Pb_half*T_Pb4;
	}

	// 2 ends of the Cuboid
	for (G4int i = 0; i < 2; i++) {
		G4Rotate3D rotY(180.*i*deg, G4ThreeVector(0., 1., 0.));
		if (is_1)
			physPb = new G4PVPlacement(rotY*T_Pb1, logicPb, "logicPb_1", logicWorld, false, i, true);
		if (is_2)
			physPb = new G4PVPlacement(rotY*T_Pb2, logicPb, "logicPb_2", logicWorld, false, i, true);
		if (is_3)
			physPb = new G4PVPlacement(rotY*T_Pb3, logicPb, "logicPb_3", logicWorld, false, i, true);
		if (is_4)
			physPb = new G4PVPlacement(rotY*T_Pb4, logicPb, "logicPb_4", logicWorld, false, i, true);
		for (G4int j = 0; j <= number_at_ends; j++) {
			if (j < number_at_ends) {
				G4Translate3D stacking(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*j, 0*m, 0*m));
				physMylar = new G4PVPlacement(rotY*stacking*T_Layer1, logicMylar, "physMylar_End_1", logicWorld, false, i, true);
				physMylar = new G4PVPlacement(rotY*stacking*T_Layer3, logicMylar, "physMylar_End_3", logicWorld, false, i, true);
			}
			G4Translate3D stacking(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*(j-.5), 0*m, 0*m));
			physMylar = new G4PVPlacement(rotY*stacking*T_Layer2, logicMylar, "physMylar_End_2", logicWorld, false, i, true);
		}
	}
	physCsI = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicCsI, "physCsI", logicMylar, false, 0, true);
}
void MyDetectorConstruction::ConstructCsI_4() {
	G4double CsI_sidelength_half = 0.05/2*m;
	G4double CsI_height_half = 0.5/2*m;
	G4double Cable_gap = 1*cm;

	G4Box* solidCsI = new G4Box("solidCsI", CsI_sidelength_half, CsI_sidelength_half,  CsI_height_half);
	logicCsI = new G4LogicalVolume(solidCsI, CsI, "logicCsI");

	G4double Mylar_sidelength_half = CsI_sidelength_half + wrapping_thickness;
	//G4double Mylar_sidelength_half = CsI_sidelength_half + 1*cm;
	G4Box* solidMylar = new G4Box("solidMylar", Mylar_sidelength_half, Mylar_sidelength_half,  CsI_height_half);
	logicMylar = new G4LogicalVolume(solidMylar, Mylar, "logicMylar");
	//fScoringVolume = logicCsI;

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
			physMylar = new G4PVPlacement(rotZ_theta*transY*stacking*T_Layer1, logicMylar, "physMylar_1", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(rotZ_theta*transY*rotX_90*stacking*T_Layer2, logicMylar, "physMylar_2", logicWorld, false, i, true);
			physMylar = new G4PVPlacement(rotZ_theta*transY*stacking*T_Layer3, logicMylar, "physMylar_3", logicWorld, false, i, true);
			//T_Layer3 = transY_1*T_Layer3;
		}
	}

	// Define parameters
	//G4int number_at_ends = round((shift_X_Layer3 + Mylar_sidelength_half) / Mylar_sidelength_half + 0.5);		// Minimized
	//G4int number_at_ends = round((2*shift_X_Layer2+shift_X_Layer1 + Mylar_sidelength_half) / Mylar_sidelength_half + 0.5)+1;
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
			G4Translate3D stacking(G4ThreeVector(-shift_distance + 2*Mylar_sidelength_half*j, 0*m, 0*m));
			physMylar = new G4PVPlacement(rotY*stacking*T_Layer1*rotX_90, logicMylar, "physMylar_End_1", logicWorld, false, j, true);
			physMylar = new G4PVPlacement(rotY*rotZ_90*stacking*T_Layer2*rotX_90, logicMylar, "physMylar_End_2", logicWorld, false, j, true);
			physMylar = new G4PVPlacement(rotY*stacking*T_Layer3*rotX_90, logicMylar, "physMylar_End_3", logicWorld, false, j, true);
		}
	}
	physCsI = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicCsI, "physCsI", logicMylar, false, 0, true);
}
// End CsI Detector