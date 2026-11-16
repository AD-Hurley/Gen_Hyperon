#include "TROOT.h"
#include "TLorentzVector.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom.h"

Double_t Gamma(Double_t *x, Double_t *par){
	Double_t Q2 = x[0];
	Double_t W2 = par[0];
	Double_t beamE = par[1];
	Double_t W2_max = par[2]*par[2];
	Double_t proton_mass = 0.938; //To-Do: move to header
	Double_t recoilE = (W2_max - Q2 - W2)/(2*proton_mass);
	Doublt_t theta_e = 2*std::asin(std::sqrt(Q2/(4*beamE*recoilE)));
	
	
	Double_t epsilon = 1.0/(1.0 + 2*std::tan(theta_e/2.0)*std::tan(theta_e/2.0)*(recoilE*recoilE + beamE*beamE - 2*beamE*recoilE*std::cos(theta_e))/Q2);
	
	Double_t gamma = (1.0/137.0)/(2*std::pow(3.14159,2))*(recoilE/beamE)*((W2 - proton_mass*proton_mass)/(2*proton_mass))*(1.0/Q2)*(1.0/(1-epsilon));
	
	return gamma;
}

Double_t lambdaDecayLength(Double_t lambda_p){
	Double_t lambda_mass = 1.116; //GeV
	Double_t lambda_mlt = 2.62e-10; //seconds
	Double_t lambda_mdl = (lambda_p/lambda_mass)*lambda_mlt*TMath::C()*10e3; //in mm
	TF1 *fdecayLength = new TF1("fdecayLength", "[0]*exp(-1.0/[0]*x)", 0, 10*lambda_mdl);
	fdecayLength->SetParameter(0,lambda_mdl);
	
	return fdecayLength->GetRandom();
}

Double_t GetBeamEnergy(){
	//To-Do: update with energy distribution
	return 11.0;
}

Double_t genThetaP(Double_t beamPol){
	//To-Do: update with weak decay slope
	TRandom3 r;
	return TMath::Pi()*r.Rndm();
}

