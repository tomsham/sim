#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	DefineMaterials();

	G4int A = 0;
	// Change 100x100 before Run Detector
	thick = 13.*um;		//Thickness of wrapping (m), 13.e-6 m = 13. um
	isCsI = false;
	isShell_Detector = true;

	logicDetector = NULL;
	logicDetector_Shell = NULL;

	// From Eltis
	dielectricMaterial = false;
	SourceScintillator = false;

	disk_radius = 9.53/2*mm;
	ring_radius = 19.1/2*mm;
	/*disk_height_half = 0.01/2*mm;
	ring_height_half = 1./2*mm;*/
	disk_height_half = 0.00508*mm;		//The activity is placed between wwo layers of 0.00508*mm Ti foil which is 0.0102*mm in total
	ring_height_half = 0.254*mm;		//Supported by two 0.254 mm Ti disks

	RingDielectric = new G4Tubs("Ring", disk_radius, ring_radius, ring_height_half, 0.*deg, 360.*deg);
	DiskDielectric = new G4Tubs("Disk", 0.*nm, disk_radius, disk_height_half, 0.*deg, 360.*deg);
	SourceDielectric = new G4Tubs("Source", 0.*nm, disk_radius, 0.0001*mm, 0.*deg, 360.*deg);
	Foil_Geometry = new G4UnionSolid("Foil_Geometry", RingDielectric, DiskDielectric);
	// End Eltis

	fMessenger = new G4GenericMessenger(this, "/MyDetector/", "Macros");
	fMessenger->DeclareProperty("control/execute region_setup.mac", A, "Visualize the active region");
	//fMessenger->DeclareProperty("isShell_Detector", isShell_Detector, "Toggle Cubic_Detector_Demo");

}
MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
	G4NistManager* nist = G4NistManager::Instance();

	worldMat = nist->FindOrBuildMaterial("G4_AIR");

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
	G4MaterialPropertiesTable* mptworld = new G4MaterialPropertiesTable();
	mptworld->AddProperty("RINDEX", energyAerogel, rindexWorld, 2);
	worldMat->SetMaterialPropertiesTable(mptworld);

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

 G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	G4double xWorld = 0.75*m;
	G4double yWorld = 0.75*m;
	G4double zWorld = 0.75*m;

	// A cubic world with volume 1.5 m*1.5 m*1.5 m
	solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
	
	if (isCsI)
		ConstructCsI();
	if (isShell_Detector)
		Construct_Ideal_Shell_Detector();
	
	// From Eltis
	if (dielectricMaterial)
		ConstructDielectric();
	if (SourceScintillator)
		ConstructSourceScintillator();
	// End Eltis

	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
	
	MySensentiveDetector* sensDet = new MySensentiveDetector("SensitiveDetector");
	/*if(logicDetector != NULL)
		logicDetector->SetSensitiveDetector(sensDet);*/
	if(logicDetector_Shell != NULL)
		logicDetector_Shell->SetSensitiveDetector(sensDet);
}

