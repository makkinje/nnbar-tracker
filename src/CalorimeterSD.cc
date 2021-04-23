//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file CalorimeterSD.cc
/// \brief Implementation of the CalorimeterSD class

#include "CalorimeterSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "CalorHit.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4TrackVector.hh"
#include "G4VTouchable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4TouchableHistory.hh"
#include "G4SystemOfUnits.hh"
#include "G4SteppingManager.hh"
#include "G4StepPoint.hh"
#include "G4UserEventAction.hh"
#include "G4TransportationManager.hh"
#include "G4VSensitiveDetector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorimeterSD::CalorimeterSD(
                            const G4String& name, 
                            const G4String& hitsCollectionName,
                            G4int nofCells)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr),
   fNofCells(nofCells)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorimeterSD::~CalorimeterSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new CalorHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  auto hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  // fNofCells for cells + one more for total sums 
  for (G4int i=0; i<fNofCells+1; i++ ) {
    fHitsCollection->insert(new CalorHit());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool CalorimeterSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{ 

  // kinetic energy
  auto eKinPre = step -> GetPreStepPoint() -> GetKineticEnergy();
  auto eKinPost = step -> GetPostStepPoint() -> GetKineticEnergy();
//  auto eKinMean = (eKinPre + eKinPost)*0.5;
  auto deltaKE = eKinPre - eKinPost;
 
  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();
  
  // TrackID
  G4Track * theTrack = step->GetTrack();
  G4int trackID = theTrack -> GetTrackID();
 
  // Process
  G4int parentID = 0;
  G4String proc = "";

  // position
  auto pos = step -> GetPostStepPoint() -> GetPosition();

  // past position
  auto prePos = step->GetPreStepPoint() -> GetPosition();

  // momentum
  auto momentum = step -> GetPostStepPoint() -> GetMomentum();

  // past momentum
  auto preMomentum = step -> GetPreStepPoint() -> GetMomentum();

  // velocity
  auto velocity = step ->GetPostStepPoint() -> GetVelocity();

  // charge
  auto charge = step -> GetPostStepPoint() -> GetCharge();

  // name
  G4ParticleDefinition* particleType = step->GetTrack()->GetDefinition();
  G4String particleName = particleType->GetParticleName();

  if (trackID > 1)
  {
    parentID = theTrack->GetParentID();
    const G4VProcess* aProcess = theTrack->GetCreatorProcess();
    proc = aProcess->GetProcessName();
  }
  else
  {
    proc = "primary";
    parentID = 0;
  }
  
  if (proc == "Decay")
  {
    theTrack->SetTrackStatus(fKillTrackAndSecondaries);
  }
  
  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }
  
  if ( edep==0. && stepLength == 0. ) return false;      

  auto touchable = (step->GetPreStepPoint()->GetTouchable());
    
  // Get calorimeter cell id 
  auto layerNumber = touchable->GetReplicaNumber(1);
  
  // Get hit accounting data for this cell
  auto hit = (*fHitsCollection)[layerNumber];
  if ( ! hit ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber; 
    G4Exception("CalorimeterSD::ProcessHits()",
      "MyCode0004", FatalException, msg);
  }         
  
  // Get hit for total accounting
  auto hitTotal 
    = (*fHitsCollection)[fHitsCollection->entries()-1];
  
  // Add values
  hit->SetKinEn(eKinPost);
  hitTotal->SetKinEn(eKinPost); 
  
  hit->SetEDep(edep);
  hitTotal->SetEDep(edep);
  
  hit->SetTrackLength(stepLength);
  hitTotal->SetTrackLength(stepLength);

  hit->SetTrackID(trackID);
  hitTotal->SetTrackID(trackID);

  hit->SetProcess(proc);
  hitTotal->SetProcess(proc); 
  
  hit->SetPos(pos);
  hitTotal->SetPos(pos);

  hit->SetPrePos(prePos);
  hitTotal->SetPrePos(prePos);

  hit->SetMomentum(momentum);
  hitTotal->SetMomentum(momentum);

  hit->SetPreMomentum(preMomentum);
  hitTotal->SetPreMomentum(preMomentum); 

  hit->SetCharge(charge);
  hitTotal->SetCharge(charge);

  hit->SetParticle(particleName);
  hitTotal->SetParticle(particleName);

  hit->SetVelocity(velocity);
  hitTotal->SetVelocity(velocity);

  return true;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     auto nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits in the tracker chambers: " << G4endl;
     for ( std::size_t i=0; i<nofHits; ++i ) (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
