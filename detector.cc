#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
	
	G4Track *track = aStep->GetTrack();
	
	track->SetTrackStatus(fStopAndKill);
	
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
	
	G4int PID = track->GetParticleDefinition()->GetPDGEncoding();
	G4ThreeVector vertexTrack = track->GetVertexPosition();
	G4ThreeVector momentumTrack = track->GetMomentum();
	G4ThreeVector posDetHit = preStepPoint->GetPosition();
	
	//G4double thetaTrack = posDetHit.theta();
	//G4double costhetaTrack = posDetHit.cosTheta();
	//G4double phiTrack = posDetHit.phi();
	
	G4double thetaTrack = momentumTrack.theta();
	G4double costhetaTrack = momentumTrack.cosTheta();
	G4double phiTrack = momentumTrack.phi();
	
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	man->FillNtupleIColumn(0, evt);
	man->FillNtupleIColumn(1, PID);
	man->FillNtupleDColumn(2, vertexTrack[0]);
	man->FillNtupleDColumn(3, vertexTrack[1]);
	man->FillNtupleDColumn(4, vertexTrack[2]);
	man->FillNtupleDColumn(5, momentumTrack[0]);
	man->FillNtupleDColumn(6, momentumTrack[1]);
	man->FillNtupleDColumn(7, momentumTrack[2]);
	man->FillNtupleDColumn(8, thetaTrack);
	man->FillNtupleDColumn(9, costhetaTrack);
	man->FillNtupleDColumn(10, phiTrack);
	man->AddNtupleRow(0);
	
	return true;	
}