Double_t genThetaK(Double_t W2){
	//TRandom3 r;
	//return TMath::Pi()*r.Rndm();
	
	Double_t proton_mass = 0.938; //To-Do: move to header
	Double_t Egamma = (W2 - std::pow(proton_mass,2))/(2*proton_mass);
	
	Double_t par[5];
	
	if (Egamma < 1.135){
		par[0] = 0.56;
		par[1] = -1.8 + 2.1*Egamma;
		par[2] = 0;
		par[3] = 0;
		par[4] = 0;
	}
	else{
		par[0] = ((Egamma < 1.435)?1:0)*0.5315 + ((Egamma >= 1.435)?1:0)*(exp(2.433 - 2.243*Egamma));
		par[1] = ((Egamma < 1.9)?1:0)*(-0.7623 + 1.66*Egamma - 0.4914*Egamma*Egamma) + ((Egamma >=1.9)?1:0)*(exp(7.054-3.948*Egamma));
		par[2] = ((Egamma < 2.1)?1:0*(-3.581 + 4.489*Egamma - 1.137*Egamma*Egamma) + ((Egamma >= 2.1)?1:0)*exp(5.742 - 2.819*Egamma);
		par[3] = ((Egamma < 1.78)?1:0)*(2.582 - 3.366*Egamma + 0.9209*Egamma*Egamma) + ((Egamma >= 1.78)?1:0)*(0.778)*(1-exp(5.58 - 2.871*Egamma));
		par[4] = ((Egamma < 2.09)?1:0)*(0.8472 - 1.1185*Egamma + 0.299*Egamma*Egamma) + ((Egamma >= 2.09)?1:0)*(0.9943)*(exp(4.945 - 2.28*Egamma));
	}
	
	TF1 *fThetaK = new TF1("fThetaK","[0] + [1]*cos(x) + [2]*Power(cos(x),2) + [3]*Power(cos(x),3) + [4]*Power(cos(x),4)",-1,1,5);
	
	fThetaK->SetParameter(0,par[0]);
	fThetaK->SetParameter(1,par[1]);
	fThetaK->SetParameter(2,par[2]);
	fThetaK->SetParameter(3,par[3]);
	fThetaK->SetParameter(4,par[4]);
	
	Double_t ThetaK = fThetaK->GetRandom();
	return ThetaK;
}

void genHyperon(){

	//--------------------------------------Generator options----------------------------------
	// To-Do: make into arguement values
	TString HypName = "lambda";
	Double_t beamPol = 1;
	int numEvents = 100;
	int numThrownEvents = 0;
	Double_t maxPionTheta_Lab = 0.06; //radians
	
	
	//----------------------------------fixed generator variables------------------------------
	// To-Do: Move to a header file
	Double_t beamE = GetBeamEnergy();
	Double_t proton_mass = 0.938;
	Double_t kaon_mass = 0.494;
	Double_t pion_mass = 0.140;
	Double_t decayAlpha = 0.732;
	TVector 3 transPol(-0.2, 0.0, 0.1);
	
	Double_t hyperon_mass, W_min, W_max;
	
	TF1 *fW;

	if (HypName == "lambda"){
		hyperon_mass = 1.116;
		W_min = kaon_mass + hyperon_mass;
		W_max = sqrt(std::pow(proton_mass,2) + 2*proton_mass*beamE);
		fW = new TF1("fW","(gaus(0) + gaus(3) + pol2(6)) * ((x<1.8)?1:0) + (expo(9))*((x>=1.8 && x < 2.477)?1:0) + (expo(11))*((x>=2.477)?1:0)",W_min,W_max);
		fW->SetParameter(0,0.9500);
		fW->SetParameter(1,1.0699);
		fW->SetParameter(2,0.0800);
		fW->SetParameter(3,1.9994);
		fW->SetParameter(4,1.3502);
		fW->SetParameter(5,0.2442);
		fW->SetParameter(6,0.0027);
		fW->SetParameter(7,-1.0635);
		fW->SetParameter(8,0.8868);
		fW->SetParameter(9,2.1889);
		fW->SetParameter(10,-1.0522);
		fW->SetParameter(11,0.9315);
		fW->SetParameter(12,-0.5458);
		
	}
	else if (HypName == "sigma"){
		hyperon_mass = 1.193;
		W_min = kaon_mass + hyperon_mass;
		W_max = sqrt(std::pow(proton_mass,2) + 2*proton_mass*beamE);
		fW = new TF1("fW","(gaus(0) + pol2(3)) * ((x<1.6)?1:0)+(expo(6))*((x>=1.6 && x < 2.477)?1:0) + (expo(8))*((x>=2.477)?1:0)",W2_min,W2_max);
		fW->SetParameter(0,0.6828);
		fW->SetParameter(1,1.393);
		fW->SetParameter(2,0.0874);
		fW->SetParameter(3,-18.3100);
		fW->SetParameter(4,27.1200);
		fW->SetParameter(5,-9.1750);
		fW->SetParameter(6,2.1180);
		fW->SetParameter(7,-1.0390);
		fW->SetParameter(8,0.9828);
		fW->SetParameter(9,-0.5850);
	}
	else{
	
		cout << "ERROR: Invalid Hyperon Name" << endl;
		return;
	}
	
	 
	
	
	Double_t helMomentum = sqrt(std::pow((std::pow(hyperon_mass,2) + std::pow(proton_mass,2) - std::pow(pion_mass,2)),2) - 4*(std::pow(hyperon_mass,2))*(std::pow(proton_mass,2)))/(2*hyperon_mass);
	
	Double_t proton_helE = sqrt(std::pow(proton_mass,2) + std::pow(helMomentum,2));
	Double_t pion_helE = sqrt(std::pow(pion_mass,2) + std::pow(helMomentum,2));

	
	//-----------------------------------------------------------------------------------------
	//---------------------------------Event variables-----------------------------------------
	//-----------------------------------------------------------------------------------------
	TRandom3 r;
	
	//Kinematic variables
	Double_t recoilE, Q2_max, Q2, W2, epsilon, gamma, theta_e, phi_e, theta_K, phi_K, theta_p, phi_p;
	std::array<Double_t,2> K_angles; //To-Do: might not need To-Do it this way	
	
	//Four vectors
	TLorentzVector vecTarget_Lab, vecBeam_Lab, vecRecoilElectron_Lab, vecPhoton_Lab, vecCMFrame, vecHyperon_CM, vecHyperon_Lab, vecKaon_CM, vecKaon_Lab, vecRecoilProton_Hel, vecRecoilProton_CM, vecRecoilProton_Lab, vecPion_Hel, vecPion_CM, vecPion_Lab;
	
	TF1 *fDecay - new TF1("fDecay", "1 - [0]*[1]*x",-1,1);
	fDecay->SetParameter(0, decayAlpha);
	fDecay->SetParameter(1, transPol.Mag());
	
	TF1 *fGamma = new TF1("fGamma", Gamma, 0, 20, 3);

	//Event Loop		
	for(int i = 0; i < numEvents; i++){
		numThrownEvents++;
		
		//do-while caps pion lab phase space for efficiency		
		//do{
			//Throw Flat Angles
			phi_e = 2*TMath::Pi()*(r.Rndm() - 0.5);
			
			//define and sample from Q2 and W2 distribution
			W2 = std::pow(fW->GetRandom(),2);
			
			Q2_max = (std::pow(W_max,2) - W2)/(1.0 + 0.5*proton_mass/beamE);
			fGamma->SetRange(0,Q2_max);
			fGamma->SetParameter(0,W2);
			fGamma->SetParameter(1,beamE);
			fGamma->SetParameter(2,W_max);
			
			Q2 = fGamma->GetRandom();
			
			recoilE = (W_max*W_max - Q2 - W2)/(2*proton_mass);
			theta_e = 2*std::asin(std::sqrt(Q2/(4*beamE*recoilE)));
			
			//sample W-dependent Kaon angles
			
			//Set Helicity Frame 4-vectors
			phi_p = 2*TMath::Pi()*(r.Rndm() - 0.5);
			theta_p = fDecay->GetRandom();
			//----To-Do: rotate theta to helicity frame proper
			
			
			//boost Helicity 4-vectors to CM and set other CM 4-vectors
			phi_K = 2*TMath::Pi()*(r.Rndm() - 0.5);
			theta_K = genThetaK(W2);
			
			//boost CM 4-vectors to Lab and set other 4-vectors
			
			//write event to remoll format tree
			
		//}while(vecPion_Lab.Theta() > maxPionTheta_Lab);	
	}
	
	/*
	Loop through events and set proper rate based on number of events skipped in previous event loop
	for(int i = 0; i < numEvents; i++){
	}
	*/
}









































