#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	DefineMaterials();

	G4int A = 0;
	// Change 100x100 before Run Detector
	nCols = 10;
	nRows = 10;
	isPET = false;
	isCherenkov = false; // Turn it true for Detector
	isCubic_Detector_Demo = false;
	isCubic_Detector = true;

	logicDetector = NULL;
	logicEnergyDetector_cylindrical = NULL;

	// From Eltis
	dielectricMaterial = true;
	SourceScintillator = true;
	EnergyDetector_Cylindrical = false;
	// End Eltis

	fMessenger = new G4GenericMessenger(this, "/MyDetector/", "Detector Construction");
	fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
	fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
	fMessenger->DeclareProperty("control/execute rebuild.mac", A, "Rebuild macro");
	fMessenger->DeclareProperty("control/execute rebuild_10.mac", A, "Rebuild macro");
	fMessenger->DeclareProperty("control/execute rebuild_100.mac", A, "Rebuild macro");
	fMessenger->DeclareProperty("control/execute run_t1.mac", A, "Thread 1 macro");
	fMessenger->DeclareProperty("control/execute run_t4.mac", A, "Thread 4 macro");
	fMessenger->DeclareProperty("isPET", isPET, "Toggle PET");
	fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Toggle Cherenkov");
	fMessenger->DeclareProperty("isCubic_Detector_Demo", isCubic_Detector_Demo, "Toggle Cubic_Detector_Demo");
	fMessenger->DeclareProperty("isCubic_Detector", isCubic_Detector, "Toggle Cubic_Detector");

}
MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
	G4NistManager* nist = G4NistManager::Instance();

	SiO2 = new G4Material("SiO2", 2.2021 * g / cm3, 2);
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

	H2O = new G4Material("H2O", 1.000 * g / cm3, 2);
	H2O->AddElement(nist->FindOrBuildElement("H"), 2);
	H2O->AddElement(nist->FindOrBuildElement("O"), 1);

	C = nist->FindOrBuildElement("C");

	Aerogel = new G4Material("Aerogel", 0.200 * g / cm3, 3);
	Aerogel->AddMaterial(SiO2, 62.5 * perCent);
	Aerogel->AddMaterial(H2O, 37.4 * perCent);
	Aerogel->AddElement(C, 0.1 * perCent);

	G4double energy[2] = { 1.239841939 * eV / 0.9, 1.239841939 * eV / 0.2 };
	G4double rindexAerogel[2] = { 1.1, 1.1 };
	G4MaterialPropertiesTable* mptAerogel = new G4MaterialPropertiesTable();
	mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);
	Aerogel->SetMaterialPropertiesTable(mptAerogel);

	worldMat = nist->FindOrBuildMaterial("G4_AIR");


	NaI = new G4Material("NaI", 3.67 * g / cm3, 2);
	Na = nist->FindOrBuildElement("Na");
	I = nist->FindOrBuildElement("I");
	NaI->AddElement(Na, 1);
	NaI->AddElement(I, 1);

	G4double rindexWorld[2] = { 1.0, 1.0 };
	G4MaterialPropertiesTable* mptworld = new G4MaterialPropertiesTable();
	mptworld->AddProperty("RINDEX", energy, rindexWorld, 2);
	worldMat->SetMaterialPropertiesTable(mptworld);

	// From Eltis
	Kapton = nist->FindOrBuildMaterial("G4_KAPTON");

	Ti = nist->FindOrBuildElement("Ti");
	Ti_ = new G4Material("Ti_", 4.507 * g / cm3, 1);
	Ti_->AddElement(Ti, 1.);
	// End Eltis

}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	G4double xWorld = 0.51 * m;
	G4double yWorld = 0.51 * m;
	G4double zWorld = 0.51 * m;

	// a cubic with volume 1 m^3
	solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

	if (isPET)
		ConstructPET();
	if (isCherenkov)
		ConstructCherenkov();
	if (isCubic_Detector_Demo)
		ConstructCubic_Detector_Demo();
	if (isCubic_Detector)
		ConstructCubic_Detector();

	// From Eltis
	if (dielectricMaterial)
		ConstructDielectric();
	if (SourceScintillator)
		ConstructSourceScintillator();
	if (EnergyDetector_Cylindrical)
		Construct_Energy_detector_Cylindrical();
	// End Eltis

	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
	MySensentiveDetector* sensDet = new MySensentiveDetector("SensitiveDetector");
	if (logicDetector != NULL)
	{
		logicDetector->SetSensitiveDetector(sensDet);
	}
	if (logicEnergyDetector_cylindrical != NULL)
		logicEnergyDetector_cylindrical->SetSensitiveDetector(sensDet);
	//logicPET->SetSensitiveDetector(sensDet);
}

