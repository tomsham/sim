#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	// Define required materials
	DefineMaterials();

	// The A is a placeholder for the user defined commands (fMessenger)
	G4int A = 0;

	// Here to control the construction selector to select which part should be constructed
	// Radioactive Source (Positron Source)
	isRing = false;
	isDisk = false;
	isBareSource = false;
	// Dt = Detector, only need to construct one of them
	isBareSource_Dt = false;
	isBS_Disk_Dt = false;
	isBSD_Ring_Dt = false;

	// Positronium Source
	isLiquid = false;
	isCupDetector = false;
	isDetector_Shell = false;
	isDetector_Cylinder = false;

	// CsI crystal detector
	isCsI = false;

	// Aerogel, from Eltis
	isSourceScintillator = false;
	// End Aerogel

	// Set the material for each logical volume
	matWorld = Air; //Vacuum;
	matRing = Ti_, matDisk = Ti_, matSource = NaCl;
	matContainer = Ti_, matLiquid = H2O;

	// Set the default of each logical volume to be NULL so the sensitive detector selector can work well
	logicDetector_Shell = NULL;
	logicDetector_Cylinder = NULL;

	logicBareSource_Dt = NULL;
	logicBS_Disk_Dt = NULL;
	logicBSD_Ring_Dt = NULL;

	logicPlaneDetector_W = NULL;
	logicPlaneDetector_C = NULL;
	logicPlaneDetector_L = NULL;
	logicRingDetector = NULL;
	logicTubeDetector = NULL;
	logicCsI = NULL;

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
	
	// These are user defined commands for use in User-Interface(UI) mode and batch mode(using macro file)
	fMessenger = new G4GenericMessenger(this, "/@MyDetector/", "Macros");
	fMessenger->DeclareProperty("control/execute region_setup.mac", A, "Set the active region (cylinder locate at origin, radius = 9.53/2*mm, half height = 0.0001*mm)");
	fMessenger->DeclareProperty("control/execute rebuild.mac",A,"Rebuild Selected Physical Volume inside a 1.5*1.5*1.5 m^3 Cubic World contains Air, its center is the origin");
	fMessenger->DeclareProperty("Source/isBareSource", isBareSource, "Construct the BareSouce (cylinder locate at origin, radius = 9.53/2*mm, half height = 0.0001*mm)");
	fMessenger->DeclareProperty("Source/isDisk", isDisk, "Construct the Disk (cylinder locate at origin, radius = 9.53/2*mm, half height = 0.00508*mm)");
	fMessenger->DeclareProperty("Source/isRing", isRing, "Construct the Ring (tube locate at origin, inner radius = 9.53/2*mm, outer radius = 19.1/2*mm, half height = 0.254*mm)");
	fMessenger->DeclareProperty("Container/isLiquid", isLiquid, "Construct the Ti-Container and Water (2 cylinders locate in front and back of the Source, radius = 19.1/2*mm, half height = 1*cm)");
	fMessenger->DeclareProperty("Container/isCupDetector", isCupDetector, "Select the detectors inside the Ti-Container and Water (including Plane Detector and Tube Detector, the Plane Detector has spacing = 0.02*mm)");
	fMessenger->DeclareProperty("Container/container_thickness",container_thickness,"Set the thickness of container. Default = 1*mm.");
	fMessenger->DeclareProperty("Container/d_pos_z",d_pos_z,"Set the spacing between two nearest plane detectors. Default = 0.02*mm, Default Amount of Plane Detetors = (int)(2*mm/d_pos_z) = 100");
	fMessenger->DeclareProperty("isShell_Detector", isDetector_Shell, "Construct Shell Detector (spherical shell locate at origin, inner radius = 3*cm, thickness = 1*nm)");
	//fMessenger->DeclareProperty("isCsI", isCsI, "Select CsI detector");
}
MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
	G4NistManager* nist = G4NistManager::Instance();

	Air = nist->FindOrBuildMaterial("G4_AIR");
	Vacuum = nist->FindOrBuildMaterial("G4_Galactic");

	// Aerogel
	SiO2 = new G4Material("SiO2", 0.125 * g / cm3, 2);
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

	H2O = new G4Material("H2O", 1.000 * g / cm3, 2);
	H2O->AddElement(nist->FindOrBuildElement("H"), 2);
	H2O->AddElement(nist->FindOrBuildElement("O"), 1);

	C = nist->FindOrBuildElement("C");

	Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
	Aerogel->AddMaterial(SiO2, 62.5*perCent);
	Aerogel->AddMaterial(H2O, 37.4*perCent);
	Aerogel->AddElement(C, 0.1*perCent);

	G4double energyAerogel[2] = { 1.239841939*eV/0.9, 1.239841939*eV/0.2 };
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
	G4double energyCsI[1] = { 1.239841939*eV/0.31 };	//310nm
	G4double rindexCsI[1] = { 1.95 };
	G4MaterialPropertiesTable* mptCsI = new G4MaterialPropertiesTable();
	mptCsI->AddProperty("RINDEX", energyCsI, rindexCsI, 1);

	/*
	//Scintillation Test (test data for CsI)
	G4double scintCsI[1] = {0.1};
	G4double fractionCsI[1] = { 1.0 };						//Intensity fraction of different wavelengths which are the component of scintillation intensity eg 30./keV
	mptCsI->AddConstProperty("RESOLUTIONSCALE", 1.);				// AddConstProperty for a number
	//mptCsI->AddConstProperty("SCINTILLATIONYIELD", 30./keV);
	mptCsI->AddProperty("SCINTILLATIONCOMPONENT1", energyCsI, fractionCsI, 1);	// AddProperty for a array
	mptCsI->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1500.0*ns);	// Time constant is the decay time	
	mptCsI->AddConstProperty("SCINTILLATIONYIELD1", 30./keV);
	//End Scintillation Test
	*/

	CsI->SetMaterialPropertiesTable(mptCsI);
	// End CsI

	// Mylar
	Mylar = nist->FindOrBuildMaterial("G4_MYLAR");
	// End Mylar
	
	// From Eltis
	Kapton = nist->FindOrBuildMaterial("G4_KAPTON");

	Ti = nist->FindOrBuildElement("Ti");
	Ti_ = new G4Material("Ti_", 4.507*g/cm3, 1);	//The density of G4_Ti is 4.54*g/cm3
	Ti_->AddElement(Ti,1.);
	// End Eltis

}

