#include "parallelconstruction.hh"

MyParallelConstruction::MyParallelConstruction(G4String worldName) : G4VUserParallelWorld(worldName),logicDetector(0)
{}
MyParallelConstruction::~MyParallelConstruction()
{}

void MyParallelConstruction::Construct()
{
	G4VPhysicalVolume* ghostWorld = GetWorld();
  	G4LogicalVolume *logicWorld = ghostWorld->GetLogicalVolume();
  
	//G4Material *detectorMat = nist->FindOrBuildMaterial("G4_Galactic");

	G4Tubs *solidDetector = new G4Tubs("solidDetector", 0.*cm, 4.*cm, 0.0001*cm, 0.*deg, 360.*deg);
	logicDetector = new G4LogicalVolume(solidDetector, 0, "logicDetector");
	for(G4int i = 1; i < 125; i++)
	{
		G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0.,0.,(i*1.)*cm), logicDetector, "physDetector", logicWorld, false, i, true);
	}
	
	return;
}

void MyParallelConstruction::ConstructSD()
{
	G4String sensitiveDetectorName = "physDetector";
	MySensitiveDetector *sensDet = new MySensitiveDetector(sensitiveDetectorName);
	logicDetector->SetSensitiveDetector(sensDet);

}
