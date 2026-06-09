#include "run.hh"

MyRunAction::MyRunAction()
{}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->OpenFile("output.root");
	
	man->CreateNtuple("electronHitInfo","electronHitInfo");

	man->CreateNtupleDColumn("hit.pid");
	man->CreateNtupleDColumn("hit.x");
	man->CreateNtupleDColumn("hit.y");
	man->CreateNtupleDColumn("hit.z");
	man->CreateNtupleDColumn("hit.px");
	man->CreateNtupleDColumn("hit.py");
	man->CreateNtupleDColumn("hit.pz");
	man->CreateNtupleDColumn("evt");
	man->FinishNtuple(0);
	
	man->CreateNtuple("pionHitInfo","pionHitInfo");

	man->CreateNtupleDColumn("hit.pid");
	man->CreateNtupleDColumn("hit.x");
	man->CreateNtupleDColumn("hit.y");
	man->CreateNtupleDColumn("hit.z");
	man->CreateNtupleDColumn("hit.px");
	man->CreateNtupleDColumn("hit.py");
	man->CreateNtupleDColumn("hit.pz");
	man->CreateNtupleDColumn("evt");
	man->FinishNtuple(1);
	
	man->CreateNtuple("protonHitInfo","protonHitInfo");

	man->CreateNtupleDColumn("hit.pid");
	man->CreateNtupleDColumn("hit.x");
	man->CreateNtupleDColumn("hit.y");
	man->CreateNtupleDColumn("hit.z");
	man->CreateNtupleDColumn("hit.px");
	man->CreateNtupleDColumn("hit.py");
	man->CreateNtupleDColumn("hit.pz");
	man->CreateNtupleDColumn("evt");
	man->FinishNtuple(2);
	
	man->CreateNtuple("kaonHitInfo","kaonHitInfo");

	man->CreateNtupleDColumn("hit.pid");
	man->CreateNtupleDColumn("hit.x");
	man->CreateNtupleDColumn("hit.y");
	man->CreateNtupleDColumn("hit.z");
	man->CreateNtupleDColumn("hit.px");
	man->CreateNtupleDColumn("hit.py");
	man->CreateNtupleDColumn("hit.pz");
	man->CreateNtupleDColumn("evt");
	man->FinishNtuple(3);
	
	
	
	man->CreateNtuple("eventInfo","eventInfo");
	
	man->CreateNtupleDColumn("rate");
	man->CreateNtupleDColumn("A");
	man->CreateNtupleDColumn("Am");
	man->CreateNtupleDColumn("xs");
	man->CreateNtupleDColumn("Q2");
	man->CreateNtupleDColumn("W2");
	man->CreateNtupleDColumn("thcom");
	man->CreateNtupleDColumn("beamp");
	man->FinishNtuple(4);
	
	/*
	man->CreateNtuple("Hits","Hits");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleIColumn("PID");
	man->CreateNtupleDColumn("vx");
	man->CreateNtupleDColumn("vy");
	man->CreateNtupleDColumn("vz");
	man->CreateNtupleDColumn("px");
	man->CreateNtupleDColumn("py");
	man->CreateNtupleDColumn("pz");
	man->CreateNtupleDColumn("cosTheta");
	man->CreateNtupleDColumn("phi");
	man->FinishNtuple(0);
	*/
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->Write();
	man->CloseFile("output.root");

}
