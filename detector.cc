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
	G4ThreeVector hitposTrack = track->GetPosition();
	
	//G4cout << "track PID = " << PID << G4endl;
	//G4cout << "Track Vertex = " << vertexTrack << G4endl;
	//G4cout << "hit.pz = " << momentumTrack[2] << G4endl;
	
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	
	
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	if (PID == 11) {
		if (evt%1000 == 0){G4cout << evt << "th event generated" << G4endl;}
		man->FillNtupleDColumn(0, 0, PID);  //pid
		man->FillNtupleDColumn(0, 1, vertexTrack[0]);  //vx
		man->FillNtupleDColumn(0, 2, vertexTrack[1]); //vy
		man->FillNtupleDColumn(0, 3, vertexTrack[2]); //vz
		man->FillNtupleDColumn(0, 4, momentumTrack[0]); //px
		man->FillNtupleDColumn(0, 5, momentumTrack[1]); //py
  	man->FillNtupleDColumn(0, 6, momentumTrack[2]); //pz
  	man->FillNtupleDColumn(0, 7, evt);
  	man->AddNtupleRow(0);
	}
	
	if (PID == -211) {
		man->FillNtupleDColumn(1, 0, PID);  //pid
		man->FillNtupleDColumn(1, 1, vertexTrack[0]);  //vx
		man->FillNtupleDColumn(1, 2, vertexTrack[1]); //vy
		man->FillNtupleDColumn(1, 3, vertexTrack[2]); //vz
		man->FillNtupleDColumn(1, 4, momentumTrack[0]); //px
		man->FillNtupleDColumn(1, 5, momentumTrack[1]); //py
  	man->FillNtupleDColumn(1, 6, momentumTrack[2]); //pz
  	man->FillNtupleDColumn(1, 7, evt);
  	man->AddNtupleRow(1);
	}
	
	if (PID == 2212) {
		man->FillNtupleDColumn(2, 0, PID);  //pid
		man->FillNtupleDColumn(2, 1, vertexTrack[0]);  //vx
		man->FillNtupleDColumn(2, 2, vertexTrack[1]); //vy
		man->FillNtupleDColumn(2, 3, vertexTrack[2]); //vz
		man->FillNtupleDColumn(2, 4, momentumTrack[0]); //px
		man->FillNtupleDColumn(2, 5, momentumTrack[1]); //py
  	man->FillNtupleDColumn(2, 6, momentumTrack[2]); //pz
  	man->FillNtupleDColumn(2, 7, evt);
  	man->AddNtupleRow(2);
	}
	
		if (PID == 321) {
		man->FillNtupleDColumn(3, 0, PID);  //pid
		man->FillNtupleDColumn(3, 1, vertexTrack[0]);  //vx
		man->FillNtupleDColumn(3, 2, vertexTrack[1]); //vy
		man->FillNtupleDColumn(3, 3, vertexTrack[2]); //vz
		man->FillNtupleDColumn(3, 4, momentumTrack[0]); //px
		man->FillNtupleDColumn(3, 5, momentumTrack[1]); //py
  	man->FillNtupleDColumn(3, 6, momentumTrack[2]); //pz
  	man->FillNtupleDColumn(3, 7, evt);
  	man->AddNtupleRow(3);
	}
	
	
	
/*
	//if (PID == 3122) {
	man->FillNtupleIColumn(0, evt);
	man->FillNtupleIColumn(1, PID);
	man->FillNtupleDColumn(2, vertexTrack[0]);
	man->FillNtupleDColumn(3, vertexTrack[1]);
	man->FillNtupleDColumn(4, vertexTrack[2]);
	man->FillNtupleDColumn(5, momentumTrack[0]);
	man->FillNtupleDColumn(6, momentumTrack[1]);
	man->FillNtupleDColumn(7, momentumTrack[2]);
	man->FillNtupleDColumn(8, std::cos(hitposTrack.theta()));
	man->FillNtupleDColumn(9, hitposTrack.phi());
	man->AddNtupleRow(0);
	//}
*/
	
	/*
	G4Track *track = aStep->GetTrack();
	
	track->SetTrackStatus(fStopAndKill);
	
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
	
	G4ThreeVector posPhoton = preStepPoint->GetPosition();
	
	//G4cout << "Photon position: " << posPhoton << G4endl;
	
	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	G4int copyNo = touchable->GetCopyNumber();
	
	//G4cout << "Copy Number: " << copyNo << G4endl;
	
	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();
	
	G4cout << "Detector position: " << posDetector << G4endl;
	
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	
	G4AnalysisManager *man = G4AnalysisManager::Instance();	
	//G4AnalysisManager *man = new G4AnalysisManager::Instance();
	man->FillNtupleIColumn(0, evt);
	man->FillNtupleDColumn(1, posDetector[0]);
	man->FillNtupleDColumn(2, posDetector[1]);
	man->FillNtupleDColumn(3, posDetector[2]);
	man->AddNtupleRow(0);
	*/
	
	return true;	
}
