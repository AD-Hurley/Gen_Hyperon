#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh" //if using 10.7 or newer
//#include "g4root.hh" //if using 10.6 or older

class MyRunAction : public G4UserRunAction
{
public:
	MyRunAction();
	~MyRunAction();
	
	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);
};

#endif
