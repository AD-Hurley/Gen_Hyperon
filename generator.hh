#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4PrimaryParticle.hh"
#include "G4DynamicParticle.hh"
#include "G4DecayProducts.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "Randomize.hh"
#include "G4Exp.hh"
#include "G4LorentzVector.hh"
#include "G4AnalysisManager.hh"
#include "data_CLAS2006.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	MyPrimaryGenerator();
	~MyPrimaryGenerator();

	virtual void GeneratePrimaries(G4Event*);
	virtual std::array<G4double,2> genPhiK(G4double Q2, G4double W2, G4double epsilon, G4double beamPol);
	virtual G4double genThetaP(G4double beamPol);
	
	G4double beamE = 11.0;
	
	G4ParticleDefinition* electron = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  G4ParticleDefinition* proton = G4ParticleTable::GetParticleTable()->FindParticle("proton");
	G4ParticleDefinition* kaon = G4ParticleTable::GetParticleTable()->FindParticle("kaon+");
	G4ParticleDefinition* hyperon = G4ParticleTable::GetParticleTable()->FindParticle("lambda");
	G4ParticleDefinition* pion = G4ParticleTable::GetParticleTable()->FindParticle("pi-");
	//G4ParticleDefinition* hyperon = G4ParticleTable::GetParticleTable()->FindParticle("sigma0");
  
	G4double proton_mass = proton->GetPDGMass()*0.001;
	G4double kaon_mass = kaon->GetPDGMass()*0.001;
	G4double hyperon_mass = hyperon->GetPDGMass()*0.001;
	G4double pion_mass = pion->GetPDGMass()*0.001;
	
	G4double hel_Momentum = std::sqrt(std::pow((std::pow(hyperon_mass,2) + std::pow(proton_mass,2) - std::pow(pion_mass,2)),2) - 4*std::pow(hyperon_mass,2)*std::pow(proton_mass,2))/(2*hyperon_mass);
	G4double proton_helE = std::sqrt(std::pow(proton_mass,2) + std::pow(hel_Momentum,2));
	G4double pion_helE =  std::sqrt(std::pow(pion_mass,2) + std::pow(hel_Momentum,2)); 
	
	//G4double proton_helE = (std::pow(hyperon_mass,2) + std::pow(proton_mass,2) - std::pow(pion_mass,2))/(2*hyperon_mass);
	//G4double pion_helE =  (std::pow(hyperon_mass,2) - std::pow(proton_mass,2) + std::pow(pion_mass,2))/(2*hyperon_mass);

	//W2 minimum is the mass required to produce K and Y with 0 momentum
	G4double W2_min = std::pow((kaon_mass + hyperon_mass),2);
	G4double W2_max = std::pow(proton_mass,2) + 2*proton_mass*beamE;
	
	//G4double evt_Q2, evt_W2, evt_thcom, evt_beamp;
	 
private:
	//G4ParticleGun *fParticleGun;
	 G4GeneralParticleSource *fParticleSource;
	 
};

#endif
