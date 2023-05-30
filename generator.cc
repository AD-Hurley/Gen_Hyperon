#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}


G4ThreeVector MyPrimaryGenerator::GetHyperonVertex()
{
	G4ThreeVector vertex(0., 0., 0.);

	G4double z;
	G4double z0;
	
	bool generated = false;
	while (!generated)
	{
		z0 = (0. + 125*(G4UniformRand()-0.5)) *cm;
		z = 0.5 + 0.0008*z0;
		if (1.1 * G4UniformRand() < z)
		{
			vertex[2] = z0;
			generated = true;
		}
	
	}
	
	
	return vertex;
}

G4double MyPrimaryGenerator::GetPhotonE()
{
	G4double EbremSim, dsigdE, gamEnergy, gamEnergy_prob; 
	G4double EfnMax = 0.2518; //maximum value of the product of the imperical fits to normalized Egamma brem dist (simulated) and 1/sigma*dsigma/dE (fit to data) 
	
	
	bool generated = false;
	while (!generated)
	{
	
		gamEnergy = (11.0-.915)*(G4UniformRand()) + .915;
		//G4cout << "gamEnergy " << gamEnergy << " GeV" << G4endl;
	
		EbremSim = (.142697 - 0.0188345*gamEnergy + 0.000754237*gamEnergy*gamEnergy + G4Exp(0.0225982-1.08665*gamEnergy)); //Photon energy spectrum from e- on lH2 geant4 sim
		dsigdE = (((.950*G4Exp(-.5*( ((gamEnergy-1.07)/.0800) * ((gamEnergy-1.07)/.0800) ) ) ) + (2.00*G4Exp(-.5*( ((gamEnergy-1.35)/.244) * ((gamEnergy-1.35)/.244) ) ) ) + (.00274 + -1.06*gamEnergy + .887*gamEnergy*gamEnergy) )*((gamEnergy<1.8)?1:0)+ (G4Exp(2.19-1.05*gamEnergy))*((gamEnergy>=1.8 && gamEnergy < 2.477)?1:0) + (G4Exp(.931-.546*gamEnergy))*((gamEnergy>=2.477)?1:0) ); //fit to dsigma/dE from CLAS 2006 data 
	
		gamEnergy_prob =  EbremSim * ((1.0/3.39847) * dsigdE); //product of the imperical fits to normalized Egamma brem dist (simulated) and 1/sigma*dsigma/dE (fit to data)
		
		
		//G4cout << "Egamma weight " << gamEnergy_prob << " GeV" << G4endl;
		if (1.2 * EfnMax * G4UniformRand() < gamEnergy_prob)
			generated = true;
	
	}
	
	//gamEnergy_prob = 3.*GeV;
	return gamEnergy * GeV;
}

G4ThreeVector MyPrimaryGenerator::GetHyperonAngle()
{
	G4ThreeVector Angle(0., 0., 1.);



	return Angle;
}

G4double MyPrimaryGenerator::GetHyperonMom()
{
	G4double Hypmom = 1.*GeV;



	return Hypmom;
}


void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName="lambda";
	G4ParticleDefinition *particle = particleTable->FindParticle("lambda");
	
	//--------------------------- Force the Lambda decay to pi-p only ---------------------------
	G4DecayTable *ppiOnlyTable = new G4DecayTable();
	G4PhaseSpaceDecayChannel *ppimDecayChannel = new G4PhaseSpaceDecayChannel(particleName, 1.0, 2, "proton", "pi-");
	ppiOnlyTable->Insert(ppimDecayChannel);
	particle->SetDecayTable(ppiOnlyTable);
	
	//--------------------------- Initialize particle gun ---------------------------------------
	//G4double z0 = (0. + 125*(G4UniformRand()-0.5))*cm;
	//G4double z0 = 0.*cm;
	//G4cout << "z0 = " << z0 << G4endl;
	
	//G4ThreeVector pos(0., 0., z0);
	//G4ThreeVector mom(0., 0., 1.);
	
	G4ThreeVector pos = GetHyperonVertex();
	G4ThreeVector ang = GetHyperonAngle();
	//G4double mom = GetHyperonMom();
	G4double mom = GetPhotonE();
	
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(ang);
	fParticleGun->SetParticleMomentum(mom);
	fParticleGun->SetParticleDefinition(particle);
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
