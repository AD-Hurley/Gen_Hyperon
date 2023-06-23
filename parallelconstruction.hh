#ifndef PARALLELCONSTRUCTION_HH
#define PARALLELCONSTRUCTION_HH

#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4VUserParallelWorld.hh"
#include "globals.hh"
#include "detector.hh"

class MyParallelConstruction : public G4VUserParallelWorld
{
public:
	MyParallelConstruction(G4String worldName);
	virtual ~MyParallelConstruction();
	
	virtual void Construct();
	virtual void ConstructSD();
	
private:
	G4LogicalVolume *logicDetector;

};



#endif
