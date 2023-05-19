#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName="lambda";
	G4ParticleDefinition *particle = particleTable->FindParticle("lambda");
	
	/*
	G4cout << "Old Decay Table is:" << G4endl;
	G4DecayTable *lambdaDecayTable = particle->GetDecayTable();
	lambdaDecayTable->DumpInfo();
	*/
	
	G4DecayTable *ppiOnlyTable = new G4DecayTable();
	G4PhaseSpaceDecayChannel *ppimDecayChannel = new G4PhaseSpaceDecayChannel(particleName, 1.0, 2, "proton", "pi-");
	ppiOnlyTable->Insert(ppimDecayChannel);
	particle->SetDecayTable(ppiOnlyTable);
	
	/*
	G4cout << "New Decay Table is:" << G4endl;
	lambdaDecayTable = particle->GetDecayTable();
	lambdaDecayTable->DumpInfo();
	*/
	
	G4double z0 = (0. + 125*(G4UniformRand()-0.5))*cm;
	//z0 = 125./2.*cm;
	G4cout << "z0 = " << z0 << G4endl;
	
	G4ThreeVector pos(0., 0., z0);
	G4ThreeVector mom(0., 0., 1.);
	
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(1.*GeV);
	fParticleGun->SetParticleDefinition(particle);
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
