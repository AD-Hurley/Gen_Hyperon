#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	//fParticleGun = new G4ParticleGun(1);
	fParticleSource = new G4GeneralParticleSource();
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	//delete fParticleGun;
	delete fParticleSource;
}

bool testNoAngDist = false;
bool testFlatDist = false;
bool testHypRestFrame = false;
bool testCOMRestFrame = false;

G4double MyPrimaryGenerator::genPhiK(G4double Q2, G4double W2, G4double cosThetaK, G4double epsilon, G4double beamPol)
{
	//------------------------------------------------------------------------------------
	//This function samples CM frame phi_K based on the angular cross-section data 
	//------------------------------------------------------------------------------------
	
	G4double sig_U, sig_LT, sig_TT, sig_LTp;
	
	//determine Q2 index
	/*
	G4int Resp_QIndex = 0; //only one Q2 value in data set currently
	for(G4int i =0; i < 20; i++)
	{
			
	}
	*/
	
	//determine W index
	G4int Resp_WIndex = 0;
	G4double W = std::sqrt(W2);
	for(G4int i = 0; i < 20; i++)
	{
		if (W > Resp_WIndexKey[i])
			{
				Resp_WIndex++;
			} 	
	}
	
	//determine cos(theta) index
	G4int Resp_cosThetaIndex = 0;
	for(G4int i = 0; i < 10; i++)
	{
		if (W > Resp_cosThetaIndexKey[i])
			{
				Resp_cosThetaIndex++;
			} 	
	}
	
	sig_U = ResponseFnValues[Resp_WIndex][Resp_cosThetaIndex][0];
	sig_LT = ResponseFnValues[Resp_WIndex][Resp_cosThetaIndex][1];
	sig_TT = ResponseFnValues[Resp_WIndex][Resp_cosThetaIndex][2];
	sig_LTp = ResponseFnValues[Resp_WIndex][Resp_cosThetaIndex][3];
	
	//if(Q2 < 1.8)
	//{
		//do nothing right now. Interpolation of data incoming
	//}

	G4double phi_K, sigma, randAcceptor;
	bool isAcceptablePhi = false;
	
	if (testFlatDist == true){
		phi_K = 2*CLHEP::pi*(G4UniformRand() - .5);
	}
	else if (testNoAngDist == true){
		phi_K = 0;
	}
	else{
		while(!isAcceptablePhi)
		{
			phi_K = 2*CLHEP::pi*(G4UniformRand() - .5);
			
			randAcceptor = 1.5*(sig_U + epsilon*sig_TT + std::sqrt(2*epsilon*(1 + epsilon))*sig_LT + beamPol*std::sqrt(2*epsilon*(1 - epsilon))*sig_LTp)*G4UniformRand();
			sigma = sig_U + epsilon*sig_TT + std::sqrt(2*epsilon*(1 + epsilon))*sig_LT*std::cos(phi_K) + beamPol*std::sqrt(2*epsilon*(1 - epsilon))*sig_LTp*std::sin(phi_K);
			
			//G4cout << "Throwing Phi_K: " <<  randAcceptor - sigma << G4endl;
			
			if(randAcceptor < sigma)
			{
				isAcceptablePhi = true;
			}
		}
	}

	
	return phi_K;
}

