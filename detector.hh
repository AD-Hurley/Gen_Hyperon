#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh" //if using 10.7 or newer
//#include "g4root.hh" //if using 10.6 or older
#include "G4RunManager.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
	MySensitiveDetector(G4String);
	~MySensitiveDetector();
	
private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);

};

#endif
