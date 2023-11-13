#ifndef __REMOLLGENHYP_HH 
#define __REMOLLGENHYP_HH

#include "remollVEventGen.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Exp.hh"
#include "G4LorentzVector.hh"

class remollGenHyp : public remollVEventGen {
    public:
	remollGenHyp();
	virtual ~remollGenHyp();

	virtual G4LorentzVector GetHyperonMom(G4double);	
	virtual G4ThreeVector HypVertex();
	virtual G4double HypPhotonE();
	virtual G4ThreeVector HypCMAngle(G4double);

    private:
	void SamplePhysics(remollVertex *, remollEvent *);
};

#endif//__REMOLLGENHYP_HH 