// Construct All physical volumes
G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	G4double xWorld = 0.75*m;
	G4double yWorld = 0.75*m;
	G4double zWorld = 0.75*m;

	// A cubic world with volume 1.5 m*1.5 m*1.5 m
	solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
	logicWorld = new G4LogicalVolume(solidWorld, matWorld, "logicWorld");
	//logicWorld = new G4LogicalVolume(solidWorld, matVacuum, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

	if (isRing)
		ConstructRing();
	if (isDisk)
		ConstructDisk();
	if (isBareSource)
		ConstructBareSource(isDisk);
	if (isBareSource_Dt)
		ConstructBareSource_Detector();
	if (isBS_Disk_Dt)
		ConstructBS_Disk_Detector();
	if (isBSD_Ring_Dt)
		ConstructBSD_Ring_Detector();

	if (isLiquid)
		ConstructLquidScintillator();
	if (isCupDetector)
		ConstructCup_Detector();

	if (isDetector_Shell)
		ConstructShell_Detector();
	if (isDetector_Cylinder)
		ConstructCylinder_Detector();

	// From Eltis, edited by Tom
	if (isSourceScintillator)
		ConstructSourceScintillator();
	// End Eltis

	return physWorld;
}

// Set Sensitive Detector(SD) and Field
void MyDetectorConstruction::ConstructSDandField()
{
	MySensentiveDetector* sensDet = new MySensentiveDetector("SensitiveDetector");
	if(logicDetector_Shell != NULL)
		logicDetector_Shell->SetSensitiveDetector(sensDet);
	if(logicDetector_Cylinder != NULL)
		logicDetector_Cylinder->SetSensitiveDetector(sensDet);

	if(logicBareSource_Dt != NULL)
		logicBareSource_Dt->SetSensitiveDetector(sensDet);
	if(logicBS_Disk_Dt != NULL)
		logicBS_Disk_Dt->SetSensitiveDetector(sensDet);
	if(logicBSD_Ring_Dt != NULL)
		logicBSD_Ring_Dt->SetSensitiveDetector(sensDet);

	// Ideal Cup_Detector
	if(logicPlaneDetector_W != NULL)
		logicPlaneDetector_W->SetSensitiveDetector(sensDet);
	if(logicPlaneDetector_C != NULL)
		logicPlaneDetector_C->SetSensitiveDetector(sensDet);
	if(logicPlaneDetector_L != NULL)
		logicPlaneDetector_L->SetSensitiveDetector(sensDet);
	if(logicRingDetector != NULL)
		logicRingDetector->SetSensitiveDetector(sensDet);
	if(logicTubeDetector != NULL)
		logicTubeDetector->SetSensitiveDetector(sensDet);
	// End Ideal Cup_Detector

	/*if(logicCsI != NULL)
	logicCsI->SetSensitiveDetector(sensDet);*/
}

