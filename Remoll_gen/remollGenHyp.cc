#include "remollGenHyp.hh"

#include "G4Material.hh"
#include "G4PhysicalConstants.hh"

#include "remollEvent.hh"
#include "remollVertex.hh"
#include "remolltypes.hh"

remollGenMoller::remollGenHyp()
  : remollVEventGen("Hyperon") {
  //fThCoM_min =    30.0*deg;
  //fThCoM_max =   150.0*deg;

  //fApplyMultScatt = true;
}

remollGenHyp::~remollGenHyp(){
}

G4LorentzVector MyPrimaryGenerator::GetHyperonMom(G4double PhotonE, G4ThreeVector HypDir)
{
	
	G4double Hypmom, HypE, W;
	G4double protonM = 0.938 * GeV;
	G4double kaonM = 0.494 * GeV;
	G4double HypM = 1.116 * GeV;
	
	
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


G4double HypPosRate(G4ThreeVector)
{

}
G4double HypPhotonERate(G4double)
{

}
G4double HypAngleRate(HypCMDir, G4double)
{

}


void remollGenHyp::SamplePhysics(remollVertex *vert, remollEvent *evt){

//void SetRate( G4double rate ){ fRate = rate;}

	bool IsPhotoproduction = true;

	if (IsPhotoproduction) {
		
		G4double PhotonE = (11.0-.915)*(G4UniformRand()) + .915;
		G4ThreeVector HyperonVertex(0.*cm, 0.*cm, (0. + 125*(G4UniformRand()-0.5)) *cm);
		
		G4ThreeVector HypCMDir;
		HypCMDir.setRThetaPhi(1., 3.1415*(G4UniformRand()), 2*3.1415*(G4UniformRand() - .5));
		
		G4LorentzVector vecHyperonLab = GetHyperonMom(PhotonE, HypCMDir);
		G4ThreeVector HypMom = vecHyperonLab.vect();
		
		G4double HypRate = HypPosRate(HyperonVertex)*HypPhotonERate(PhotonE)*HypAngleRate(HypCMDir, PhotonE);
		
	}

return;
}
