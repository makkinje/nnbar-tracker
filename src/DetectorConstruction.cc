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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "CalorimeterSD.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fCheckOverlaps(true),
   fNofLayers(-1)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ 
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  
  // Define variables 
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density; 

  // Air
  G4Element* elN = nistManager->FindOrBuildElement("N");

  // Lead-Glass
  G4Element* elTi = nistManager->FindOrBuildElement("Ti");
  G4Element* elAs = nistManager->FindOrBuildElement("As");
  G4Element* elPb = nistManager->FindOrBuildElement("Pb");
  G4Element* elO = nistManager->FindOrBuildElement("O");
  G4Element* elSi = nistManager->FindOrBuildElement("Si");
  
  // Absorber 
  G4Material* Abs = new G4Material("Abs", 3.86*g/cm3, 5);
  Abs->AddElement(elO, 0.156453);
  Abs->AddElement(elSi, 0.080866);
  Abs->AddElement(elTi, 0.008092);
  Abs->AddElement(elAs, 0.002651);
  Abs->AddElement(elPb,0.751938);

  // TPC
  G4Element* elAr = nistManager->FindOrBuildElement("Ar");

  // BC-408 taken from datasheet
  G4Element* elH = nistManager->FindOrBuildElement("H");
  G4Element* elC = nistManager->FindOrBuildElement("C");

  // Find or build materials

  // Scint 
  G4Material* Scint = new G4Material("Scint", 1.023*g/cm3, 2);
  Scint->AddElement(elH, 0.524573);
  Scint->AddElement(elC, 1 - 0.524573);
 
  // Tube
  new G4Material("Aluminium", z=13., a= 26.98*g/mole, density= 2.70*g/cm3);
 
  // Silicon
  new G4Material("Silicon", z=14., a= 28.0855*g/mole, density = 2.33*g/cm3);

  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);
  
  // Air
  G4Material* Air = new G4Material("Air", density = 1.29*mg/cm3, 2);
  Air->AddElement(elN, 0.7);
  Air->AddElement(elO, 0.3);

  // FR4
  // Epoxy
  G4Material* Epoxy = new G4Material("Epoxy", density = 1.2*g/cm3, 2);
  Epoxy->AddElement(elH, 2);
  Epoxy->AddElement(elC, 2);
  
  // SiO2 (Quartz)
  G4Material* SiO2 = new G4Material("SiO2", density = 2.200*g/cm3, 2);
  SiO2->AddElement(elSi, 1);
  SiO2->AddElement(elO, 2);

  // FR4 (Glass + Epoxy)
  G4Material* FR4 = new G4Material("FR4", density = 1.86*g/cm3, 2);
  FR4->AddMaterial(Epoxy, 0.472);
  FR4->AddMaterial(SiO2, 0.528);

  // TPC
  
  // CO2
  G4Material* CO2 = new G4Material("CO2", density = 1.98*g/cm3, 2);
  CO2->AddElement(elO, 2);
  CO2->AddElement(elC, 1);

  // Air/CO2 90/20
  G4Material* Gas = new G4Material("Gas", density = 1.3954*g/cm3, 2);
  Gas->AddElement(elAr, 0.8);
  Gas->AddMaterial(CO2, 0.2);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  G4double PhotonWavelength[] =
      { 2325.4*nm, 1970.1*nm, 1529.6*nm, 1060.0*nm,
        1014.0*nm, 852.10*nm, 706.50*nm, 656.30*nm,
        643.80*nm, 632.80*nm, 589.30*nm, 587.60*nm,
        546.10*nm, 486.10*nm, 480.00*nm, 435.80*nm,
        404.70*nm, 365.00*nm
       };


  const G4int nEntries = sizeof(PhotonWavelength)/sizeof(G4double);

  G4double PhotonEnergy[nEntries];

  for (int i=0; i < nEntries; ++i) {
    PhotonEnergy[i] = (1240.*nm/PhotonWavelength[i])*eV;
  };

  G4double refractiveIndex[] =
        { 1.63289, 1.63785, 1.64359, 1.65104,
          1.65206, 1.65664, 1.66327, 1.66661,
          1.66756, 1.66846, 1.67252, 1.67270,
          1.67764, 1.68750, 1.68876, 1.69986,
          1.71069, 1.73056};
G4MaterialPropertiesTable* absMPT = new G4MaterialPropertiesTable();

absMPT->AddProperty("RINDEX", PhotonEnergy, refractiveIndex, nEntries)
      ->SetSpline(true);

Abs->SetMaterialPropertiesTable(absMPT);

  G4cout << "Absorber Properties -------" << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  absMPT->DumpTable();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // Geometry parameters
  fNofLayers = 1;