G4double MyPrimaryGenerator::genThetaP(G4double beamPol)
{

	//------------------------------------------------------------------------------------
	// This function samples the theta_p of the proton in the hyperon rest frame
	//------------------------------------------------------------------------------------

	G4double helPol = -0.5*beamPol;
	G4double helAlpha = 0.75;
	G4double helTheta;
	G4double randAcceptor;
	bool isAcceptableTheta = false;
	
	if (testFlatDist == true){
		helTheta = G4UniformRand()*CLHEP::pi;
	}
	else if (testNoAngDist == true){
		helTheta = 0;
	}
	else{
		while(!isAcceptableTheta)
		{
			randAcceptor = G4UniformRand()*2;
			helTheta = G4UniformRand()*CLHEP::pi;
			
			//G4cout << "Throwing ThetaP: " << randAcceptor - (1-helAlpha*helPol*std::cos(helTheta)) << G4endl;
			
			if(randAcceptor < (1-helAlpha*helPol*std::cos(helTheta)))
			{
				//G4cout << "found a good ThetaP" << G4endl; 
				isAcceptableTheta = true;
			}
		}
	}
	
	
	
	return helTheta;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	CLHEP::HepRandom::setTheSeed((unsigned)clock());

	//Kinematic variables
	G4double beamPol, recoilE, Q2_max, Q2, W2, epsilon, gamma, theta_e, phi_e, theta_K, phi_K, theta_p, phi_p;
	
	G4int debugInt = 0;
	G4int debugInt_2 = 0;
	
	G4double gammaAcceptor;
	bool isAcceptableQW;
	
	//four vectors
	G4LorentzVector vecTarget_Lab, vecBeam_Lab, vecRecoilElectron_Lab, vecPhoton_Lab, vecCMFrame, vecHyperon_CM, vecHyperon_Lab, vecKaon_CM, vecKaon_Lab, vecRecoilProton_Hel, vecRecoilProton_CM, vecRecoilProton_Lab, vecPion_Hel, vecPion_CM, vecPion_Lab;
	
	//------------------------------------------Get kinematic variables------------------------------------------
	
	//G4cout << W2_min << " = (" << kaon_mass << " + " << hyperon_mass << ")^2" << G4endl;
	
	//throw beam energy (fixed for now)
	beamE = 11.0;
	
	//set beam polarization
	beamPol = -1.0;
	
	//Limit Phasespace for efficiency loop
	//do{
			
		//Throw angular distributions (except phi_K and theta_e)
		//theta_e = G4UniformRand()*CLHEP::pi;
		phi_e = 2*CLHEP::pi*(G4UniformRand() - .5);
		//if(phi_e < 0.00001){phi_e == 0.00001;}
		theta_K = G4UniformRand()*CLHEP::pi;
		phi_p = 2*CLHEP::pi*(G4UniformRand() - .5);
		theta_p = genThetaP(beamPol);
		
		//Throw W2, calculate max Q2, Throw Q2, and calculate E' and theta_e
		
		gammaAcceptor = G4UniformRand();
		
		debugInt = 0;
		
		isAcceptableQW = false;
		while(!isAcceptableQW){
			W2 = (W2_max - W2_min)*G4UniformRand() + W2_min;
			Q2_max = (W2_max - W2)/(1.0 + 0.5*proton_mass/beamE);
			Q2 = Q2_max*G4UniformRand();
				
			recoilE = (W2_max - Q2 - W2)/(2*proton_mass);
			theta_e = 2*std::asin(std::sqrt(Q2/(4*beamE*recoilE)));
			
			epsilon = 1.0/(1.0 + 2*std::tan(theta_e/2.0)*std::tan(theta_e/2.0)*(recoilE*recoilE + beamE*beamE - 2*beamE*recoilE*std::cos(theta_e))/Q2);
			gamma = (1.0/137.0)/(2*std::pow(3.14159,2))*(recoilE/beamE)*((W2 - proton_mass*proton_mass)/(2*proton_mass))*(1.0/Q2)*(1.0/(1-epsilon));
				
			/*
				debugInt++;
				if(debugInt > 10000){
					G4cout << "stuck?? theta_e = " << theta_e << ", recoilE = " << recoilE << ", epsilon = " << epsilon << ", G = " << gamma << G4endl; 
				}
				*/			
			if(gammaAcceptor < gamma){
				isAcceptableQW = true;
				//G4cout << "Good Q2 and W2" << G4endl;
			}	
		}
		
		/*
		W2 = (W2_max - W2_min)*G4UniformRand() + W2_min;
		do{
			W2 = W2_min + CLHEP::RandExponential::shoot(4);
			//G4cout << W2 << G4endl;
		}while(W2 > W2_max);
		Q2_max = (W2_max - W2)/(1.0 + 0.5*proton_mass/beamE);
		do{
			Q2 = CLHEP::RandExponential::shoot(4);
		}while(Q2 > Q2_max);
		//Q2 = G4UniformRand()*Q2_max;
		recoilE = (W2_max - Q2 - W2)/(2*proton_mass);
		theta_e = 2*std::asin(std::sqrt(Q2/(4*beamE*recoilE)));
	
		//G4cout << "W2 " << W2 << G4endl;
		//G4cout << "Q2_max " << Q2_max << G4endl;

		epsilon = 1.0/(1.0 + 2*std::tan(theta_e)*std::tan(theta_e)*(recoilE*recoilE + beamE*beamE - 2*beamE*recoilE*std::cos(theta_e))/Q2);
		gamma = (1.0/137.0)/(2*std::pow(3.14159,2))*(recoilE/beamE)*((W2 - proton_mass*proton_mass)/(2*proton_mass))*(1.0/Q2)*(1.0/(1-epsilon));	
		*/
			
		if (testNoAngDist == true){
			theta_e = 0;
			phi_e = 0;
			theta_K = 0;
			phi_p = 0;
		}
	
		if ((testNoAngDist == true) || (testFlatDist == true)){
			recoilE = 5.0;
		}
		
		//Sample phi_K
		phi_K = genPhiK(Q2, W2, std::cos(theta_K), epsilon, beamPol);
		
		//------------------------------------------Set 4-vectors and boost to lab frame------------------------------------------
		
		//Set Hyperon Helicity frame 4-vectors
		vecRecoilProton_Hel.setRThetaPhi(hel_Momentum,theta_p,phi_p);
		vecRecoilProton_Hel.setE(proton_helE);
		
		vecPion_Hel.setVect(-1.0*vecRecoilProton_Hel.vect());
		vecPion_Hel.setE(pion_helE);
		
		//Set CM Frame 4-vectors
		G4double CM_Momentum = std::sqrt(std::pow(W2 + std::pow(kaon_mass,2) - std::pow(hyperon_mass,2),2) - 4*W2*std::pow(kaon_mass,2))/(2*std::sqrt(W2));
		G4double kaonE_CM = std::sqrt(std::pow(kaon_mass,2) + std::pow(CM_Momentum,2));
		G4double hyperonE_CM = std::sqrt(std::pow(hyperon_mass,2) + std::pow(CM_Momentum,2));
		
		vecKaon_CM.setRThetaPhi(CM_Momentum, theta_K, phi_K);
		vecKaon_CM.setE(kaonE_CM);
		
		//G4cout << vecKaon_CM.x() << "," << vecKaon_CM.y() << "," << vecKaon_CM.z() << G4endl;
		
		vecHyperon_CM.setVect(-1.0*vecKaon_CM.vect());
		vecHyperon_CM.setE(hyperonE_CM);
		
		//G4cout << vecKaon_CM.x() << "," << vecKaon_CM.y() << "," << vecKaon_CM.z() << G4endl;
		
		//Set Lab Frame 4-vectors
		vecTarget_Lab.set(0,0,0,proton_mass);
		vecBeam_Lab.set(0,0,beamE,beamE);
		vecRecoilElectron_Lab.setRThetaPhi(recoilE,theta_e,phi_e);
		vecRecoilElectron_Lab.setE(recoilE);

		vecPhoton_Lab = vecBeam_Lab - vecRecoilElectron_Lab;

		//Boost CM Frame 4-vectors to Lab Frame
		vecCMFrame = vecTarget_Lab + vecPhoton_Lab;
		G4ThreeVector vecBoost_CMtoLab = vecCMFrame.boostVector();
		vecHyperon_Lab = vecHyperon_CM;
		vecKaon_Lab = vecKaon_CM;
		
		
		if ((testHypRestFrame == false) && (testCOMRestFrame == false)){
			vecHyperon_Lab.boost(vecBoost_CMtoLab);
			vecKaon_Lab.boost(vecBoost_CMtoLab);
		}
		
		
		if (testHypRestFrame == true){
			vecHyperon_Lab.set(0,0,0,hyperon_mass);
		}
		
		//G4cout << vecKaon_Lab.x() << "," << vecKaon_Lab.y() << "," << vecKaon_Lab.z() << G4endl;

		//Boost Hyperon Helicity Frame 4-vectors to Lab Frame
		G4ThreeVector vecBoost_HeltoCM = vecHyperon_CM.boostVector();
		vecRecoilProton_CM = vecRecoilProton_Hel;
		vecPion_CM = vecPion_Hel;

		if (testHypRestFrame == false){
			vecRecoilProton_CM = vecRecoilProton_CM.boost(vecBoost_HeltoCM);
			vecPion_CM = vecPion_CM.boost(vecBoost_HeltoCM);	
		}


		vecRecoilProton_Lab = vecRecoilProton_CM;
		vecPion_Lab = vecPion_CM;
		if ((testHypRestFrame == false) && (testCOMRestFrame == false)){
			vecPion_Lab = vecPion_Lab.boost(vecBoost_CMtoLab);
			vecRecoilProton_Lab = vecRecoilProton_Lab.boost(vecBoost_CMtoLab);
		}		
		/*
		if(debugInt_2 > 10000){
			G4cout << vecPion_Lab.theta() << G4endl;
		}
		debugInt_2++;
		*/
	//}while(vecPion_Lab.theta() > 0.06); //0.06 based on 1M sample of pol = -1 run through remoll


	//------------------------------------------Generate Event------------------------------------------
	
	G4PrimaryVertex *primVertex = new G4PrimaryVertex(0,0,0,0);
	
	//G4double beamE, beamPol, recoilE, recoilE_max, Q2, W2, W2_min, epsilon, theta_e, phi_e, theta_K, phi_K, theta_p, phi_p;
	
	/*
	G4cout << "Q2: " << Q2 << G4endl;
	G4cout << "W2_min: " << W2_min << G4endl;
	G4cout << "W2: " << W2 << G4endl;
	G4cout << "beamE: " << beamE << G4endl;
	G4cout << "RecoilE: " << recoilE << G4endl;
	G4cout << "recoilE_max: " << recoilE_max << G4endl;
	G4cout << "epsilon: " << epsilon << G4endl;
	G4cout << "theta_e: " << theta_e << G4endl;
	G4cout << "phi_e: " << phi_e << G4endl;
	G4cout << "theta_K: " << theta_K << G4endl;
	G4cout << "phi_K: " << phi_K << G4endl;
	G4cout << "theta_p: " << theta_p << G4endl;
	G4cout << "phi_p: " << phi_p << G4endl;
	*/
	//G4cout << vecHyperon_Lab.x()*GeV << "," <<  vecHyperon_Lab.y()*GeV << "," << vecHyperon_Lab.z()*GeV << G4endl;
	
	G4PrimaryParticle *primaryHyperon = new G4PrimaryParticle(hyperon,vecHyperon_Lab.x()*GeV,vecHyperon_Lab.y()*GeV,vecHyperon_Lab.z()*GeV);
	G4PrimaryParticle *primaryElectron = new G4PrimaryParticle(electron,vecRecoilElectron_Lab.x()*GeV,vecRecoilElectron_Lab.y()*GeV,vecRecoilElectron_Lab.z()*GeV);
	G4PrimaryParticle *primaryKaon = new G4PrimaryParticle(kaon,vecKaon_Lab.x()*GeV,vecKaon_Lab.y()*GeV,vecKaon_Lab.z()*GeV);
	G4PrimaryParticle *daughterProton = new G4PrimaryParticle(proton,vecRecoilProton_Lab.x()*GeV,vecRecoilProton_Lab.y()*GeV,vecRecoilProton_Lab.z()*GeV);
	G4PrimaryParticle *daughterPion = new G4PrimaryParticle(pion,vecPion_Lab.x()*GeV,vecPion_Lab.y()*GeV,vecPion_Lab.z()*GeV);
	
	primaryHyperon->SetDaughter(daughterProton);
	primaryHyperon->SetDaughter(daughterPion);
	
	primVertex->SetPrimary(primaryHyperon);
	
	if (testHypRestFrame == false){
		if (testCOMRestFrame == false){
			primVertex->SetPrimary(primaryElectron);
		}
		primVertex->SetPrimary(primaryKaon);
	}
	
	anEvent->AddPrimaryVertex(primVertex);
	
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->FillNtupleDColumn(4, 0, gamma);  //rate
	man->FillNtupleDColumn(4, 1, 1.0);  //A
	man->FillNtupleDColumn(4, 2, recoilE);  //Am
	man->FillNtupleDColumn(4, 3, epsilon);  //xs
	man->FillNtupleDColumn(4, 4, Q2);  //Q2
	man->FillNtupleDColumn(4, 5, W2);  //W2
	man->FillNtupleDColumn(4, 6, vecPion_Lab.theta());  //thcom
	man->FillNtupleDColumn(4, 7, beamE);  //beamp
	man->AddNtupleRow(4);
	
	//evt_Q2 = Q2;
	//evt_W2 = W2;
	//evt_thcom = theta_e;
	//evt_beamp = beamE;
}
	
