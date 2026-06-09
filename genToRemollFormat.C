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
	
void genToRemollFormat(const char *inFileName, const char *outFileName){
	
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

	
	fdet = 9999;
	
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
		newrate = rate;
		newev->A = fA;
		newev->Am = fAm;
		newev->xs = fxs;
		newev->Q2 = fQ2;
		newev->W2 = fW2;
		newev->thcom = fthcom;
		newev->beamp = fbeamp;
		
		//cout << eevt << endl;
			
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
	
	inFile->Close();
	
	outTree->Write();
	outFile->Close();
	
}