// Positron Source
void MyDetectorConstruction::ConstructRing() {
	solidRing = new G4Tubs("solidRing", disk_radius, ring_radius, ring_height_half, 0.*deg, 360.*deg);
	logicRing = new G4LogicalVolume(solidRing, matRing, "logicRing");
	physRing = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicRing, "Ring", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructDisk() {
	solidDisk = new G4Tubs("solidDisk", 0.*nm, disk_radius, disk_height_half, 0.*deg, 360.*deg);
	logicDisk = new G4LogicalVolume(solidDisk, matDisk, "logicDisk");
	physDisk = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicDisk, "Disk", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructBareSource(G4bool matSelect)
{
	G4LogicalVolume *logicMother;
	logicMother = logicWorld;
	if (matSelect)
		logicMother = logicDisk;
	solidBareSource = new G4Tubs("solidBareSource", 0.*nm, bare_source_radius, bare_source_height_half, 0.*deg, 360.*deg);
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
void MyDetectorConstruction::ConstructLquidScintillator(){
	solidContainer = new G4Tubs("LiquidContainer", 0.*nm, container_radius, container_height_half, 0.*deg, 360.*deg);
	solidLiquid = new G4Tubs("Liquid", 0.*nm, container_radius-container_thickness, container_height_half-container_thickness, 0.*deg, 360.*deg);

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
	solidTubeDetector = new G4Tubs("solidTubeDetector", container_radius, container_radius+1.*nm, ring_height_half+2*container_height_half, 0.*deg, 360.*deg);
	logicTubeDetector = new G4LogicalVolume(solidTubeDetector, matWorld, "logicTubeDetector");
	physTubeDetector = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0*m), logicTubeDetector, "TubeDetector", logicWorld, false, 0, true);
	// End TubeDetector

	// PlaneDetector
	G4double plane_radius = container_radius-container_thickness;
	G4double plane_height_half = 1*nm/2;

	solidPlaneDetector = new G4Tubs("solidPlaneDetector", 0.*nm, plane_radius, plane_height_half, 0.*deg, 360.*deg);
	logicPlaneDetector_W = new G4LogicalVolume(solidPlaneDetector, matWorld, "logicPlaneDetector_W");
	logicPlaneDetector_C = new G4LogicalVolume(solidPlaneDetector, matContainer, "logicPlaneDetector_C");
	logicPlaneDetector_L = new G4LogicalVolume(solidPlaneDetector, matLiquid, "logicPlaneDetector_L");
	solidRingDetector = new G4Tubs("solidRingDetector", plane_radius+1.*nm, container_radius, plane_height_half, 0.*deg, 360.*deg);
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
// End Liquid scintillator and CupDetector

// Ideal Cylinder Detector
void MyDetectorConstruction::ConstructCylinder_Detector() {
	//G4double cylinder_thickness = 1*um;
	G4double cylinder_thickness = 1.*nm;

	G4Tubs *solidInside = new G4Tubs("solidInside", 0.*nm, ring_radius+cylinder_thickness/2, ring_height_half+cylinder_thickness/2, 0.*deg, 360.*deg);
	G4Tubs *solidOutside = new G4Tubs("solidOutside", 0.*nm, ring_radius+cylinder_thickness, ring_height_half+cylinder_thickness, 0.*deg, 360.*deg);
	G4SubtractionSolid *solidDetector_Cylinder = new G4SubtractionSolid("solidDetector_Cylinder", solidOutside, solidInside);
	logicDetector_Cylinder = new G4LogicalVolume(solidDetector_Cylinder, matWorld, "logicDetector_Cylinder");
	physDetector_Cylinder = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicDetector_Cylinder, "Detector_Cylinder", logicWorld, false, 0, true);
}
// End Ideal Cylinder Detector

// Ideal Detector
void MyDetectorConstruction::ConstructShell_Detector() {
	//Large sphere
	G4double inner_radius = 3*cm;
	//G4double inner_radius = 70*cm;
	G4double outer_radius = inner_radius + 1*nm;
	solidDetector_Shell = new G4Sphere("solidDetector_Shell", inner_radius, outer_radius, 0.*deg, 360.*deg, 0.*deg, 360.*deg);
	logicDetector_Shell = new G4LogicalVolume(solidDetector_Shell, matWorld, "logicDetector_Shell");
	physDetector_Shell = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicDetector_Shell, "Detector_Shell", logicWorld, false, 0, true);
}
// End Ideal Detector

