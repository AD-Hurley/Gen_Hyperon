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
	bool generated = false;
	
	G4double z;
	G4double z0;
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

G4double MyPrimaryGenerator::GetHyperonEg()
{
	G4double Egamma = 3.*Gev;
	G4double Egamma0; 
	
	Egamma0 = (11000.0-915.0)*(G4UniformRand()) + 915.0 *MeV;
	/*
	Egamma =  (.000142697 - 0.0000000188345*Egamma0 + .000000000000754237*Egamma0*Egamma0 -6.88516*G4Exp(-.00108665*Egamma0)) * (1.0/3.39847) * ((.950*G4Exp(-.5*( ((Egamma0-1.07)/.0800) * ((Egamma0-1.07)/.0800) ) ) ) + (2.00*G4Exp(-.5*( ((Egamma0-1.35)/.244) * ((Egamma0-1.35)/.244) ) ) ) + (.00274 + -1.06*Egamma0 + .887*Egamma0*Egamma0) + (2.19 * G4Exp(-1.05*Egamma0))*((x>=1.8 && x < 2.477)?1:0) + (.931 * G4Exp(-.546*Egamma0))*((x>=2.477)?1:0) );
	*/


	return Egamma;
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
	G4double mom = GetHyperonMom();
	
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(ang);
	fParticleGun->SetParticleMomentum(mom);
	fParticleGun->SetParticleDefinition(particle);
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
