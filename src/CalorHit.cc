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
/// \file CalorHit.cc
/// \brief Implementation of the CalorHit class

#include "CalorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<CalorHit>* CalorHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorHit::CalorHit()
 : G4VHit()
//   fEdep(0.),
//   fTrackLength(0.),
//   fKinEn(0.)
{
   fEdep = 0;
   fKinEn = 0;
   fTrackLength = 0; 
   particleName = "";
   process = "";
   time = 0;
   fPrePos = G4ThreeVector(0., 0., 0.);
   fPos = G4ThreeVector(0., 0., 0.);
   fPreMomentum = G4ThreeVector(0., 0., 0.);
   fMomentum = G4ThreeVector(0., 0., 0.);
   fCharge = 0;
   fVelocity = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorHit::~CalorHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorHit::CalorHit(const CalorHit& right)
  : G4VHit()
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
  fKinEn = right.fKinEn;
  particleName = right.particleName;
  process = right.process;
  time = right.time;
  fPos = right.fPos;
  fPrePos = right.fPrePos;
  fPreMomentum = right.fPreMomentum;
  fMomentum = right.fMomentum;
  fCharge = right.fCharge;
  fVelocity = right.fVelocity;
  fEnergy = right.fEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const CalorHit& CalorHit::operator=(const CalorHit& right)
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
  fKinEn = right.fKinEn;
  particleName = right.particleName;
  process = right.process;
  time = right.time;
  fPos = right.fPos;
  fPrePos = right.fPrePos;
  fPreMomentum = right.fPreMomentum;
  fMomentum = right.fMomentum;
  fCharge = right.fCharge;
  fVelocity = right.fVelocity;
  fEnergy = right.fEnergy;
 
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool CalorHit::operator==(const CalorHit& right) const
{
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CalorHit::Print()
{
  G4cout
     << "Edep: " 
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " track length: " 
     << std::setw(7) << G4BestUnit( fTrackLength,"Length")
     << " kinetic energy: "
     << std::setw(7) << G4BestUnit( fKinEn, "Energy")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
