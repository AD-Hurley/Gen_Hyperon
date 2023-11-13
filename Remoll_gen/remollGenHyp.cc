#include "remollGenHyp.hh"

#include "G4Material.hh"
#include "G4PhysicalConstants.hh"

#include "remollEvent.hh"
#include "remollVertex.hh"
#include "remolltypes.hh"

remollGenHyp::remollGenHyp()
  : remollVEventGen("hyperon") {
  //fThCoM_min =    0.0*deg;
  //fThCoM_max =   180.0*deg;

  //fQ2 = 0;
  //fW2 = 1.7;
  fApplyMultScatt = false;
}

remollGenHyp::~remollGenHyp(){
}

G4LorentzVector remollGenHyp::GetHyperonMom(G4double PhotonE)
{
	G4double Hypmom, HypE, W;
	G4double protonM = 0.938 * GeV;
	G4double kaonM = 0.494 * GeV;
	G4double HypM = 1.116 * GeV;
	
	G4ThreeVector HypDir = HypCMAngle(PhotonE);
	//G4ThreeVector HypDir(0.,0.,1.); //move to args
	//HypDir.setRThetaPhi(1, 3.1415*G4UniformRand(), 2*3.1415*(G4UniformRand() - .5));
	//HypDir = HypDir*(1./HypDir.mag());
	//G4cout << "HypDir = (" << HypDir[0] << "," << HypDir[1] << "," << HypDir[2] << ")" << G4endl;
	
	//Get beta vector for boost. This is the boost that goes from Lab(Eg_x, Eg_y, Eg_z, Eg + Mp) to CM(0,0,0,Eg'+Ep')
	G4LorentzVector vecPhotonLab(0,0,PhotonE, PhotonE + protonM); //When Q2 != 0, move to input args
	G4ThreeVector CMBoost = vecPhotonLab.boostVector();
	
	W = std::sqrt(protonM*protonM + 2*protonM*PhotonE); //caclulate W in the lab frame
	//G4cout << "W = " << W << G4endl;
	
	Hypmom = std::sqrt((W*W + kaonM*kaonM - HypM*HypM)*(W*W + kaonM*kaonM - HypM*HypM) - 4*W*W*kaonM*kaonM)/(2*W); //calculate hyperon momentum in the CM frame
	HypE = std::sqrt(Hypmom*Hypmom + HypM*HypM); //calculate hyperon energy in the CM frame

	G4LorentzVector vecHypCM(HypDir*Hypmom, HypE); //make sure HypDir is correctly normalized
	G4LorentzVector vecHypLab = boostOf(vecHypCM, CMBoost);
	
	//G4cout << "CM P = " << Hypmom << G4endl;
	Hypmom = vecHypLab.rho();
	//G4cout << "Lab P = " << Hypmom << G4endl;

	return vecHypLab;
	//return vecHypCM;
}


G4ThreeVector remollGenHyp::HypVertex()
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

G4double remollGenHyp::HypPhotonE()
{
	G4double EbremSim, dsigdE, gamEnergy, gamEnergy_prob; 
	G4double EfnMax = 0.2518; //maximum value of the product of the imperical fits to normalized Egamma brem dist (simulated) and 1/sigma*dsigma/dE (fit to data) 
	
	
	bool generated = false;
	while (!generated)
	{
	
		gamEnergy = (11.0-.915)*(G4UniformRand()) + .915;
	
		EbremSim = (.142697 - 0.0188345*gamEnergy + 0.000754237*gamEnergy*gamEnergy + G4Exp(0.0225982-1.08665*gamEnergy)); //Photon energy spectrum from e- on lH2 geant4 sim
		dsigdE = (((.950*G4Exp(-.5*( ((gamEnergy-1.07)/.0800) * ((gamEnergy-1.07)/.0800) ) ) ) + (2.00*G4Exp(-.5*( ((gamEnergy-1.35)/.244) * ((gamEnergy-1.35)/.244) ) ) ) + (.00274 + -1.06*gamEnergy + .887*gamEnergy*gamEnergy) )*((gamEnergy<1.8)?1:0)+ (G4Exp(2.19-1.05*gamEnergy))*((gamEnergy>=1.8 && gamEnergy < 2.477)?1:0) + (G4Exp(.931-.546*gamEnergy))*((gamEnergy>=2.477)?1:0) ); //fit to dsigma/dE from CLAS 2006 data 
	
		gamEnergy_prob =  EbremSim * ((1.0/3.39847) * dsigdE); //product of the imperical fits to normalized Egamma brem dist (simulated) and 1/sigma*dsigma/dE (fit to data)
		
		if (1.2 * EfnMax * G4UniformRand() < gamEnergy_prob)
			generated = true;
	
	}
	
	//gamEnergy_prob = 3.*GeV;
	return gamEnergy * GeV;
}

