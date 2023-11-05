#ifndef __REMOLLGENHYP_HH 
#define __REMOLLGENHYP_HH

 include "remollVEventGen.hh"

class remollGenHyp : public remollVEventGen {
    public:
	remollGenHyp();
	virtual ~remollGenHyp();

	virtual G4LorentzVector GetHyperonMom(G4double, G4ThreeVector);	
	virtual G4double HypPosRate(G4ThreeVector)
	virtual G4double HypPhotonERate(G4double)
	virtual G4double HypAngleRate(HypCMDir, G4double)

    private:
	void SamplePhysics(remollVertex *, remollEvent *);
};

#endif//__REMOLLGENHYP_HH 
