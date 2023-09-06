#include "construction.hh"
//Define global variables
MyDetectorConstruction::MyDetectorConstruction()
{
    DefineMaterials();


    //User parameters
    foil_material = Ti_;
    scintillator_material = SiO2;
    //Source parameters
    size_inner_radius = 20. / 2 * mm;
    size_outer_radius = 29. / 2 * mm;
    outer_size_of_height = 1. / 2 * mm;
    inner_size_of_height = 0.01 / 2 * mm;
    size_Scintillator = size_outer_radius + 1. / 2 * mm;
    height_of_Scintillator = outer_size_of_height + 4.0 / 2 * mm;
    height_of_Detector = height_of_Scintillator + 1. / 2 * mm;// or inner_size_of_height;

    nCols = 10;
    nRows = 10;
    xWorld = 20.*cm;
    yWorld = 20.*cm;
    zWorld = 20.*cm;
    

    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
    
    fMessenger->DeclareProperty("nCols", nCols, "Number of cols");
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
    
    fMessenger->DeclareProperty("cherenkov", cherenkov, "Activate Chreenkov Detector");
    fMessenger->DeclareProperty("scintillator", SourceScintillator, "Activate Scintillator");
    fMessenger->DeclareProperty("Aerogel", SourceAerogel, "Activate Aerogel");
    fMessenger->DeclareProperty("dielectricMaterial", dielectricMaterial, "Activate dielectricMaterial");
    fMessenger->DeclareProperty("EnergyDetector(cylindrical)", EnergyDetector_Cylindrical, "Activate EnergyDetector(cylindrical)");
    fMessenger->DeclareProperty("isCubic_Detector_Demo", isCubic_Detector_Demo, "Toggle Cubic_Detector_Demo");
	fMessenger->DeclareProperty("isCubic_Detector", isCubic_Detector, "Toggle Cubic_Detector");
    fMessenger->DeclareProperty("isPET", isPET, "Toggle PET");



    dielectricMaterial = false;
    EnergyDetector_Cylindrical = true;
    SourceScintillator = false;
    SourceAerogel = false;
    cherenkov=false;
    isCubic_Detector_Demo=false;
    isCubic_Detector = false;

}

MyDetectorConstruction::~MyDetectorConstruction()
{}
//Define material and its property
void MyDetectorConstruction::DefineMaterials()
{
    //Material numerical Properties
    G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
    G4double rindexAerogel[2] = {1.1, 1.1};
    G4double rindexWorld[2] = {1.0, 1.0};
    G4double rindexNaI[2] = { 1.78, 1.78 };
    G4double reflectivity[2] = { 1.,1. };
    G4double fraction[2] = { 1.,1. };

    //Define Material Class Property
    G4NistManager *nist = G4NistManager::Instance();
    G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    G4MaterialPropertiesTable* mptNaI = new G4MaterialPropertiesTable();
    G4MaterialPropertiesTable* mptMirror = new G4MaterialPropertiesTable();


    //Define Materials
    worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
    worldMat->SetMaterialPropertiesTable(mptWorld);

    C = nist->FindOrBuildElement("C");
    
    
   
    H2O = new G4Material("H2O", 1.000*g/cm3, 2);
    H2O->AddElement(nist->FindOrBuildElement("H"), 2);
    H2O->AddElement(nist->FindOrBuildElement("O"), 1);

    Kapton = nist->FindOrBuildMaterial("G4_KAPTON");
    
    SiO2 = new G4Material("SiO2", 0.125*g/cm3, 2);
    SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO2->AddElement(nist->FindOrBuildElement("O"), 2);
    
    Polystyrene = new G4Material("Polystyrene", 1.06 * g / cm3, 2);
    Polystyrene->AddElement(nist->FindOrBuildElement("C"), 8);
    Polystyrene->AddElement(nist->FindOrBuildElement("H"), 8);

    Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
    Aerogel->AddMaterial(SiO2, 62.5*perCent);
    Aerogel->AddMaterial(H2O, 37.4*perCent);
    Aerogel->AddElement(C, 0.1*perCent);
    mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);
    Aerogel->SetMaterialPropertiesTable(mptAerogel);
    
    NaI = new G4Material("NaI", 3.67 * g / cm3, 2);
    NaI->AddElement(nist->FindOrBuildElement("Na"), 1);
    NaI->AddElement(nist->FindOrBuildElement("I"), 1);
    mptNaI->AddProperty("RINDEX", energy, rindexNaI, 2);
    mptNaI->AddProperty("SCINTILLATIONCOMPONENT1", energy,fraction,2);
    mptNaI->AddConstProperty("SCINTILLATIONYIELD", 38. / keV);
    mptNaI->AddConstProperty("RESOLUTIONSCALE", 1.);
    mptNaI->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 250.*ns);
    mptNaI->AddConstProperty("SCINTILLATIONYIELD1", 1.);
    NaI->SetMaterialPropertiesTable(mptNaI);
    
    NaCl = new G4Material("NaCl", 2.17 * g / cm3, 2);
    NaCl->AddElement(nist->FindOrBuildElement("Na"), 1);
    NaCl->AddElement(nist->FindOrBuildElement("Cl"), 4);
    
    Ti = nist->FindOrBuildElement("Ti");
    Ti_ = new G4Material("Ti_", 4.507 * g / cm3, 1);
    Ti_->AddElement(Ti,1.);

    
    mirrorSurface = new G4OpticalSurface("mirrorSurface");
    mptMirror->AddProperty("REFLECTIVITY",energy,reflectivity,2);
    mirrorSurface->SetMaterialPropertiesTable(mptMirror);
    mirrorSurface->SetType(dielectric_metal);
    mirrorSurface->SetFinish(ground);
    mirrorSurface->SetModel(unified);
    
}
//Define source parameters



