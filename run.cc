#include "run.hh"

MyRunAction::MyRunAction()
{}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->OpenFile("output.root");
	
	man->CreateNtuple("Hits","Hits");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleIColumn("PID");
	man->CreateNtupleDColumn("vx");
	man->CreateNtupleDColumn("vy");
	man->CreateNtupleDColumn("vz");
	man->CreateNtupleDColumn("px");
	man->CreateNtupleDColumn("py");
	man->CreateNtupleDColumn("pz");
	man->CreateNtupleDColumn("theta");
	man->CreateNtupleDColumn("costheta");
	man->CreateNtupleDColumn("phi");
	man->CreateNtupleDColumn("Egamma");
	man->FinishNtuple(0);
	
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->Write();
	man->CloseFile("output.root");

}