// From Eltis
void MyDetectorConstruction::ConstructDielectric() {
	G4Material* foil_material = Kapton; //Kapton Or Ti_ or worldMat
	G4ThreeVector Center = G4ThreeVector(0. * m, 0. * m, 0. * m);
	G4double size_inner_source = 4. / 2 * mm;
	G4double size_inner_radius = 20. / 2 * mm;
	G4double size_outer_radius = 29. / 2 * mm;
	G4double outer_size_of_height = 1. / 2 * mm;
	G4double inner_size_of_height = 0.01 / 2 * mm;
	G4Tubs* OuterDielectric = new G4Tubs("Outer", size_inner_radius, size_outer_radius, outer_size_of_height, 0. * deg, 360. * deg);
	G4Tubs* InnerSource = new G4Tubs("source", 0. * nm, size_inner_radius, inner_size_of_height, 0. * deg, 360. * deg);
	logicDielectric = new G4LogicalVolume(OuterDielectric, foil_material, "logicDielectric");
	physDielectric = new G4PVPlacement(0, Center, logicDielectric, "physDielectric", logicWorld, false, 0, true);
	logicSource = new G4LogicalVolume(InnerSource, foil_material, "logicSource");
	physSource = new G4PVPlacement(0, Center, logicSource, "physSource", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructSourceScintillator() {
	G4double size_inner_source = 4. / 2 * mm;
	G4double size_inner_radius = 20. / 2 * mm;
	G4double size_outer_radius = 29. / 2 * mm;
	G4double outer_size_of_height = 1. / 2 * mm;
	G4double inner_size_of_height = 0.01 / 2 * mm;
	G4Tubs* OuterDielectric = new G4Tubs("Outer", size_inner_radius, size_outer_radius, outer_size_of_height, 0. * deg, 360. * deg);
	G4Tubs* InnerSource = new G4Tubs("source", 0. * nm, size_inner_radius, inner_size_of_height, 0. * deg, 360. * deg);
	G4Sphere* BaseSourceScintillator = new G4Sphere("Base", 0. * nm, size_outer_radius + 1 * mm, 0 * deg, 360 * deg, 0 * deg, 360 * deg);

	G4SubtractionSolid* SourceScintillator_SubtractInner = new G4SubtractionSolid("SourceScintillatorSubtractInner", BaseSourceScintillator, InnerSource);
	G4SubtractionSolid* SourceScintillator_SubtractOuter = new G4SubtractionSolid("SourceScintillatorSubtractInner", SourceScintillator_SubtractInner, OuterDielectric);
	logic_SourceScintillator = new G4LogicalVolume(SourceScintillator_SubtractOuter, SiO2, "CylindricallogicEnergyDetector");
	phys_SourceScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0. * m), logic_SourceScintillator, "CylindricalphysEnergyDetector", logicWorld, false, 0, true);

}
void MyDetectorConstruction::Construct_Energy_detector_Cylindrical() {
	G4double size_inner_radius = 30 * mm;
	G4double size_outer_radius = size_inner_radius + 3. * cm;
	G4double size_inner_height = 3 * mm;
	G4double size_outer_height = size_inner_height + 3. * cm;
	G4Tubs* InnerTube = new G4Tubs("InnerTube", 0., size_inner_radius, size_inner_height, 0. * deg, 360. * deg);
	G4Tubs* OuterTube = new G4Tubs("OuterTube", 0, size_outer_radius, size_outer_height, 0. * deg, 360. * deg);
	G4SubtractionSolid* CylindricalDetector = new G4SubtractionSolid("CylindricalDetector", OuterTube, InnerTube);
	logicEnergyDetector_cylindrical = new G4LogicalVolume(CylindricalDetector, worldMat, "CylindricallogicEnergyDetector");
	physEnergyDetector_cylindrical = new G4PVPlacement(0, G4ThreeVector(0., 0., 0. * m), logicEnergyDetector_cylindrical, "CylindricalphysEnergyDetector", logicWorld, false, 0, true);
}