// Design-1 CsI Detector
void MyDetectorConstruction::ConstructCsI()
{
	G4double CsI_sidelength_half = 0.05/2*m;
	G4double CsI_height_half = 0.5/2*m;
	solidCsI = new G4Box("solidCsI", CsI_sidelength_half, CsI_sidelength_half,  CsI_height_half);
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
	//Detector_H 
	solidDetector_H = new G4Box("solidDetector_H", (CsI_sidelength_half+wrapping_thickness)*nCS_H,(CsI_sidelength_half+wrapping_thickness)*nCS, CsI_height_half);
	logicDetector_F = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_F");
	logicDetector_L = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_L");
	logicDetector_B = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_B");
	logicDetector_R = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_R");
	//Transformation
	G4Translate3D transZ_H(G4ThreeVector(-(nCS*CsI_sidelength_half+(nCS-0.5)*wrapping_thickness)+(CsI_sidelength_half+wrapping_thickness)*nCS_H, 0*m, Cube_side_length));
	G4Rotate3D rotY_90(90.*deg, G4ThreeVector(0.,1.,0.));
	G4Rotate3D rotY_90_2(90.*2*deg, G4ThreeVector(0.,1.,0.));
	G4Rotate3D rotY_90_3(90.*3*deg, G4ThreeVector(0.,1.,0.));
	//physDetector_H
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

	//Detector_V
	solidDetector_V = new G4Box("solidDetector_V", (CsI_sidelength_half+wrapping_thickness)*nCS_V,(CsI_sidelength_half+wrapping_thickness)*nCS_V, CsI_height_half);
	logicDetector_U = new G4LogicalVolume(solidDetector_V, Mylar, "logicDetector_U");
	logicDetector_D = new G4LogicalVolume(solidDetector_V, Mylar, "logicDetector_D");
	//Transformation
	G4Translate3D transZ_V(G4ThreeVector(0*m, 0*m, Cube_side_height));
	G4Rotate3D rotX_90(90.*deg, G4ThreeVector(1.,0.,0.));
	G4Rotate3D rotX_270(-90.*deg, G4ThreeVector(1.,0.,0.));
	//physDetector_H
	G4VPhysicalVolume* physDetector_U = new G4PVPlacement((rotX_270)*(transZ_V), logicDetector_U, "Detector_U", logicWorld, false, 0, true);
	G4VPhysicalVolume* physDetector_D = new G4PVPlacement((rotX_90)*(transZ_V), logicDetector_D, "Detector_D", logicWorld, false, 0, true);
	
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
// End Design-1

// Construct Aerogel, from Eltis
void MyDetectorConstruction::ConstructSourceScintillator() {
	//G4double Scintillator_radius = ring_radius+1./2*mm;
	G4double Scintillator_radius = 6./2*cm;
	//G4double Scintillator_height = ring_height_half+4.0/2*mm;
	G4double Scintillator_height = 3./2*cm;

	G4Tubs *solidA = new G4Tubs("A", disk_radius, ring_radius, ring_height_half, 0.*deg, 360.*deg);				//Same as solidRing
	G4Tubs *solidB = new G4Tubs("B", 0.*nm, disk_radius, disk_height_half, 0.*deg, 360.*deg);					//Same as solidDisk
	G4UnionSolid *solidInside = new G4UnionSolid("solidInside", solidA, solidB);

	G4Tubs* solidOutside = new G4Tubs("solidOutside", 0.*nm, Scintillator_radius, Scintillator_height, 0.*deg, 360.*deg);
	G4SubtractionSolid* SourceScintillator_No_Disk_Ring = new G4SubtractionSolid("SourceScintillator_No_Disk_Ring", solidOutside, solidInside);
	logic_SourceScintillator = new G4LogicalVolume(SourceScintillator_No_Disk_Ring, SiO2, "logic_SourceScintillator");
	phys_SourceScintillator = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logic_SourceScintillator, "phys_SourceScintillator", logicWorld, false, 0, true);	
}
/*
void MyDetectorConstruction::ConstructAerogel() {
G4double size_Scintillator = ring_radius+1./2*mm;
G4double height_of_Scintillator = ring_height_half+4.0/2*mm;
//Scintillator
G4Tubs* BaseSourceScintillator = new G4Tubs("Base", 0. * nm, size_outer_radius, inner_size_of_height + 1.0 / 2 * mm, 0. * deg, 360. * deg);
G4IntersectionSolid* Intersection_Part_Scintillator = new G4IntersectionSolid("Intersection_Part_Scintillator", BaseSourceScintillator, Foil_Geometry);
SourceScintillator_SubtractOuter = new G4SubtractionSolid("SourceScintillatorSubtractOuter", BaseSourceScintillator, Intersection_Part_Scintillator);
//Aerogel
G4Box* BaseSourceAerogel = new G4Box("base", 5. * cm, 5. * cm, 5. * cm);
G4SubtractionSolid* SourceAerogel_SubtractInner = new G4SubtractionSolid("SourceAerogelSubtractInner", BaseSourceAerogel, Foil_Geometry);
AerogelGeometry = new G4SubtractionSolid("AerogelGeometry", SourceAerogel_SubtractInner, SourceScintillator_SubtractOuter);
logic_SourceAerogel = new G4LogicalVolume(AerogelGeometry, SiO2, "logic_SourceAerogel");
phys_SourceAerogel = new G4PVPlacement(0, G4ThreeVector(0. * m, 0. * m, 0. * m), logic_SourceAerogel, "phys_SourceAerogel", logicWorld, false, 0, true);
}
*/
// End Aerogel