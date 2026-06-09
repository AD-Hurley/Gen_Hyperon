#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}
MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	G4NistManager *nist = G4NistManager::Instance();
	
	//-------------- Material definitions ----------------------
		
	G4Material *targetMat = nist->FindOrBuildMaterial("G4_Galactic");
	//G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
		
	//-------------- Material Optical Properties ---------------
	
	/*
	G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
	G4double rindexAerogel[2] = {1.1, 1.1};
	G4double rindexWorld[2] = {1.0, 1.0};
	
	G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
	mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);
	
	Aerogel->SetMaterialPropertiesTable(mptAerogel);
	
	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
	mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
	
	worldMat->SetMaterialPropertiesTable(mptWorld);
	*/
	
	//---------------- Component Geometry ---------------------
	
	//--------------------
	// World Volume
	//--------------------
	
	G4Box *solidWorld = new G4Box("solidWorld", 2.0*m,2.0*m,2.0*m);
	
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, targetMat, "logicWorld");
	
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
	
	//-------------------
	// Target Volume
	//-------------------
	
	
	//G4Tubs *solidTarget = new G4Tubs("solidTarget", 0.*cm, 4.*cm, 125./2.*cm, 0*deg, 360*deg);
	
	//G4LogicalVolume *logicTarget = new G4LogicalVolume(solidTarget, worldMat, "logicTarget"); //!!!!! SET TO VACUUM MATERIAL
	
	//G4VPhysicalVolume *physTarget = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicTarget, "physTarget", logicWorld, false, 0, true);
	
	
	/*
	G4Box *solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);
	
	G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");
	
	G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);
	*/
	
	//-------------------
	// Detector Volume
	//-------------------
	
	G4Sphere *solidDetector = new G4Sphere("solidDetector", 1.999*m, 2.0*m, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
	//G4Sphere *solidDetector = new G4Sphere("solidDetector", 0*m, 2.0*m, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
	
	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");
	
	G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 0.*m), logicDetector, "physDetector", logicWorld, false, 0, true);
	
	
	return physWorld;
	
}


void MyDetectorConstruction::ConstructSDandField()
{

	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
	
	logicDetector->SetSensitiveDetector(sensDet);
}