// Edited by Tom, Eltis Final
void MyDetectorConstruction::ConstructDielectric() {
	G4Material* foil_material_1 = Ti_; //Kapton Or Ti_ or worldMat
	G4Material* foil_material_2 = Ti_;

	/*logicRing = new G4LogicalVolume(RingDielectric, foil_material_1, "logicRing");
	physRing = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicRing, "Ring", logicWorld, false, 0, true);
	logicDisk = new G4LogicalVolume(DiskDielectric, foil_material_2, "logicDisk");
	physDisk = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicDisk, "Disk", logicWorld, false, 0, true);*/

	logicSource = new G4LogicalVolume(SourceDielectric, NaCl, "logicSource");
	//physSource = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicSource, "Source", logicDisk, false, 0, true);
	physSource = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicSource, "Source", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructSourceScintillator() {
	//G4double Scintillator_radius = ring_radius+1./2*mm;
	G4double Scintillator_radius = 6./2*cm;
	//G4double Scintillator_height = ring_height_half+4.0/2*mm;
	G4double Scintillator_height = 3./2*cm;

	G4Tubs* BaseSourceScintillator = new G4Tubs("base", 0.*nm, Scintillator_radius, Scintillator_height, 0.*deg, 360.*deg);
	G4SubtractionSolid* SourceScintillator_No_Disk_Ring = new G4SubtractionSolid("SourceScintillator_No_Disk_Ring", BaseSourceScintillator, Foil_Geometry);
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
// End

void MyDetectorConstruction::Construct_Ideal_Shell_Detector() {
	//Large sphere
	G4double inner_radius = 1*cm;
	//G4double inner_radius = 70*cm;
	G4double outer_radius = inner_radius + 0.001*mm;
	solidDetector_Shell = new G4Sphere("solidDetector_Shell", inner_radius, outer_radius, 0.*deg, 360.*deg, 0.*deg, 360.*deg);
	logicDetector_Shell = new G4LogicalVolume(solidDetector_Shell, worldMat, "logicDetector_Shell");
	physDetector_Shell = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicDetector_Shell, "Shell", logicWorld, false, 0, true);
}

//Design
void MyDetectorConstruction::ConstructCsI()
{
	G4double CsI_sidelength_half = 0.05/2*m;
	G4double CsI_height_half = 0.5/2*m;
	solidCsI = new G4Box("solidCsI", CsI_sidelength_half, CsI_sidelength_half,  CsI_height_half);
	logicCsI = new G4LogicalVolume(solidCsI, CsI, "logicDetector");
	logicDetector = logicCsI;
	//fScoringVolume = logicDetector;

	G4double d = 2*(CsI_sidelength_half+thick);	//Distance between 2 CsI (m)
	G4double nCS = 4;							//Number of Crystals on the Side of Cube
	G4double nExtra = 1;
	G4double nCS_H = nCS+nExtra;				//Horizontal, Front, Back, Left, Right
	G4double nCS_V = nCS+nExtra*2;				//Vertical, Up, Down
	G4double Cube_side_length = nCS*CsI_sidelength_half+(nCS-0.5)*thick+CsI_height_half;
	G4double Cube_side_height = nCS*(CsI_sidelength_half+thick)+CsI_height_half;

	//Create Mylar Wrapping as the mother logicVolume, Add CsI(pure) as the daughter logicVolume
	//Detector_H 
	solidDetector_H = new G4Box("solidDetector_H", (CsI_sidelength_half+thick)*nCS_H,(CsI_sidelength_half+thick)*nCS, CsI_height_half);
	logicDetector_F = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_F");
	logicDetector_L = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_L");
	logicDetector_B = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_B");
	logicDetector_R = new G4LogicalVolume(solidDetector_H, Mylar, "logicDetector_R");
	//Transformation
	G4Translate3D transZ_H(G4ThreeVector(-(nCS*CsI_sidelength_half+(nCS-0.5)*thick)+(CsI_sidelength_half+thick)*nCS_H, 0*m, Cube_side_length));
	G4Rotate3D rotY_90(90.*deg, G4ThreeVector(0.,1.,0.));
	G4Rotate3D rotY_90_2(90.*2*deg, G4ThreeVector(0.,1.,0.));
	G4Rotate3D rotY_90_3(90.*3*deg, G4ThreeVector(0.,1.,0.));
	//physDetector_H
	physDetector_F = new G4PVPlacement(transZ_H, logicDetector_F, "Detector_F", logicWorld, false, 0, true);
	physDetector_L = new G4PVPlacement((rotY_90)*(transZ_H), logicDetector_L, "Detector_L", logicWorld, false, 0, true);
	physDetector_B = new G4PVPlacement((rotY_90_2)*(transZ_H), logicDetector_B, "Detector_B", logicWorld, false, 0, true);
	physDetector_R = new G4PVPlacement((rotY_90_3)*(transZ_H), logicDetector_R, "Detector_R", logicWorld, false, 0, true);
	
	G4double StartPoint_X = -nCS_H*(CsI_sidelength_half+thick)+CsI_sidelength_half+thick;		//-Half_length+Half_d
	G4double StartPoint_Y = -nCS*(CsI_sidelength_half+thick)+CsI_sidelength_half+thick;			//-Half_length+Half_d
	for (G4int i = 0; i < nCS_H; i++)	//Column
	{
		for (G4int j = 0; j < nCS; j++)	//Row
		{
			G4Translate3D transZ_CsI(G4ThreeVector(StartPoint_X+i*d, StartPoint_Y+j*d, 0.*m));
			G4VPhysicalVolume* physDetector_f = new G4PVPlacement(transZ_CsI, logicDetector, "Detector_f", logicDetector_F, false, i*10+j, true);
			G4VPhysicalVolume* physDetector_l = new G4PVPlacement(transZ_CsI, logicDetector, "Detector_l", logicDetector_L, false, i*10+j, true);
			G4VPhysicalVolume* physDetector_b = new G4PVPlacement(transZ_CsI, logicDetector, "Detector_b", logicDetector_B, false, i*10+j, true);
			G4VPhysicalVolume* physDetector_r = new G4PVPlacement(transZ_CsI, logicDetector, "Detector_r", logicDetector_R, false, i*10+j, true);
		}
	}

	//Detector_V
	solidDetector_V = new G4Box("solidDetector_V", (CsI_sidelength_half+thick)*nCS_V,(CsI_sidelength_half+thick)*nCS_V, CsI_height_half);
	logicDetector_U = new G4LogicalVolume(solidDetector_V, Mylar, "logicDetector_U");
	logicDetector_D = new G4LogicalVolume(solidDetector_V, Mylar, "logicDetector_D");
	//Transformation
	G4Translate3D transZ_V(G4ThreeVector(0*m, 0*m, Cube_side_height));
	G4Rotate3D rotX_90(90.*deg, G4ThreeVector(1.,0.,0.));
	G4Rotate3D rotX_270(-90.*deg, G4ThreeVector(1.,0.,0.));
	//physDetector_H
	G4VPhysicalVolume* physDetector_U = new G4PVPlacement((rotX_270)*(transZ_V), logicDetector_U, "Detector_U", logicWorld, false, 0, true);
	G4VPhysicalVolume* physDetector_D = new G4PVPlacement((rotX_90)*(transZ_V), logicDetector_D, "Detector_D", logicWorld, false, 0, true);
	
	G4double StartPoint_V = -nCS_V*(CsI_sidelength_half+thick)+CsI_sidelength_half+thick;		//-Half_length+Half_d
	for (G4int i = 0; i < nCS_V; i++)
	{
		for (G4int j = 0; j < nCS_V; j++)
		{
			G4Translate3D transZ_CsI(G4ThreeVector(StartPoint_V+i*d, StartPoint_V+j*d, 0*m));
			G4VPhysicalVolume* physDetector_u = new G4PVPlacement(transZ_CsI, logicDetector, "Detector_u", logicDetector_U, false, i*10+j, true);
			G4VPhysicalVolume* physDetector_d = new G4PVPlacement(transZ_CsI, logicDetector, "Detector_d", logicDetector_D, false, i*10+j, true);
		}
	}
}