//Define Mother world
G4VPhysicalVolume *MyDetectorConstruction::Construct()
{       
    /* define a volumn of world contain air, it needs to be include size,what inside the world */
    /*In general it needs to define three volumne, */
    /*1st solid volume: tell the size*/
    /*2nd logical volume: tell what are the material inside the world*/
    /*3rd physical volume: place volumes into the applications*/

    /*To define a solid volumne, G4Box("name",half size of x, y, z)*/
    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    /*To insert particle inside that solid(physical volume)*/
    /*G4LogicalVolume("the solid", "the material", "the name")*/
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    /*To place it into a Physical Volume */
    /*G4PVPlacement(rotation, center of coordinate(Use G4ThreeVector(x,y,z)), logic volume, name of the physical world,
    "Mother logical volume name",boolean,copy number(if need insert this volume multiple time), whether check overlap)*/
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    if(isPET){
        ConstructPET();
    }
    if (cherenkov)
		ConstructCherenkov();
	if (isCubic_Detector_Demo)
		ConstructCubic_Detector_Demo();
	if (isCubic_Detector)
		ConstructCubic_Detector();
    if (dielectricMaterial)
        ConstructDielectric();
    if (EnergyDetector_Cylindrical)
        Construct_Energy_detector_Cylindrical();
    if (SourceScintillator)
        ConstructSourceScintillator();
    if (SourceAerogel)
        ConstructAerogel();
    return physWorld;
}

