#ifndef __REMOLLGENHYP_HH 
#define __REMOLLGENHYP_HH

 include "remollVEventGen.hh"

class remollGenHyp : public remollVEventGen {
    public:
	remollGenHyp();
	virtual ~remollGenHyp();

	virtual G4LorentzVector GetHyperonMom(G4double, G4ThreeVector);	
	virtual G4double HypVertex();
	virtual G4double HypPhotonE();
	virtual G4double HypCMAngle(G4double);

    private:
	void SamplePhysics(remollVertex *, remollEvent *);
};

#endif//__REMOLLGENHYP_HH 