// End Eltis

void MyDetectorConstruction::ConstructCubic_Detector()
{
	solidDetector = new G4Box("solidDetector", 0.005 * m, 0.005 * m, 0.005 * m);
	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

	for (G4int i = 0; i < nCols; i++)
	{
		for (G4int j = 0; j < nRows; j++)
		{
			G4VPhysicalVolume* physDetector_f = new G4PVPlacement(0, G4ThreeVector(-0.5 * m + (i + 0.5) * m / nCols, -0.5 * m + (j + 0.5) * m / nRows, 0.505 * m), logicDetector, "physDetector_f", logicWorld, false, i * 100 + j, true);
			G4VPhysicalVolume* physDetector_b = new G4PVPlacement(0, G4ThreeVector(-0.5 * m + (i + 0.5) * m / nCols, -0.5 * m + (j + 0.5) * m / nRows, -0.505 * m), logicDetector, "physDetector_b", logicWorld, false, i * 100 + j, true);
			G4VPhysicalVolume* physDetector_l = new G4PVPlacement(0, G4ThreeVector(0.505 * m, 0.5 * m - (j + 0.5) * m / nRows, -0.5 * m + (i + 0.5) * m / nCols), logicDetector, "physDetector_l", logicWorld, false, i * 100 + j, true);
			G4VPhysicalVolume* physDetector_r = new G4PVPlacement(0, G4ThreeVector(-0.505 * m, 0.5 * m - (j + 0.5) * m / nRows, -0.5 * m + (i + 0.5) * m / nCols), logicDetector, "physDetector_r", logicWorld, false, i * 100 + j, true);
			G4VPhysicalVolume* physDetector_u = new G4PVPlacement(0, G4ThreeVector(-0.5 * m + (i + 0.5) * m / nCols, 0.505 * m, 0.5 * m - (j + 0.5) * m / nRows), logicDetector, "physDetector_u", logicWorld, false, i * 100 + j, true);
			G4VPhysicalVolume* physDetector_d = new G4PVPlacement(0, G4ThreeVector(-0.5 * m + (i + 0.5) * m / nCols, -0.505 * m, 0.5 * m - (j + 0.5) * m / nRows), logicDetector, "physDetector_d", logicWorld, false, i * 100 + j, true);
		}
	}
}