//Define Ideal Detector
void MyDetectorConstruction::Construct_Energy_detector_Cylindrical() {

    G4Tubs* InnerTube = new G4Tubs("InnerTube", 0., size_Scintillator , height_of_Detector, 0. * deg, 360. * deg);
    G4Tubs* OuterTube = new G4Tubs("OuterTube", 0., size_Scintillator + 2. * mm, height_of_Detector + 1. * mm, 0. * deg, 360. * deg);
    G4SubtractionSolid* CylindricalDetector = new G4SubtractionSolid("CylindricalDetector", OuterTube, InnerTube);
    logicEnergyDetector_cylindrical = new G4LogicalVolume(CylindricalDetector, worldMat, "CylindricallogicEnergyDetector");
    physEnergyDetector_cylindrical = new G4PVPlacement(0, G4ThreeVector(0., 0., 0. * m), logicEnergyDetector_cylindrical, "CylindricalphysEnergyDetector", logicWorld, false, 0, true);

}
//Define the foil around the source
void MyDetectorConstruction::ConstructDielectric() {
	G4Material* foil_material = Ti_; //Kapton Or Ti_ or worldMat
	G4ThreeVector Center = G4ThreeVector(0. * m, 0. * m, 0. * m);
	G4double size_inner_source = 4./2 * mm;
	G4double size_inner_radius = 20./2 * mm;
	G4double size_outer_radius = 29./2 * mm;
	G4double outer_size_of_height = 1./2 * mm;
	G4double inner_size_of_height = 0.01/2 * mm;
	G4Tubs* OuterDielectric = new G4Tubs("Outer", size_inner_radius, size_outer_radius, outer_size_of_height, 0. * deg, 360. * deg);
	G4Tubs* InnerSource = new G4Tubs("source", 0. * nm, size_inner_radius, inner_size_of_height, 0. * deg, 360. * deg);
	logicDielectric = new G4LogicalVolume(OuterDielectric, foil_material, "logicDielectric");
	physDielectric = new G4PVPlacement(0, Center, logicDielectric, "physDielectric", logicWorld, false, 0, true);
	logicSource = new G4LogicalVolume(InnerSource, foil_material,"logicSource");
	physSource = new G4PVPlacement(0, Center, logicSource, "physSource", logicWorld, false, 0, true);
}
void MyDetectorConstruction::ConstructSourceScintillator() {
    G4Tubs* OuterDielectric = new G4Tubs("Outer", size_inner_radius, size_outer_radius, outer_size_of_height, 0. * deg, 360. * deg);
    G4Tubs* InnerSource = new G4Tubs("source", 0. * nm, size_inner_radius, inner_size_of_height, 0. * deg, 360. * deg);
    G4Tubs* BaseSourceScintillator = new G4Tubs("base", 0. * nm, size_Scintillator, height_of_Scintillator, 0. * deg, 360. * deg);
    G4SubtractionSolid* SourceScintillator_SubtractInner = new G4SubtractionSolid("SourceScintillatorSubtractInner", BaseSourceScintillator, InnerSource);
    G4SubtractionSolid* SourceScintillator_SubtractOuter = new G4SubtractionSolid("SourceScintillatorSubtractInner", SourceScintillator_SubtractInner, OuterDielectric);
    
    
    logic_SourceScintillator = new G4LogicalVolume(SourceScintillator_SubtractOuter, scintillator_material, "CylindricallogicEnergyDetector");
    phys_SourceScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0. * m), logic_SourceScintillator, "CylindricalphysEnergyDetector", logicWorld, false, 0, true);

}
void MyDetectorConstruction::ConstructAerogel() {
    G4Tubs* OuterDielectric = new G4Tubs("Outer", size_inner_radius, size_outer_radius, outer_size_of_height, 0. * deg, 360. * deg);
    G4Tubs* InnerSource = new G4Tubs("source", 0. * nm, size_inner_radius, inner_size_of_height, 0. * deg, 360. * deg);
    Foil_Geometry = new G4UnionSolid("Foil_Geometry", OuterDielectric, InnerSource);
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

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    if (EnergyDetector_Cylindrical) {
        logicEnergyDetector_cylindrical->SetSensitiveDetector(sensDet);
    }
}

void MyDetectorConstruction::ConstructCubic_Detector()
{
	solidDetector = new G4Box("solidDetector", 0.005*m, 0.005*m, 0.005*m);
	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

	for (G4int i = 0; i < nCols; i++)
	{
		for (G4int j = 0; j < nRows; j++)
		{
			G4VPhysicalVolume* physDetector_f = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nCols, -0.5*m+(j+0.5)*m/nRows, 0.505*m), logicDetector, "physDetector_f", logicWorld, false, i*100+j, true);
			G4VPhysicalVolume* physDetector_b = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nCols, -0.5*m+(j+0.5)*m/nRows, -0.505*m), logicDetector, "physDetector_b", logicWorld, false, i*100+j, true);
			G4VPhysicalVolume* physDetector_l = new G4PVPlacement(0, G4ThreeVector(0.505*m, 0.5*m-(j+0.5)*m/nRows, -0.5*m+(i+0.5)*m/nCols), logicDetector, "physDetector_l", logicWorld, false, i*100+j, true);
			G4VPhysicalVolume* physDetector_r = new G4PVPlacement(0, G4ThreeVector(-0.505*m, 0.5*m-(j+0.5)*m/nRows, -0.5*m+(i+0.5)*m/nCols), logicDetector, "physDetector_r", logicWorld, false, i*100+j, true);
			G4VPhysicalVolume* physDetector_u = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nCols, 0.505*m, 0.5*m-(j+0.5)*m/nRows), logicDetector, "physDetector_u", logicWorld, false, i*100+j, true);
			G4VPhysicalVolume* physDetector_d = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nCols, -0.505*m, 0.5*m-(j+0.5)*m/nRows), logicDetector, "physDetector_d", logicWorld, false, i*100+j, true);
		}
	}
}

