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
/// \file CalorHit.hh
/// \brief Definition of the CalorHit class

#ifndef CalorHit_h
#define CalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"
#include "G4Track.hh"

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class CalorHit : public G4VHit
{
  public:
    CalorHit();
    CalorHit(const CalorHit&);
    virtual ~CalorHit();

    // operators
    const CalorHit& operator=(const CalorHit&);
    G4bool operator==(const CalorHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void SetPos(G4ThreeVector xyz) {fPos = xyz;}; 
    void SetParticle(G4String name) {particleName = name;};
    void SetTrackID(G4int trackNum) {trackID = trackNum;}; 
    void SetProcess(G4String processName) {process = processName;};
    void SetEDep(G4double eDep) {fEdep = eDep;};
    void SetTrackLength(G4double trackLength) {fTrackLength = trackLength;};
//    void Add(G4double dk);
//    void Add(G4double de, G4double dl, G4double dk);
    void SetTime(const G4double t) {time = t;};
    void SetMomentum(G4ThreeVector abc) {fMomentum = abc;};
    void SetKinEn(G4double kinEn) {fKinEn = kinEn;};
    void SetPrePos(G4ThreeVector prePos) {fPrePos = prePos;};
    void SetPreMomentum(G4ThreeVector preMom) {fPreMomentum = preMom;};
    void SetCharge(G4double charge) {fCharge = charge;};
    void SetVelocity(G4double velocity) {fVelocity = velocity;};
    void SetEnergy(G4double energy) {fEnergy = energy;};

//    G4double GetEdep() const;
//    G4double GetTrackLength() const;
    G4double GetEdep() {return fEdep;};
    G4double GetTrackLength() {return fTrackLength;};
    G4double GetKinEn() {return fKinEn;};
    G4ThreeVector GetPos() const {return fPos;};
    G4String GetParticle() {return particleName;};
    G4int GetTrackID() {return trackID;};
    G4String GetProcess() {return process;};
    G4double GetTime() {return time;};
    G4ThreeVector GetMomentum() const {return fMomentum;};
    G4ThreeVector GetPrePos() const {return fPrePos;};
    G4ThreeVector GetPreMomentum() const {return fPreMomentum;};
    G4double GetCharge() const {return fCharge;};
    G4double GetVelocity() const {return fVelocity;};
    G4double GetEnergy() {return fEnergy;};

  private:
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4double fTrackLength; ///< Track length in the  sensitive volume
    G4double fKinEn; /// << Kinetic energy in the sensitive volume
    G4ThreeVector fPos;
    G4ThreeVector fPrePos;
    G4String particleName;
    G4int trackID;
    G4String process;
    G4double time;
    G4ThreeVector fMomentum;
    G4ThreeVector fPreMomentum;
    G4double fCharge;
    G4double fVelocity;
    G4double fEnergy;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using CalorHitsCollection = G4THitsCollection<CalorHit>;
//typedef G4THitsCollection<CalorHit> CalorHitsCollection;

extern G4ThreadLocal G4Allocator<CalorHit>* CalorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//typedef G4THitsCollection<CalorHit> CalorHitsCollection;
//extern G4ThreadLocal G4Allocator<CalorHit>* CalorHitsCollection;

inline void* CalorHit::operator new(size_t)
{
  if (!CalorHitAllocator) {
    CalorHitAllocator = new G4Allocator<CalorHit>;
  }
  void *hit;
  hit = (void *) CalorHitAllocator->MallocSingle();
  return hit;
}

inline void CalorHit::operator delete(void *hit)
{
  if (!CalorHitAllocator) {
    CalorHitAllocator = new G4Allocator<CalorHit>;
  }
  CalorHitAllocator->FreeSingle((CalorHit*) hit);
}
/*
inline void CalorHit::Add(G4double dk)
{ 
  fKinEn += dk;
}

inline void CalorHit::Add(G4double de, G4double dl, G4double dk) {
  fEdep += de; 
  fTrackLength += dl;
  fKinEn += dk;
}

inline G4double CalorHit::GetEdep() const { 
  return fEdep; 
}

inline G4double CalorHit::GetTrackLength() const { 
  return fTrackLength; 
}

inline G4double CalorHit::GetKinEn() const {
  return fKinEn;
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