void MyDetectorConstruction::ConstructCubic_Detector_Demo()
{
	solidDetector_FB = new G4Box("solidDetector", 0.51 * m, 0.51 * m, 0.005 * m);
	solidDetector_LR = new G4Box("solidDetector", 0.005 * m, 0.51 * m, 0.51 * m);
	solidDetector_UD = new G4Box("solidDetector", 0.51 * m, 0.005 * m, 0.51 * m);
	solidDetector = new G4Box("solidDetector", 0.005 * m, 0.005 * m, 0.005 * m);

	logicDetector_F = new G4LogicalVolume(solidDetector_FB, worldMat, "logicDetector_F");
	logicDetector_B = new G4LogicalVolume(solidDetector_FB, worldMat, "logicDetector_B");
	logicDetector_L = new G4LogicalVolume(solidDetector_LR, worldMat, "logicDetector_L");
	logicDetector_R = new G4LogicalVolume(solidDetector_LR, worldMat, "logicDetector_R");
	logicDetector_U = new G4LogicalVolume(solidDetector_UD, worldMat, "logicDetector_U");
	logicDetector_D = new G4LogicalVolume(solidDetector_UD, worldMat, "logicDetector_D");
	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

	physDetector_F = new G4PVPlacement(0, G4ThreeVector(0. * m, 0. * m, 0.505 * m), logicDetector_F, "physDetector_F", logicWorld, false, 0, true);
	physDetector_B = new G4PVPlacement(0, G4ThreeVector(0. * m, 0. * m, -0.505 * m), logicDetector_B, "physDetector_B", logicWorld, false, 0, true);
	physDetector_L = new G4PVPlacement(0, G4ThreeVector(0.505 * m, 0. * m, 0. * m), logicDetector_L, "physDetector_L", logicWorld, false, 0, true);
	physDetector_R = new G4PVPlacement(0, G4ThreeVector(-0.505 * m, 0. * m, 0. * m), logicDetector_R, "physDetector_R", logicWorld, false, 0, true);
	physDetector_U = new G4PVPlacement(0, G4ThreeVector(0. * m, 0.505 * m, 0. * m), logicDetector_U, "physDetector_U", logicWorld, false, 0, true);
	physDetector_D = new G4PVPlacement(0, G4ThreeVector(0. * m, -0.505 * m, 0. * m), logicDetector_D, "physDetector_D", logicWorld, false, 0, true);

	for (G4int i = 0; i < nCols; i++)
	{
		for (G4int j = 0; j < nRows; j++)
		{
			G4VPhysicalVolume* physDetector_f = new G4PVPlacement(0, G4ThreeVector(-0.5 * m + (i + 0.5) * m / nCols, -0.5 * m + (j + 0.5) * m / nRows, 0. * m), logicDetector, "physDetector_f", logicDetector_F, false, i * 100 + j, true);
			G4VPhysicalVolume* physDetector_b = new G4PVPlacement(0, G4ThreeVector(-0.5 * m + (i + 0.5) * m / nCols, -0.5 * m + (j + 0.5) * m / nRows, 0. * m), logicDetector, "physDetector_b", logicDetector_B, false, i * 100 + j, true);
			G4VPhysicalVolume* physDetector_l = new G4PVPlacement(0, G4ThreeVector(0. * m, 0.5 * m - (j + 0.5) * m / nRows, -0.5 * m + (i + 0.5) * m / nCols), logicDetector, "physDetector_l", logicDetector_L, false, i * 100 + j, true);
			G4VPhysicalVolume* physDetector_r = new G4PVPlacement(0, G4ThreeVector(0. * m, 0.5 * m - (j + 0.5) * m / nRows, -0.5 * m + (i + 0.5) * m / nCols), logicDetector, "physDetector_r", logicDetector_R, false, i * 100 + j, true);
			G4VPhysicalVolume* physDetector_u = new G4PVPlacement(0, G4ThreeVector(-0.5 * m + (i + 0.5) * m / nCols, 0. * m, 0.5 * m - (j + 0.5) * m / nRows), logicDetector, "physDetector_u", logicDetector_U, false, i * 100 + j, true);
			G4VPhysicalVolume* physDetector_d = new G4PVPlacement(0, G4ThreeVector(-0.5 * m + (i + 0.5) * m / nCols, 0. * m, 0.5 * m - (j + 0.5) * m / nRows), logicDetector, "physDetector_d", logicDetector_D, false, i * 100 + j, true);
		}
	}
}

void MyDetectorConstruction::ConstructPET()
{
	//G4Tubs(Name, Inner radius, Outer radius, Length, Initial angle, Final angle)
	//solidPET = new G4Tubs("solidPET", 10*cm, 20*cm, 30*cm, 0*deg, 360*deg);
	solidPET = new G4Box("solidPET", 5 * cm, 5 * cm, 6 * cm);
	logicPET = new G4LogicalVolume(solidPET, NaI, "logicPET");
	fScoringVolume = logicPET;
	for (G4int i = 0; i < 6; i++)
	{
		for (G4int j = 0; j < 16; j++)
		{
			G4Rotate3D rotZ(j * 360. / 16 * deg, G4ThreeVector(0., 0., 1));
			G4Translate3D transX_PET(G4ThreeVector(5. / tan(22.5 / 2 * deg) * cm + 5. * cm, 0 * cm, -40 * cm + i * 15. * cm));
			G4Transform3D transform_PET = (rotZ) * (transX_PET);
			physPET = new G4PVPlacement(transform_PET, logicPET, "physPET", logicWorld, false, 0, true);
		};
	};
}

void MyDetectorConstruction::ConstructCherenkov()
{
	solidRadiator = new G4Box("solidRadiator", 0.4 * m, 0.4 * m, 0.01 * m);
	logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");
	fScoringVolume = logicRadiator;
	physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25 * m), logicRadiator, "physRadiator", logicWorld, false, 0, true);
}