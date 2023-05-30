#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "Randomize.hh"
#include "G4Exp.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	MyPrimaryGenerator();
	~MyPrimaryGenerator();
	
	virtual G4ThreeVector GetHyperonVertex();
	virtual G4double GetPhotonE();
	virtual G4ThreeVector GetHyperonAngle();
	virtual G4double GetHyperonMom(G4double);
	
	virtual void GeneratePrimaries(G4Event*);
	
private:
	G4ParticleGun *fParticleGun; 
};

#endif
