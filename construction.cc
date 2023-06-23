#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}
MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	G4NistManager *nist = G4NistManager::Instance();
	
	//-------------- Material definitions ----------------------
	
	//G4Material *LH2 = new G4Material("LH2", 0.07085*g/cm3, 1);
	//LH2->AddElement(nist->FindOrBuildElement("H"),2);
	
	G4Material *targetMat = nist->FindOrBuildMaterial("G4_lH2");
	//G4Material *targetMat = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material *detectorMat = nist->FindOrBuildMaterial("G4_lH2");
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
	//G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
	
	//worldMat->GetMaterialPropertiesTable()->DumpTable();
	//LH2->GetMaterialPropertiesTable()->DumpTable();
	
	//-------------- Material Optical Properties ---------------
	
	/*
	G4double energy[2] = {1.7*eV, 3.0*eV};
	G4double rindexLH2[2] = {1.02, 1.055};
	G4double rindexWorld[2] = {1.0, 1.0};
	
	G4MaterialPropertiesTable *mptLH2 = new G4MaterialPropertiesTable();
	mptLH2->AddProperty("RINDEX", energy, rindexLH2, 2);
	
	LH2->SetMaterialPropertiesTable(mptLH2);
	
	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
	mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
	
	worldMat->SetMaterialPropertiesTable(mptWorld);
	
	worldMat->GetMaterialPropertiesTable()->DumpTable();
	*/
	
	//---------------- Component Geometry ---------------------
	
	//--------------------
	// World Volume
	//--------------------
	
	G4Box *solidWorld = new G4Box("solidWorld", 2.0*m,2.0*m,2.0*m);
	
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
	
	//-------------------
	// Target Volume
	//-------------------
	
	
	G4Tubs *solidTarget = new G4Tubs("solidTarget", 0.*cm, 4.*cm, 125./2.*cm, 0.*deg, 360.*deg);
	
	//G4LogicalVolume *logicTarget = new G4LogicalVolume(solidTarget, LH2, "logicTarget");
	//G4LogicalVolume *logicTarget = new G4LogicalVolume(solidTarget, targetMat, "logicTarget");
	
	//logicTarget = new G4LogicalVolume(solidTarget, LH2, "logicTarget");
	logicTarget = new G4LogicalVolume(solidTarget, targetMat, "logicTarget");
	
	//G4VPhysicalVolume *physTarget = new G4PVPlacement(0, G4ThreeVector(0., 0., 125./2.*cm), logicTarget, "physTarget", logicWorld, false, 0, true);
	G4VPhysicalVolume *physTarget = new G4PVPlacement(0, G4ThreeVector(0., 0., 125./2.*cm), logicTarget, "physTarget", logicWorld, false, 0, true);
	
	
	/*
	G4Box *solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);
	
	G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");
	
	G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);
	*/
	
	//-------------------
	// Detector Volume
	//-------------------
	
	//G4Sphere *solidDetector = new G4Sphere("solidDetector", 1.999*m, 2.0*m, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
	
	/*
	G4Tubs *solidDetector = new G4Tubs("solidDetector", 0.*cm, 4.*cm, 0.0001*cm, 0.*deg, 360.*deg);
	
	logicDetector = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector");
	
	
	for(G4int i = 1; i < 35; i++)
	{
		G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0.,0.,(i*5.)*cm), logicDetector, "physDetector", logicWorld, false, i, true);
	}
	*/
	
	//G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0.*cm, 0.*cm, (0.-0.0001)*cm), logicDetector, "physDetector", logicWorld, false, 0, true);
	//G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0.*cm, 0.*cm, 125.*cm), logicDetector, "physDetector", logicWorld, false, 0, true);
	
	return physWorld;
	
}


void MyDetectorConstruction::ConstructSDandField()
{

	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
	
	logicTarget->SetSensitiveDetector(sensDet);
	//logicDetector->SetSensitiveDetector(sensDet);
}

