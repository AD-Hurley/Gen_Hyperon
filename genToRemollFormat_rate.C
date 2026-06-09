#include "TFile.h"
#include "TTree.h"
/*
// Event structure
struct remollEvent_t {
  double A, Am;
  double xs;
  double Q2, W2;
  double thcom;
  double beamp;
};

// Generic detector hit and sum structure
struct remollGenericDetectorHit_t {
  int det;
  int id;
  int trid;
  int pid;
  int gen;
  int mtrid;
  double t;
  double x, y, z;
  double xl, yl, zl;
  double r, ph;
  double px, py, pz;
  double pxl, pyl, pzl;
  double sx, sy, sz;
  double p, e, m, k, beta;
  double vx, vy, vz;
  double edep;
};
*/
vector<remollGenericDetectorHit_t>  *newhit=0;
Double_t newrate=0;
	
void genToRemollFormat_rate(const char *inFileName, const char *inRateName, const char *outFileName){
	
	Double_t photoCross, targetFactor, beame_rate, branchRatio, avoNum, targetLength, targetDensity, targetMolarMass, targetZ, SItoBarnes;
	
	//MOLLER liquid hydrogen target factor 
	avoNum = 6.022e23; //per mol
	targetLength = 1.25; //m
	targetDensity = 70.85; //kg per m^3
	targetMolarMass = 2.016e-3; //kg per mol
	targetZ = 2;
	SItoBarnes = 1.0e-28; // per m^2 to inverse barn
	targetFactor = avoNum*targetLength*targetDensity*targetZ/targetMolarMass*SItoBarnes; //inverse barns
	
	photoCross = 3.35e-6; //3.35 microbarns
	beame_rate = (6.5e-5)/(1.602e-19); //65 microamps / coulombs per electron = electrons/sec  
	branchRatio = 0.641; //lambda to proton+pi
	
	//------------------------------------------------------------------------------------------------------
	//Loop through uncut generated file to determine rate factor:
	TFile *inRateFile = new TFile(inRateName, "READ");
	TTree *inRate_evTree = (TTree*)inRateFile->Get("eventInfo");
	
	long nEntries_rate = inRate_evTree->GetEntries();
	
	Double_t fQ2_rate, fW2_rate, fepsilon_rate, fTheta_rate;
	inRate_evTree->SetBranchAddress("Q2", &fQ2_rate);
	inRate_evTree->SetBranchAddress("W2", &fW2_rate);
	inRate_evTree->SetBranchAddress("xs", &fepsilon_rate);
	inRate_evTree->SetBranchAddress("thcom", &fTheta_rate);
	
	TH2D *hQ2vW2_tCut = new TH2D("hEvent_Q2_W2_tCut","Q2 vs W2; Q^{2} ; W^{2}",200, 0, 20, 250, 0, 25);
	TH2D *hQ2vW2 = new TH2D("hEvent_Q2_W2","Q2 vs W2; Q^{2} ; W^{2}",200, 0, 20, 250, 0, 25);
	TH2D *hQ2vW2_cs = new TH2D("hEvent_Q2_W2_cs","Q2 vs W2; Q^{2} ; W^{2}",200, 0, 20, 250, 0, 25);
	
	for (long i = 0; i < nEntries_rate; i++){
		inRate_evTree->GetEntry(i);
		hQ2vW2->Fill(fQ2_rate,fW2_rate);
		if(fTheta_rate < 0.06){
			hQ2vW2_tCut->Fill(fQ2_rate,fW2_rate);
		}
	}
	
	for (long i = 0; i < nEntries_rate; i++){
		inRate_evTree->GetEntry(i);
		hQ2vW2_cs->Fill(fQ2_rate,fW2_rate,fepsilon_rate/hQ2vW2->GetBinContent(hQ2vW2->FindBin(fQ2_rate,fW2_rate)));
	}
	
	
	//Double_t CSweight = 4.6e09/hQ2vW2_cs->ProjectionX()->GetBinContent(1);
	Double_t CSweight = (photoCross*targetFactor*beame_rate*branchRatio)/hQ2vW2_cs->ProjectionX()->GetBinContent(1);
	//cout << photoCross*targetFactor*beame_rate*branchRatio << endl;
	//------------------------------------------------------------------------------------------------------
	
	Double_t zOffset = -5445;
	
	remollEvent_t *newev=0;
	//remollGenericDetectorHit_t *newhit=0;
	
	TFile *inFile = new TFile(inFileName, "READ");
	//inFile->Close();
	TFile *outFile = new TFile(outFileName, "RECREATE");
	
	TTree *in_evTree = (TTree*)inFile->Get("eventInfo");
	TTree *in_eTree = (TTree*)inFile->Get("electronHitInfo");
	TTree *in_pionTree = (TTree*)inFile->Get("pionHitInfo");
	TTree *in_protonTree = (TTree*)inFile->Get("protonHitInfo");
	TTree *in_kaonTree = (TTree*)inFile->Get("kaonHitInfo");
	
	TTree *outTree = new TTree("T","Hyperon Pions");	
	TBranch *b_rate = outTree->Branch("rate", &newrate);
	TBranch *b_ev = outTree->Branch("ev", &newev);
	TBranch *b_hit = outTree->Branch("hit", &newhit);
	
	long nEntries = in_evTree->GetEntries();
	long neEntries = in_eTree->GetEntries();
	long npiEntries = in_pionTree->GetEntries();
	long npEntries = in_protonTree->GetEntries();
	long nkEntries = in_kaonTree->GetEntries();
	
	//nEntries = 10000;
	
	cout << nEntries << endl;

	//vector<remollGenericDetectorHit_t>  *hit=0;
	remollGenericDetectorHit_t ehit, pihit, phit, khit;

	Double_t rate, fA, fAm, fxs, fQ2, fW2, fthcom, fbeamp, fdet; 
	Double_t epid, ex, ey, ez, epx, epy, epz, eevt; 
	Double_t pipid, pix, piy, piz, pipx, pipy, pipz, pievt; 
	Double_t ppid, px, py, pz, ppx, ppy, ppz, pevt; 
	Double_t kpid, kx, ky, kz, kpx, kpy, kpz, kevt;
	
	in_evTree->SetBranchAddress("rate", &rate);
	in_evTree->SetBranchAddress("A", &fA);
	in_evTree->SetBranchAddress("Am", &fAm);
	in_evTree->SetBranchAddress("xs", &fxs);
	in_evTree->SetBranchAddress("Q2", &fQ2);
	in_evTree->SetBranchAddress("W2", &fW2);
	in_evTree->SetBranchAddress("thcom", &fthcom);
	in_evTree->SetBranchAddress("beamp", &fbeamp);
	
	in_eTree->SetBranchAddress("hit.pid", &epid);
	in_eTree->SetBranchAddress("hit.x", &ex);
	in_eTree->SetBranchAddress("hit.y", &ey);
	in_eTree->SetBranchAddress("hit.z", &ez);
	in_eTree->SetBranchAddress("hit.px", &epx);
	in_eTree->SetBranchAddress("hit.py", &epy);
	in_eTree->SetBranchAddress("hit.pz", &epz);
	in_eTree->SetBranchAddress("evt", &eevt);

	in_pionTree->SetBranchAddress("hit.pid", &pipid);
	in_pionTree->SetBranchAddress("hit.x", &pix);
	in_pionTree->SetBranchAddress("hit.y", &piy);
	in_pionTree->SetBranchAddress("hit.z", &piz);
	in_pionTree->SetBranchAddress("hit.px", &pipx);
	in_pionTree->SetBranchAddress("hit.py", &pipy);
	in_pionTree->SetBranchAddress("hit.pz", &pipz);
	in_pionTree->SetBranchAddress("evt", &pievt);
	
	in_protonTree->SetBranchAddress("hit.pid", &ppid);
	in_protonTree->SetBranchAddress("hit.x", &px);
	in_protonTree->SetBranchAddress("hit.y", &py);
	in_protonTree->SetBranchAddress("hit.z", &pz);
	in_protonTree->SetBranchAddress("hit.px", &ppx);
	in_protonTree->SetBranchAddress("hit.py", &ppy);
	in_protonTree->SetBranchAddress("hit.pz", &ppz);
	in_protonTree->SetBranchAddress("evt", &pevt);
	
	in_kaonTree->SetBranchAddress("hit.pid", &kpid);
	in_kaonTree->SetBranchAddress("hit.x", &kx);
	in_kaonTree->SetBranchAddress("hit.y", &ky);
	in_kaonTree->SetBranchAddress("hit.z", &kz);
	in_kaonTree->SetBranchAddress("hit.px", &kpx);
	in_kaonTree->SetBranchAddress("hit.py", &kpy);
	in_kaonTree->SetBranchAddress("hit.pz", &kpz);
	in_kaonTree->SetBranchAddress("evt", &kevt);

	TH2D *hQ2vW2_cutFile = new TH2D("hEvent_Q2_W2_cutFile","Q2 vs W2; Q^{2} ; W^{2}",200, 0, 20, 250, 0, 25);
	//TH2D *hQ2vW2_cutFile_w = new TH2D("hEvent_Q2_W2_cutFile_w","Q2 vs W2; Q^{2} ; W^{2}",200, 0, 20, 250, 0, 25);
	Double_t scale_cut, binContent, tCutBinContent;
	
	fdet = 9999;
	//Double_t realRate;
	
	int i_e = 0;
	int i_k = 0;
	int i_p = 0;
	int i_pi = 0;

	in_eTree->GetEntry(i_e);
	in_pionTree->GetEntry(i_pi);
	in_protonTree->GetEntry(i_p);
	in_kaonTree->GetEntry(i_k);

	for (long i = 0; i < nEntries; i++){
		in_evTree->GetEntry(i);
		hQ2vW2_cutFile->Fill(fQ2,fW2);
	}

	for (long i = 0; i < nEntries; i++){
		in_evTree->GetEntry(i);
		//newrate = rate;
		newev->A = fA;
		newev->Am = fAm;
		newev->xs = fxs;
		newev->Q2 = fQ2;
		newev->W2 = fW2;
		newev->thcom = fthcom;
		newev->beamp = fbeamp;
		
		tCutBinContent = hQ2vW2_tCut->GetBinContent(hQ2vW2_tCut->FindBin(fQ2,fW2));
		if (tCutBinContent == 0) {tCutBinContent = 1;} //avoid killing low statistic bin entirely
		
		binContent = hQ2vW2->GetBinContent(hQ2vW2->FindBin(fQ2,fW2));
		if (binContent == 0) {binContent = 1;} //bin with 0 entries causes -nan. setting to 1 should be within stat errors anyways
		
		newrate = hQ2vW2_tCut->GetBinContent(hQ2vW2_tCut->FindBin(fQ2,fW2)) / hQ2vW2_cutFile->GetBinContent(hQ2vW2_cutFile->FindBin(fQ2,fW2));
		newrate = newrate*fA / binContent;
		newrate = newrate*CSweight;
		
		while (eevt == i && i_e < neEntries){
				ehit.det = fdet;
				ehit.pid = epid;
				ehit.x = ex;
				ehit.y = ey;
				ehit.z = ez + zOffset;
				ehit.px = epx;
				ehit.py = epy;
				ehit.pz = epz;
				newhit->push_back(ehit);
				i_e++;
				//cout << i_e << endl;
				in_eTree->GetEntry(i_e);
		}
		
		
		while (pievt == i && i_pi < npiEntries){
			pihit.det = fdet;
			pihit.pid = pipid;
			pihit.x = pix;
			pihit.y = piy;
			pihit.z = piz + zOffset;
			pihit.px = pipx;
			pihit.py = pipy;
			pihit.pz = pipz;
			newhit->push_back(pihit);
			i_pi++;
			in_pionTree->GetEntry(i_pi);
		}
		
		
		if (pevt == i && i_p < npEntries){
			phit.det = fdet;
			phit.pid = ppid;
			phit.x = px;
			phit.y = py;
			phit.z = pz + zOffset;
			phit.px = ppx;
			phit.py = ppy;
			phit.pz = ppz;
			newhit->push_back(phit);
			i_p++;
			in_protonTree->GetEntry(i_p);
		}
		
		
		if (kevt == i && i_k < nkEntries){
			khit.det = fdet;
			khit.pid = kpid;
			khit.x = kx;
			khit.y = ky;
			khit.z = kz + zOffset;
			khit.px = kpx;
			khit.py = kpy;
			khit.pz = kpz;
			newhit->push_back(khit);
			i_k++;
			in_kaonTree->GetEntry(i_k);
		}
		
		
		
		outTree->Fill();
		//newev->clear();
		newhit->clear();
		//newrate=0;
	}
	
	//
	outFile->cd();
	outTree->Write("",TObject::kOverwrite);
	outFile->Close();
	
	
	inFile->Close();
	inRateFile->Close();
	
}

