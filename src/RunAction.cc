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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "Analysis.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

void PrintNParticles(std::map<const G4ParticleDefinition*, int>& container);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
 : G4UserRunAction()
{
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in Analysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple

  // Create 1D Histograms
  analysisManager->CreateH1("xBDiff", "XDiff - Layer B", 500, -500*nm, 500*nm); // -100*nm, 100*nm);
  analysisManager->CreateH1("yBDiff", "YDiff - Layer B", 500, -500*nm, 500*nm);
  analysisManager->CreateH1("xAHit", "X Position Layer A", 500, -100*cm, 100*cm);
  analysisManager->CreateH1("yAHit", "Y Position Layer A", 500, -100*cm, 100*cm);
  analysisManager->CreateH1("yBHit", "Y Position Layer B", 500, -10*cm, 10*cm);
  analysisManager->CreateH1("yBHit", "Y Position Layer B", 500, -10*cm, 10*cm);
  analysisManager->CreateH1("xAPred", "Predicted X Position Layer A", 500, -10*cm, 10*cm);
  analysisManager->CreateH1("yAPred", "Predicted Y Position Layer A", 500, -10*cm, 10*cm);
  analysisManager->CreateH1("ChiSquaredX", "Chi-Squared Value - X", 500, 0, 0.00000000000000005);
  analysisManager->CreateH1("ChiSquaredY", "Chi-Squared Value - Y", 500, 0, 0.00000000000000005);
  analysisManager->CreateH1("Theta", "Scattering Angle", 500, 0.0188, 0.0194);
  analysisManager->CreateH1("eDepSi", "eDep", 500, 0*MeV,1*MeV);
  analysisManager->CreateH1("pSi", "pSI", 500, 0*MeV, 10*MeV);

  analysisManager->CreateH1("xBDiff0", "XDiff - Layer B", 500, -25*mm, 25*mm); // -100*nm, 100*nm);
  analysisManager->CreateH1("xBDiff1", "XDiff - Layer B", 500, -25*mm, 25*mm); // -100*nm, 100*nm);
  analysisManager->CreateH1("xBDiff2", "XDiff - Layer B", 500, -25*mm, 25*mm); // -100*nm, 100*nm);

  analysisManager->CreateH1("xTheta", "xTheta", 500, 0*mm, 7*mm);

  analysisManager->CreateH1("xVarA", "xVarA", 500, 0*cm, 10*cm);
  analysisManager->CreateH1("yVarA", "yVarA", 500, 0*cm, 10*cm);

  // Create 2D Histograms
  analysisManager->CreateH2("XY_Diff_B", "X Diff vs. Y Diff - Layer B", 500, -500*nm, 500*nm, 500, -500*nm, 500*nm);
  analysisManager->CreateH2("XYA_Hits", "X Hit vs. Y Hit - Layer A", 500, -10*cm, 10*cm, 500, -10*cm, 10*cm);
  analysisManager->CreateH2("XYB_Hits", "X Hit vs. Y Hit - Layer B", 500, -10*cm, 10*cm, 500, -10*cm, 10*cm);
  analysisManager->CreateH2("XYA_Pred", "Predicted X Hit vs. Predicted Y Hit - Layer A", 500, -10*cm, 10*cm, 500, -10*cm, 10*cm);
  analysisManager->CreateH2("XYB_Pred", "Predicted X Hit vs. Predicted Y Hit - Layer B", 500, -10*cm, 10*cm, 500, -10*cm, 10*cm);



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
 
 // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "nnbar-tracker";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{ 

// extern std::vector fit_paramters;

  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    G4cout << " EAbs : mean = " 
       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;
        
    G4cout << " ESilicon : mean = " 
       << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;
    G4cout << " ETube : mean = "
       << G4BestUnit(analysisManager->GetH1(2)->mean(), "Energy")
       << " rms = "
       << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Energy") << G4endl;
    G4cout << " EScint : mean = "
       << G4BestUnit(analysisManager->GetH1(3)->mean(), "Energy")
       << " rms = "
       << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Energy") << G4endl;

    
    G4cout << " LAbs : mean = " 
      << G4BestUnit(analysisManager->GetH1(4)->mean(), "Length") 
      << " rms = " 
      << G4BestUnit(analysisManager->GetH1(4)->rms(),  "Length") << G4endl;

    G4cout << " LSilicon : mean = " 
      << G4BestUnit(analysisManager->GetH1(5)->mean(), "Length") 
      << " rms = " 
      << G4BestUnit(analysisManager->GetH1(5)->rms(),  "Length") << G4endl;
     G4cout << " LTube : mean = "
      << G4BestUnit(analysisManager->GetH1(6)->mean(), "Length")
      << " rms = "
      << G4BestUnit(analysisManager->GetH1(6)->rms(),  "Length") << G4endl;
     G4cout << " LScint : mean = "
      << G4BestUnit(analysisManager->GetH1(7)->mean(), "Length")
      << " rms = "
      << G4BestUnit(analysisManager->GetH1(7)->rms(),  "Length") << G4endl;
 }

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