G4ThreeVector remollGenHyp::HypCMAngle(G4double PhotonE)
{
	PhotonE = PhotonE/1000.; //convert to GeV
	//G4cout << "in GetHyperonCMDir, photonE = " << PhotonE << G4endl;

	G4ThreeVector CMDir(0., 0., 1.);
	//HypDir.setRThetaPhi(1, 3.1415*G4UniformRand(), 2*3.1415*(G4UniformRand() - .5));
	
	G4double p0, p1, p2, p3, p4;
	if (PhotonE <= 1.135)
	{
		p0 = 0.56;
		p1 = -1.8 + 2.1*PhotonE;
		p2 = 0;
		p3 = 0;
		p4 = 0;
	}
	else
	{
		p0 = 0.53 * ((PhotonE < 1.435)?1:0) + G4Exp(2.43 - 2.24*PhotonE) *  ((PhotonE >= 1.435)?1:0);
		p1 = (-0.76 + 1.66*PhotonE - 0.49*PhotonE*PhotonE)*((PhotonE < 1.9)?1:0) + (G4Exp(7.05 - 3.95*PhotonE))*((PhotonE >= 1.9)?1:0);
		p2 = (-3.58 + 4.49*PhotonE - 1.14*PhotonE*PhotonE)*((PhotonE < 2.1)?1:0) + (G4Exp(5.74 - 2.82*PhotonE))*((PhotonE >= 2.1)?1:0);
		p3 = (2.58 - 3.37*PhotonE + 0.92*PhotonE*PhotonE)*((PhotonE < 1.78)?1:0) + (0.78)*(1 - G4Exp(5.58 -2.87*PhotonE))*((PhotonE >= 1.78)?1:0);
		p4 = (0.85 - 1.19*PhotonE + 0.30*PhotonE*PhotonE)*((PhotonE < 2.09)?1:0) + (0.99)*(1 - G4Exp(4.95 -2.28*PhotonE))*((PhotonE >= 2.09)?1:0);
	}
	
	G4double cosThetaK, cosThetaK0, ThetaHyp;
	
	bool generated = false;
	while (!generated)
	{
	 	cosThetaK0 = 2*(G4UniformRand()-0.5);
	 	cosThetaK = p0 + p1*cosThetaK0 + p2*cosThetaK0*cosThetaK0 + p3*cosThetaK0*cosThetaK0*cosThetaK0 + p4*cosThetaK0*cosThetaK0*cosThetaK0*cosThetaK0;
	 	
	 	if ((2.0 * G4UniformRand()) < cosThetaK)
		{
			ThetaHyp = 3.1415 + std::acos(cosThetaK0);
			//G4cout << "ThetaHyp = " << ThetaHyp << G4endl;
			CMDir.setRThetaPhi(1., ThetaHyp, 2*3.1415*(G4UniformRand() - .5));
			generated = true;
		}
	}
	
	
	return CMDir;
}


void remollGenHyp::SamplePhysics(remollVertex *vert, remollEvent *evt){

//void SetRate( G4double rate ){ fRate = rate;}

	bool IsPhotoproduction = true;

	if (IsPhotoproduction) {
		
		//G4double TargetZOffset = -4500.0*mm;
		//G4ThreeVector TargetZOffset(0.0, 0.0, 0.0);
		
		G4double PhotonE = HypPhotonE();
		
		evt->fBeamE = 11.0;
		evt->fBeamMomentum = evt->fBeamMomentum.unit()*sqrt(11.0*11.0 - electron_mass_c2*electron_mass_c2);
		evt->SetQ2(0.0);
		evt->SetW2(0.938*0.938 + 2*0.938*PhotonE);
		evt->SetAsymmetry(0.0);
		evt->SetEffCrossSection(1.0);
	
		G4ThreeVector HyperonVertex = HypVertex();
		//G4ThreeVector CMDir = GetHyperonCMDir(PhotonE);
		G4LorentzVector vecHyperonLab = GetHyperonMom(PhotonE);
		//G4double HypLabP = vecHyperonLab.rho();
		G4ThreeVector HypLabP = vecHyperonLab.vect();
		
		//evt->fVertexPos = (0,0,0);
		//vert = 0;
		//evt->fVertexPos.setZ( -4500 );
		//evt->ProduceNewParticle(HyperonVertex, HypLabP, "proton");
		evt->ProduceNewParticle(G4ThreeVector(0.0,0.0,0.0), HypLabP, "lambda");  
		//evt->ProduceNewParticle(G4ThreeVector(0.0,0.0,0.0), G4ThreeVector(0.0,0.0,11.0), "proton");
		
		/*
		G4double PhotonE = (11.0-.915)*(G4UniformRand()) + .915;
		G4ThreeVector HyperonVertex(0.*cm, 0.*cm, (0. + 125*(G4UniformRand()-0.5)) *cm);
		
		G4ThreeVector HypCMDir;
		HypCMDir.setRThetaPhi(1., 3.1415*(G4UniformRand()), 2*3.1415*(G4UniformRand() - .5));
		
		G4LorentzVector vecHyperonLab = GetHyperonMom(PhotonE, HypCMDir);
		G4ThreeVector HypMom = vecHyperonLab.vect();
		
		G4double HypRate = HypPosRate(HyperonVertex)*HypPhotonERate(PhotonE)*HypAngleRate(HypCMDir, PhotonE);
		*/
	}

return;
}