/*
  G4int scintLayers = 10;
  G4int siliconLayers = 50;
  G4double scintThickness = 3.*cm;
  G4double absoThickness = 25.*cm;
*/
  G4double tubeThickness = 2.*cm;
  G4double calorSizeXY  = 1.*m;
  G4double siliconSizeX = 0.1*cm;
  G4double siliconThickness =  0.3*mm;

  auto calorThickness = 10*cm + 10*cm + 1*cm+ 4*siliconThickness + tubeThickness + 1*m; //2*(absoThickness + siliconThickness + tubeThickness + scintLayers*scintThickness);
  auto worldSizeXY = 1.2 * calorSizeXY;
  auto worldSizeZ  = 1.2 * calorThickness; 
  
  // Get materials
  auto defaultMaterial = G4Material::GetMaterial("Galactic");
  auto absorberMaterial = G4Material::GetMaterial("Abs");
  auto siliconMaterial = G4Material::GetMaterial("Silicon");
  auto tubeMaterial = G4Material::GetMaterial("Aluminium");
  auto scintMaterial = G4Material::GetMaterial("Scint");
  auto FR4Material = G4Material::GetMaterial("FR4");
  auto TPCMaterial = G4Material::GetMaterial("Gas");
 
  if ( ! defaultMaterial || ! absorberMaterial || ! siliconMaterial || ! tubeMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }  
   
  //     
  // World
  //
  auto worldS 
    = new G4Box("World",           // its name
                 worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); // its size
                         
  auto worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 defaultMaterial,  // its material
                 "World");         // its name
                                   
  auto worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
  
  //                               
  // Calorimeter
  //  
  auto calorimeterS
    = new G4Box("Calorimeter",     // its name
                 calorSizeXY/2, calorSizeXY/2, calorThickness/2); // its size
                         
  auto calorLV
    = new G4LogicalVolume(
                 calorimeterS,     // its solid
                 defaultMaterial,  // its material
                 "Calorimeter");   // its name
                                   
  new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 calorLV,          // its logical volume                         
                 "Calorimeter",    // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
  // 
  // Silicon A
  //
  
  G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
  rotationMatrix->rotateY(0.*deg);

  auto siliconAS
    = new G4Box("SiliconA",             // its name
                calorSizeXY/2, calorSizeXY/2, siliconThickness/2); // its size

  auto siliconALV
    = new G4LogicalVolume(
                 siliconAS,             // its solid
                 siliconMaterial,      // its material
                 "SiliconALV");         // its name

  new G4PVPlacement(
                 rotationMatrix,                // no rotation
                 G4ThreeVector(0.*cm, 0.*cm, siliconThickness/2), // its position
                 siliconALV,            // its logical volume                         
                 "SiliconA",            // its name
                 calorLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 

  //
  // Silicon B
  //

  auto siliconBS
    = new G4Box("SiliconB",             // its name
                calorSizeXY/2, calorSizeXY/2, siliconThickness/2); // its size

  auto siliconBLV
    = new G4LogicalVolume(
                 siliconBS,             // its solid
                 siliconMaterial,      // its material
                 "SiliconBLV");         // its name

  new G4PVPlacement(
                 rotationMatrix,                // no rotation
                 G4ThreeVector(0.*cm, 0.*cm, siliconThickness+siliconThickness/2+10*cm), // its position
                 siliconBLV,            // its logical volume                         
                 "SiliconB",            // its name
                 calorLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 

  // 
  // Tube
  //

  auto tubeS 
    = new G4Box("Tube", calorSizeXY/2, calorSizeXY/2, tubeThickness/2);

  auto tubeLV
    = new G4LogicalVolume(tubeS, tubeMaterial, "TubeLV");

  new G4PVPlacement(0, G4ThreeVector(0., 0., 2*siliconThickness + 10*cm+tubeThickness/2+1*cm), tubeLV, "Tube", calorLV, false, 0, fCheckOverlaps);

  //
  // Silicon C
  //
/*
  auto siliconCS
    = new G4Box("SiliconC",             // its name
                calorSizeXY/2, calorSizeXY/2, siliconThickness/2); // its size

  auto siliconCLV
    = new G4LogicalVolume(
                 siliconCS,             // its solid
                 siliconMaterial,      // its material
                 "SiliconCLV");         // its name

  new G4PVPlacement(
                 rotationMatrix,                // no rotation
                 G4ThreeVector(0.*cm, 0.*cm, 2*siliconThickness+10*cm+tubeThickness + 2*m + siliconThickness/2), // its position
                 siliconCLV,            // its logical volume                         
                 "SiliconC",            // its name
                 calorLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 

  //
  // Silicon D
  //

  auto siliconDS
    = new G4Box("SiliconD",             // its name
                calorSizeXY/2, calorSizeXY/2, siliconThickness/2); // its size

  auto siliconDLV
    = new G4LogicalVolume(
                 siliconDS,             // its solid
                 siliconMaterial,      // its material
                 "SiliconDLV");         // its name

  new G4PVPlacement(
                 rotationMatrix,                // no rotation
                 G4ThreeVector(0.*cm, 0.*cm, 2*siliconThickness+30*cm+tubeThickness+2*m+siliconThickness+30*cm), // its position
                 siliconDLV,            // its logical volume                         
                 "SiliconD",            // its name
                 calorLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
 */
  //                                        
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());

  auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  calorLV->SetVisAttributes(simpleBoxVisAtt);

  //
  // Always return the physical World
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // 
  // Sensitive detectors
  //

  auto siliconASD 
    = new CalorimeterSD("SiliconASD", "SiliconAHitsCollection", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(siliconASD);
  SetSensitiveDetector("SiliconALV",siliconASD);
 
  auto siliconBSD
    = new CalorimeterSD("SiliconBSD", "SiliconBHitsCollection", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(siliconBSD);
  SetSensitiveDetector("SiliconBLV",siliconBSD);

  auto tubeSD
    = new CalorimeterSD("TubeSD", "TubeHitsCollection", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(tubeSD);
  SetSensitiveDetector("TubeLV",tubeSD);
/*
  auto siliconCSD
    = new CalorimeterSD("SiliconCSD", "SiliconCHitsCollection", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(siliconCSD);
  SetSensitiveDetector("SiliconCLV",siliconCSD);

  auto siliconDSD
    = new CalorimeterSD("SiliconDSD", "SiliconDHitsCollection", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(siliconDSD);
  SetSensitiveDetector("SiliconDLV",siliconDSD);
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
