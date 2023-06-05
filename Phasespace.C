#include <iostream>
#include "TF1.h"
#include "TROOT.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TRandom.h"

void Phasespace(){
  
  // This macro is intended to study the pi- theta and momentum distribution in Hyperon photoproduction using phasespace only
  TFile *outputfile = new TFile("HyperonPhasespacePlots.root","RECREATE");

  bool isVerbose = true;

  TRandom3 * ran = new TRandom3();
  ran->SetSeed(0); //0 = computer clock

  
  //Initialize output histograms
  TH2D *hPion_ThetaVP_lab = new TH2D("Pi_Theta_P_lab","#pi^{-} #theta_{lab} vs |p|; #pi^{-} |p_{lab}| (GeV); #theta_{lab} (rad)", 200, 0, 4.0, 200, 0, TMath::Pi());
  TH2D *hPion_ThetaVP_CM = new TH2D("Pi_Theta_P_CM","#pi^{-} #theta_{CM} vs |p|; #pi^{-} |p_{CM}| (GeV); #theta_{CM} (rad)", 200, 0, 4.0, 200, 0, TMath::Pi());
  TH2D *hPion_ThetaVP_Y = new TH2D("Pi_Theta_P_Y","#pi^{-} #theta_{Y} vs |p|; #pi^{-} |p_{Y}| (GeV); #theta_{Y} (rad)", 200, 0, 4.0, 200, 0, TMath::Pi());

  /*
TH2D *hPion_ThetaVP_lab = new TH2D("Pi_Theta_P_lab","#pi^{-} #theta_{lab} vs |p|; #pi^{-} |p_{lab}| (GeV); #theta_{lab} (rad)", 200, 0, 4.0, 200, 0, TMath::Pi());
TH2D *hPion_ThetaVP_lab = new TH2D("Pi_Theta_P_lab","#pi^{-} #theta_{lab} vs |p|; #pi^{-} |p_{lab}| (GeV); #theta_{lab} (rad)", 200, 0, 4.0, 200, 0, TMath::Pi());
TH2D *hPion_ThetaVP_lab = new TH2D("Pi_Theta_P_lab","#pi^{-} #theta_{lab} vs |p|; #pi^{-} |p_{lab}| (GeV); #theta_{lab} (rad)", 200, 0, 4.0, 200, 0, TMath::Pi());
TH2D *hPion_ThetaVP_lab = new TH2D("Pi_Theta_P_lab","#pi^{-} #theta_{lab} vs |p|; #pi^{-} |p_{lab}| (GeV); #theta_{lab} (rad)", 200, 0, 4.0, 200, 0, TMath::Pi());
TH2D *hPion_WvP_lab = new TH2D("Pi_W_P_lab","#pi^{-} W vs |p|; #pi^{-} |p_{lab}| (GeV); W", 200, 0, 4.0, 200, 1.62, 4.62);*/

  //Kinematics
  Double_t BeamE = 11.0;
  Double_t GammaE;
  Double_t M_Y;
  Double_t M_K = 0.494;
  Double_t M_Proton = 0.938;
  Double_t M_pi = 0.140;

  for(int i = 0; i < 1; i++){ 

    Double_t electronE = 0;
    Double_t Q2 = 0; //0 for photoproduction. Change later for electroproduction
    /*
      Double_t electronE = ran->Uniform(0,11.0);
      Double_t Theta_lab = ran->Uniform(0,TMath::Pi()); //flat theta CM
      Q2 = 4*BeamE*electronE*std::pow(TMath::Sin(Theta_lab),2);
    */
    //Double_t W = 2.15217; //in GeV. for Q2 = 0 (photoproduction) with Egamma = 11.0 GeV. Change later for electroproduction
    //Double_t W = ran->Uniform(3.0,4.62);
    //GammaE = BeamE - electronE;
    GammaE = 4.0;
    
    Double_t W = TMath::Sqrt( std::pow(0.938,2) - Q2 + 2*0.938*(GammaE));
    cout << "W = " << W << endl;
    
    M_Y = 1.11; //Mass of the Hyperon, lambda(1116) in this case. Add switch for sigma0 later.
    
    //--------------------------------------------------------------------------------------------------
    //----------------------------------Calculate Energy Momentum in Rest Frames------------------------
    //--------------------------------------------------------------------------------------------------
    
    //CM frame
    Double_t E_Y = (W*W - M_K*M_K + M_Y*M_Y)/(2.0*W); //0.25 ~ kaon mass squared
    Double_t P_Y = TMath::Sqrt(E_Y*E_Y - M_Y*M_Y);
    
    Double_t E_K = (W*W + M_K*M_K - M_Y*M_Y)/(2.0*W);
    
    //Hyperon Helicity Frame
    Double_t E_pi = (M_Y*M_Y + M_pi*M_pi - M_Proton*M_Proton)/(2.0*M_Y); //0.88 ~ proton mass squared
    Double_t P_pi = TMath::Sqrt(E_pi*E_pi - M_pi*M_pi);
    
    Double_t E_Proton = (M_Y*M_Y - M_pi*M_pi + M_Proton*M_Proton)/(2.0*M_Y);
    
    
    //--------------------------------------------------------------------------------------------------
    //-------------------Generate rest frame 4-vectors at random angles (flat, no physics)--------------
    //--------------------------------------------------------------------------------------------------
    
    Double_t Theta_CM, Phi_CM, Theta_K, Phi_K, Theta_Y, Phi_Y, Theta_pi, Phi_pi;
    
    Theta_CM = 0; //0 for photoproduction test case
    Phi_CM = 0;
    //Theta_CM = rand(0,TMath::Pi());
    Theta_K = 0; //inverse Y angle, but usually the angles of K are measured - included for including measured distributions later
    Phi_K = 0; //inverse Y angle, but usually the angles of K are measured - included for including measured distributions later
    
    //Theta_Y = ran->Uniform(0.0,TMath::Pi()); //= TMath::Pi() - Theta_K
    //Phi_Y = ran->Uniform(0.0,2*TMath::Pi()); //= TMath::Pi() + Phi_K
    //Theta_pi = ran->Uniform(0.0,TMath::Pi());
    //Phi_pi = ran->Uniform(0.0,2*TMath::Pi());
    
    // SANITY CHECK
    Theta_Y = 0; //= TMath::Pi() - Theta_K
    Phi_Y = 0; //= TMath::Pi() + Phi_K
    Theta_pi = 0;
    Phi_pi = 0;
    
    //Photon plus target proton in lab frame
    //TLorentzVector Lab_P4(0,0,GammaE,GammaE + M_Proton);
    
    TLorentzVector Lab_P4(GammaE*TMath::Sin((-1)*Theta_CM)*TMath::Cos(Phi_CM + TMath::Pi()),
			  GammaE*TMath::Sin((-1)*Theta_CM)*TMath::Sin(Phi_CM + TMath::Pi()),
			  GammaE*TMath::Cos(Theta_CM),
			  GammaE + M_Proton);
    

    //Defined in the CM frame
    TLorentzVector Y_P4( P_Y*TMath::Sin(Theta_Y)*TMath::Cos(Phi_Y),
			 P_Y*TMath::Sin(Theta_Y)*TMath::Sin(Phi_Y),
			 P_Y*TMath::Cos(Theta_Y),
			 E_Y);
    
    TLorentzVector K_P4(-Y_P4(0),-Y_P4(1),-Y_P4(2), E_K);
    
    //Defined in the Hyperion rest  frame
    TLorentzVector Pi_P4( P_pi*TMath::Sin(Theta_pi)*TMath::Cos(Phi_pi),
			  P_pi*TMath::Sin(Theta_pi)*TMath::Sin(Phi_pi),
			  P_pi*TMath::Cos(Theta_pi),
			  E_pi);
    
    TLorentzVector Proton_P4(-Pi_P4(0),-Pi_P4(1),-Pi_P4(2), E_Proton);
    
    TVector3 YRestToCM_BoostVector = Y_P4.BoostVector();
    TVector3 CMToLab_BoostVector = Lab_P4.BoostVector();
    
    TLorentzVector Pi_CM = Pi_P4;
    Pi_CM.Boost(YRestToCM_BoostVector);
    TLorentzVector Pi_lab = Pi_CM;
    Pi_lab.Boost(CMToLab_BoostVector);
    
    TLorentzVector Proton_CM = Proton_P4;
    Proton_CM.Boost(YRestToCM_BoostVector);
    TLorentzVector Proton_lab = Proton_CM;
    Proton_lab.Boost(CMToLab_BoostVector);
    
    TLorentzVector Y_lab = Y_P4;
    Y_lab.Boost(CMToLab_BoostVector);
    
    TLorentzVector K_lab = K_P4;
    K_lab.Boost(CMToLab_BoostVector);
    
    if (isVerbose){
      cout << "-------------------Hyperon Rest Frame-------------------------" << endl;
      
      cout << "Pi_P4(Px,Py,Pz,E) = (" << Pi_P4(0) << "," << Pi_P4(1) << "," << Pi_P4(2) << "," << Pi_P4(3) << ")" << endl; 
      cout << "Proton_P4(Px,Py,Pz,E) = (" << Proton_P4(0) << "," << Proton_P4(1) << "," << Proton_P4(2) << "," << Proton_P4(3) << ")" << endl; 
      
      cout << endl << "-------------------------CM Frame-----------------------------" << endl;
      
      cout << "Pi_CM(Px,Py,Pz,E) = (" << Pi_CM(0) << "," << Pi_CM(1) << "," << Pi_CM(2) << "," << Pi_CM(3) << ")" << endl;
      cout << "Proton_CM(Px,Py,Pz,E) = (" << Proton_CM(0) << "," << Proton_CM(1) << "," << Proton_CM(2) << "," << Proton_CM(3) << ")" << endl;
      cout << "Y_P4(Px,Py,Pz,E) = (" << Y_P4(0) << "," << Y_P4(1) << "," << Y_P4(2) << "," << Y_P4(3) << ")" << endl;
      cout << "K_P4(Px,Py,Pz,E) = (" << K_P4(0) << "," << K_P4(1) << "," << K_P4(2) << "," << K_P4(3) << ")" << endl;
    
      cout << endl << "------------------------ lab Frame ---------------------------" << endl;
      
      cout << "CM to lab boost vector = (" << CMToLab_BoostVector(0) << "," << CMToLab_BoostVector(1) << "," << CMToLab_BoostVector(2) << ")" << endl;
      
      cout << "Pi_lab(Px,Py,Pz,E) = (" << Pi_lab(0) << "," << Pi_lab(1) << "," << Pi_lab(2) << "," << Pi_lab(3) << ")" << endl;
      cout << "Proton_lab(Px,Py,Pz,E) = (" << Proton_lab(0) << "," << Proton_lab(1) << "," << Proton_lab(2) << "," << Proton_lab(3) << ")" << endl;
      cout << "Y_lab(Px,Py,Pz,E) = (" << Y_lab(0) << "," << Y_lab(1) << "," << Y_lab(2) << "," << Y_lab(3) << ")" << endl;
      cout << "K_lab(Px,Py,Pz,E) = (" << K_lab(0) << "," << K_lab(1) << "," << K_lab(2) << "," << K_lab(3) << ")" << endl;  
    }
    
    
    hPion_ThetaVP_lab->Fill( (Pi_lab.Vect()).Mag(), (Pi_lab.Vect()).Theta() );
    hPion_ThetaVP_CM->Fill( (Pi_CM.Vect()).Mag(), (Pi_CM.Vect()).Theta() );
    hPion_ThetaVP_Y->Fill( (Pi_P4.Vect()).Mag(), (Pi_P4.Vect()).Theta() );
    //hPion_WvP_lab->Fill( (Pi_lab.Vect()).Mag(), W );
  }
  
  hPion_ThetaVP_lab->Write();
  hPion_ThetaVP_CM->Write();
  hPion_ThetaVP_Y->Write();
  //hPion_WvP_lab->Write();

  outputfile->Close();

}