void MyDetectorConstruction::ConstructCubic_Detector_Demo()
{
	solidDetector_FB = new G4Box("solidDetector", 0.51*m, 0.51*m, 0.005*m);
	solidDetector_LR = new G4Box("solidDetector", 0.005*m, 0.51*m, 0.51*m);
	solidDetector_UD = new G4Box("solidDetector", 0.51*m, 0.005*m, 0.51*m);
	solidDetector = new G4Box("solidDetector", 0.005*m, 0.005*m, 0.005*m);

	logicDetector_F = new G4LogicalVolume(solidDetector_FB, worldMat, "logicDetector_F");
	logicDetector_B = new G4LogicalVolume(solidDetector_FB, worldMat, "logicDetector_B");
	logicDetector_L = new G4LogicalVolume(solidDetector_LR, worldMat, "logicDetector_L");
	logicDetector_R = new G4LogicalVolume(solidDetector_LR, worldMat, "logicDetector_R");
	logicDetector_U = new G4LogicalVolume(solidDetector_UD, worldMat, "logicDetector_U");
	logicDetector_D = new G4LogicalVolume(solidDetector_UD, worldMat, "logicDetector_D");
	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

	physDetector_F = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.505*m), logicDetector_F, "physDetector_F", logicWorld, false, 0, true);
	physDetector_B = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, -0.505*m), logicDetector_B, "physDetector_B", logicWorld, false, 0, true);
	physDetector_L = new G4PVPlacement(0, G4ThreeVector(0.505*m, 0.*m, 0.*m), logicDetector_L, "physDetector_L", logicWorld, false, 0, true);
	physDetector_R = new G4PVPlacement(0, G4ThreeVector(-0.505*m, 0.*m, 0.*m), logicDetector_R, "physDetector_R", logicWorld, false, 0, true);
	physDetector_U = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.505*m, 0.*m), logicDetector_U, "physDetector_U", logicWorld, false, 0, true);
	physDetector_D = new G4PVPlacement(0, G4ThreeVector(0.*m, -0.505*m, 0.*m), logicDetector_D, "physDetector_D", logicWorld, false, 0, true);

	for (G4int i = 0; i < nCols; i++)
	{
		for (G4int j = 0; j < nRows; j++)
		{
			G4VPhysicalVolume* physDetector_f = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nCols, -0.5*m+(j+0.5)*m/nRows, 0.*m), logicDetector, "physDetector_f", logicDetector_F, false, i*100+j, true);
			G4VPhysicalVolume* physDetector_b = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nCols, -0.5*m+(j+0.5)*m/nRows, 0.*m), logicDetector, "physDetector_b", logicDetector_B, false, i*100+j, true);
			G4VPhysicalVolume* physDetector_l = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.5*m-(j+0.5)*m/nRows, -0.5*m+(i+0.5)*m/nCols), logicDetector, "physDetector_l", logicDetector_L, false, i*100+j, true);
			G4VPhysicalVolume* physDetector_r = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.5*m-(j+0.5)*m/nRows, -0.5*m+(i+0.5)*m/nCols), logicDetector, "physDetector_r", logicDetector_R, false, i*100+j, true);
			G4VPhysicalVolume* physDetector_u = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nCols, 0.*m, 0.5*m-(j+0.5)*m/nRows), logicDetector, "physDetector_u", logicDetector_U, false, i*100+j, true);
			G4VPhysicalVolume* physDetector_d = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nCols, 0.*m, 0.5*m-(j+0.5)*m/nRows), logicDetector, "physDetector_d", logicDetector_D, false, i*100+j, true);
		}
	}
}

void MyDetectorConstruction::ConstructPET()
{
	//G4Tubs(Name, Inner radius, Outer radius, Length, Initial angle, Final angle)
	//solidPET = new G4Tubs("solidPET", 10*cm, 20*cm, 30*cm, 0*deg, 360*deg);
	solidPET = new G4Box("solidPET", 5*cm, 5*cm, 6*cm);
	logicPET = new G4LogicalVolume(solidPET, NaI, "logicPET");
	fScoringVolume = logicPET;
	for (G4int i = 0; i < 6; i++)
	{
		for (G4int j = 0; j < 16; j++)
		{
			G4Rotate3D rotZ(j*360./16*deg, G4ThreeVector(0.,0.,1));
			G4Translate3D transX_PET(G4ThreeVector(5./tan(22.5/2*deg)*cm+5.*cm, 0*cm, -40*cm+i*15.*cm));
			G4Transform3D transform_PET = (rotZ)*(transX_PET);
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
