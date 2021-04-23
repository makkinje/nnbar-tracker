#include "G4Run.hh"

#include "EventAction.hh"
#include "CalorimeterSD.hh"
#include "CalorHit.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>


  extern std::vector<G4double> x_parameters;
  extern std::vector<G4double> y_parameters;
  extern std::vector<G4double> z_parameters;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
 : G4UserEventAction(),
   fSiliconAHCID(-1),
   fSiliconBHCID(-1),
   fTubeHCID(-1),
   fSiliconCHCID(-1),
   fSiliconDHCID(-1)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorHitsCollection*
EventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{ 
  auto hitsCollection
    = static_cast<CalorHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("EventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }
  
  return hitsCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  if ( fTubeHCID == -1 )
  { 
    fSiliconAHCID
      = G4SDManager::GetSDMpointer()->GetCollectionID("SiliconAHitsCollection");
    fSiliconBHCID
      = G4SDManager::GetSDMpointer()->GetCollectionID("SiliconBHitsCollection");
    fTubeHCID
      = G4SDManager::GetSDMpointer()->GetCollectionID("TubeHitsCollection");
    fSiliconCHCID
      = G4SDManager::GetSDMpointer()->GetCollectionID("SiliconCHitsCollection");
    fSiliconDHCID
      = G4SDManager::GetSDMpointer()->GetCollectionID("SiliconDHitsCollection");
  }

  auto siliconAHC = GetHitsCollection(fSiliconAHCID, event);
  auto siliconBHC = GetHitsCollection(fSiliconBHCID, event);
  auto tubeHC = GetHitsCollection(fTubeHCID, event);
  auto siliconCHC = GetHitsCollection(fSiliconCHCID, event);
  auto siliconDHC = GetHitsCollection(fSiliconDHCID, event);

  auto siliconAHit = (*siliconAHC)[siliconAHC->entries()-1];
  auto siliconBHit = (*siliconBHC)[siliconBHC->entries()-1];
  auto tubeHit = (*tubeHC)[tubeHC->entries()-1];
  auto siliconCHit = (*siliconCHC)[siliconCHC->entries()-1];
  auto siliconDHit = (*siliconDHC)[siliconDHC->entries()-1];

  // Define measurements at silicon layers
  G4double measurementA[6] = {-999, -999, -999, -999, -999, -999}; // {x,y,z,dx/dz,dy/dz,E}
  G4double measurementB[6] = {-999, -999, -999, -999, -999, -999}; // {x,y,z,dx/dz,dy/dz,E}

  // Forwards
  G4double MappingMatrixAB[6][6] = {{0., 0., 0., 0., 0., 0.}, {0., 0., 0., 0., 0., 0.}, {0., 0., 0., 0., 0., 0.}, {0., 0., 0., 0., 0., 0.}, {0., 0., 0., 0., 0., 0.}, {0., 0., 0., 0., 0., 0.,}};
  G4double MappingMatrixBC[6][6] = {{0., 0., 0., 0., 0., 0.}, {0., 0., 0., 0., 0., 0.}, {0., 0., 0., 0., 0., 0.}, {0., 0., 0., 0., 0., 0.}, {0., 0., 0., 0., 0., 0.}, {0., 0., 0., 0., 0., 0.,}};

  // Fill measurement matrix for layer A
  measurementA[0] = siliconAHit->GetPos().x();
  measurementA[1] = siliconAHit->GetPos().y();
  measurementA[2] = siliconAHit->GetPos().z();
  measurementA[3] = ((siliconAHit->GetPos().x() - siliconAHit->GetPrePos().x())/(siliconAHit->GetPos().z() - siliconAHit->GetPrePos().z()));
  measurementA[4] = ((siliconAHit->GetPos().y() - siliconAHit->GetPrePos().y())/(siliconAHit->GetPos().z() - siliconAHit->GetPrePos().z()));
  measurementA[5] = siliconAHit->GetEdep();

  // Fill measurement matrix for layer B
  measurementB[0] = siliconBHit->GetPos().x();
  measurementB[1] = siliconBHit->GetPos().y();
  measurementB[2] = siliconBHit->GetPos().z();
  measurementB[3] = ((siliconBHit->GetPos().x() - siliconBHit->GetPrePos().x())/(siliconBHit->GetPos().z() - siliconBHit->GetPrePos().z()));
  measurementB[4] = ((siliconBHit->GetPos().y() - siliconBHit->GetPrePos().y())/(siliconBHit->GetPos().z() - siliconBHit->GetPrePos().z()));
  measurementB[5] = siliconBHit->GetEdep();

  // Forwards
  MappingMatrixAB[0][3] = 30/CLHEP::cm*100;
  MappingMatrixAB[1][4] = 30/CLHEP::cm*100;
  MappingMatrixBC[0][3] = 15/CLHEP::cm*100;
  MappingMatrixBC[1][4] = 15/CLHEP::cm*100;

  // G4cout << "Mapping Matrix: " << G4BestUnit(MappingMatrixAB[0][3], "Length") << G4endl;

  // Fill diagonals of mapping matrices
  for (int i = 0; i < 6; i++)
  {
    MappingMatrixAB[i][i] = 1;
    MappingMatrixBC[i][i] = 1;
  }
  // Measurement at layer A
  G4double xA = measurementA[0];
  G4double yA = measurementA[1];

  // Measurement at layer B
  G4double xB = measurementB[0];
  G4double yB = measurementB[1];

//  G4cout << "tubeHit: " << tubeHit->GetPos().x() << G4endl;
//  G4cout << "siliconBHit: " << siliconBHit->GetPos().x() << G4endl;
//  G4cout << "xB: " << G4BestUnit(measurementB[0], "Length") << G4endl;

  // Define variables
  G4double Q = 0.05;
  G4double R = 0.1;
  G4double C = 1;
  G4double PA = 1;
  G4double QX = -9999;
  G4double QY = -9999;

  // Define variables after update
  G4double xAUpdate = -9999;
  G4double yAUpdate = -9999;
  G4double xBUpdate = -9999;
  G4double yBUpdate = -9999;
  G4double xCUpdate = -9999;
  G4double yCUpdate = -9999;

  // Define Difference
  G4double xBDiff = -9999/CLHEP::cm;
  G4double yBDiff = -9999/CLHEP::cm;
  G4double xCDiff = -9999/CLHEP::cm;
  G4double yCDiff = -9999/CLHEP::cm;

  // Define r=\sqrt{x^{2}+y^{2}} difference
  G4double rB = 0;

  // Define chi-squared
  G4double chiSquaredBX = 0;
  G4double chiSquaredBY = 0;

  G4double varianceAX = -9999;
  G4double varianceAY = -9999;

  G4double varianceBX = - 9999;
  G4double varianceBY = - 9999;

  G4double varianceCX = - 9999;
  G4double varianceCY = - 9999;

  G4double eConst = 13.6/CLHEP::MeV;
  G4double theta0 = -9999;
  G4double length = 30/CLHEP::cm*100;
  auto analysisManager = G4AnalysisManager::Instance();

  G4double eDepSi = siliconAHit->GetEdep();
  G4double pSi = sqrt((siliconAHit->GetMomentum().x()*siliconAHit->GetMomentum().x())+(siliconAHit->GetMomentum().y()*siliconAHit->GetMomentum().y())+(siliconAHit->GetMomentum().z()*siliconAHit->GetMomentum().z()));
  // If hits at both A and B, then predict B from A

/*
for (int i = 0; i < 3; i++)
{
//  G4cout << "i: " << i << G4endl;
  // Linear
  if (i == 0)
  { PA = 0;
//    G4cout << "PA: " << PA << G4endl;
  if (measurementB[0] != 0 && measurementA[0] != 0)
  {
    G4double xBPred = xA + measurementA[3]*MappingMatrixAB[0][3];
    G4double momentum = sqrt((siliconAHit->GetMomentum().x()*siliconAHit->GetMomentum().x())+(siliconAHit->GetMomentum().y()*siliconAHit->GetMomentum().y())+(siliconAHit->GetMomentum().z()*siliconAHit->GetMomentum().z()));    
    G4double velocity = siliconAHit->GetVelocity();
    G4double beta = velocity/300;
    G4double x0 = 42.7005788; // g/cm^2
    G4double num = 0.83712;
    theta0 = eConst/(momentum*beta)*num;

    G4double PBX = MappingMatrixAB[0][3]*PA*MappingMatrixAB[0][3] + Q;
    G4double KBX = PBX*1/(1*PBX*1+R);
    xBUpdate = xBPred + KBX*(measurementB[0]-xBPred);
    G4double PBUpdateX = (1-KBX)*PBX;
    xBDiff = (measurementB[0] - xBPred); //(measurementB[0] - xBUpdate);
    varianceAX = (analysisManager->GetH1(2)->rms())*(analysisManager->GetH1(2)->rms());
    varianceBX = (analysisManager->GetH1(4)->rms())*(analysisManager->GetH1(4)->rms());
    chiSquaredBX = (measurementB[0]-xBUpdate)*(measurementB[0]-xBUpdate)/(varianceBX*varianceBX);

    G4double yBPred = yA + measurementA[4]*MappingMatrixAB[1][4];
    G4double PBY = MappingMatrixAB[1][4]*PA*MappingMatrixAB[1][4] + Q;
    G4double KBY = PBY*1/(1*PBY*1+R);
    yBUpdate = yBPred + KBY*(measurementB[1]-yBPred);
    G4double PBUpdateY = (1-KBY)*PBY;
    yBDiff = (measurementB[1] - yBUpdate);
    varianceAY = (analysisManager->GetH1(3)->rms())*(analysisManager->GetH1(3)->rms());
    varianceBY = (analysisManager->GetH1(5)->rms())*(analysisManager->GetH1(5)->rms());
    chiSquaredBY = (measurementB[1]-yBUpdate)*(measurementB[1]-yBUpdate)/(varianceBY*varianceBY);
    }
  }

  // P_{X=0}, Q = 0.05
  else if (i == 1)
  { PA = 0;
//    G4cout << "PA: " << PA << G4endl;

  if (measurementB[0] != 0 && measurementA[0] != 0)
  {
    G4double xBPred = xA + measurementA[3]*MappingMatrixAB[0][3];
    G4double momentum = sqrt((siliconAHit->GetMomentum().x()*siliconAHit->GetMomentum().x())+(siliconAHit->GetMomentum().y()*siliconAHit->GetMomentum().y())+(siliconAHit->GetMomentum().z()*siliconAHit->GetMomentum().z()));    
    G4double velocity = siliconAHit->GetVelocity();
    G4double beta = velocity/300;
    G4double x0 = 42.7005788; // g/cm^2
    G4double num = 0.83712;
    theta0 = eConst/(momentum*beta)*num;

    Q = 0.05;
    G4double PBX = MappingMatrixAB[0][3]*PA*MappingMatrixAB[0][3] + Q;
    G4double KBX = PBX*1/(1*PBX*1+R);
    xBUpdate = xBPred + KBX*(measurementB[0]-xBPred);
    G4double PBUpdateX = (1-KBX)*PBX;
    xBDiff = (measurementB[0] - xBUpdate);
    varianceAX = (analysisManager->GetH1(2)->rms())*(analysisManager->GetH1(2)->rms());
    varianceBX = (analysisManager->GetH1(4)->rms())*(analysisManager->GetH1(4)->rms());
  chiSquaredBX = (measurementB[0]-xBUpdate)*(measurementB[0]-xBUpdate)/(varianceBX*varianceBX);

    G4double yBPred = yA + measurementA[4]*MappingMatrixAB[1][4];
    G4double PBY = MappingMatrixAB[1][4]*PA*MappingMatrixAB[1][4] + Q;
    G4double KBY = PBY*1/(1*PBY*1+R);
    yBUpdate = yBPred + KBY*(measurementB[1]-yBPred);
    G4double PBUpdateY = (1-KBY)*PBY;
    yBDiff = (measurementB[1] - yBUpdate);
    varianceAY = (analysisManager->GetH1(3)->rms())*(analysisManager->GetH1(3)->rms());
    varianceBY = (analysisManager->GetH1(5)->rms())*(analysisManager->GetH1(5)->rms());
    chiSquaredBY = (measurementB[1]-yBUpdate)*(measurementB[1]-yBUpdate)/(varianceBY*varianceBY);
    }
  }


  // P_{X=0}, Q = MS
  else if (i == 2)
  { PA = 0;
  if (measurementB[0] != 0 && measurementA[0] != 0)
  {
    G4double xBPred = xA + measurementA[3]*MappingMatrixAB[0][3];
    G4double momentum = sqrt((siliconAHit->GetMomentum().x()*siliconAHit->GetMomentum().x())+(siliconAHit->GetMomentum().y()*siliconAHit->GetMomentum().y())+(siliconAHit->GetMomentum().z()*siliconAHit->GetMomentum().z()));    
    G4double velocity = siliconAHit->GetVelocity();
    G4double beta = velocity/300;
    G4double x0 = 42.7005788; // g/cm^2
    G4double num = 0.83712;
    if (momentum != 0)
    {
      theta0 = eConst/(momentum*beta)*num;
    }
    else if (momentum==0)
    {
      theta0 = 9999;
    }  
 
    G4double phi = atan(siliconBHit->GetPos().y()/siliconBHit->GetPos().x());
    G4double radius = (30/CLHEP::cm)*100/cos(theta0);

   
    QX = radius*sin(theta0)*cos(phi);
    QY = radius*sin(theta0)*sin(phi);
    
//    G4cout << "phi: " << phi << G4endl; 
//    G4cout << "cosphi: " << cos(phi) << G4endl;
//    G4cout << "QX: " << QX << G4endl;

    G4double PBX = MappingMatrixAB[0][3]*PA*MappingMatrixAB[0][3] + QX; // Q;
    G4double KBX = PBX*1/(1*PBX*1+R);
    xBUpdate = xBPred + KBX*(measurementB[0]-xBPred);
    G4double PBUpdateX = (1-KBX)*PBX;
    xBDiff = (measurementB[0] - xBUpdate);
    varianceAX = (analysisManager->GetH1(2)->rms())*(analysisManager->GetH1(2)->rms());
    varianceBX = (analysisManager->GetH1(4)->rms())*(analysisManager->GetH1(4)->rms());
    chiSquaredBX = (measurementB[0]-xBUpdate)*(measurementB[0]-xBUpdate)/(varianceBX*varianceBX);

    G4double yBPred = yA + measurementA[4]*MappingMatrixAB[1][4];
    G4double PBY = MappingMatrixAB[1][4]*PA*MappingMatrixAB[1][4] + Q;
    G4double KBY = PBY*1/(1*PBY*1+R);
    yBUpdate = yBPred + KBY*(measurementB[1]-yBPred);
    G4double PBUpdateY = (1-KBY)*PBY;
    yBDiff = (measurementB[1] - yBUpdate);
    varianceAY = (analysisManager->GetH1(3)->rms())*(analysisManager->GetH1(3)->rms());
    //G4cout << varianceAY << G4endl;
    varianceBY = (analysisManager->GetH1(5)->rms())*(analysisManager->GetH1(5)->rms());
    chiSquaredBY = (measurementB[1]-yBUpdate)*(measurementB[1]-yBUpdate)/(varianceBY*varianceBY);
    }
  }
*/
  G4String particleA = siliconAHit->GetParticle();
  G4String particleB = siliconBHit->GetParticle();

  if (measurementB[0] != 0 && measurementA[0] != 0)// && particleA == particleB)
  {
    // Define predicted variable and update

   // G4cout << "xA: " << G4BestUnit(xA, "Length") << G4endl;
   // G4cout << "Distance: " << G4BestUnit(MappingMatrixAB[0][3], "Length") << G4endl;
   // G4cout << "Slope: " << measurementA[3] << G4endl;
   // G4cout << "xB: " << G4BestUnit(siliconBHit->GetPos().x(), "Length") << G4endl;
   // G4cout << "Particle A: " << siliconAHit->GetParticle() << G4endl;
   // G4cout << "Particle B: " << siliconBHit->GetParticle() << G4endl;

    G4double xBPred = xA + measurementA[3]*MappingMatrixAB[0][3];//+ (siliconBHit->GetPos().x()-siliconAHit->GetPos().x())/(siliconBHit->GetPos().z()-siliconAHit->GetPos().z())*MappingMatrixAB[0][3]; //measurementA[3]*MappingMatrixAB[0][3];
    G4double momentum = sqrt((siliconAHit->GetMomentum().x()*siliconAHit->GetMomentum().x())+(siliconAHit->GetMomentum().y()*siliconAHit->GetMomentum().y())+(siliconAHit->GetMomentum().z()*siliconAHit->GetMomentum().z()));
    G4double velocity = siliconAHit->GetVelocity();
    G4double beta = velocity/300;
    G4double x0 = 42.7005788; // g/cm^2
    G4double num = 0.83712;

    if (momentum != 0)
    {
      theta0 = eConst/(momentum*beta)*num;
    }
    else if (momentum==0)
    {
      theta0 = 9999;
    }

    G4double phi = atan(siliconBHit->GetPos().y()/siliconBHit->GetPos().x());
    G4double radius = (30/CLHEP::cm)*100/cos(theta0);

    QX = radius*sin(theta0)*cos(phi);
    QY = radius*sin(theta0)*sin(phi);

    G4double PAX = (0.6931/CLHEP::cm)*100*(0.6931/CLHEP::cm)*100;
    G4double PBX = MappingMatrixAB[0][3]*PA*MappingMatrixAB[0][3] + QX;
    G4double KBX = PBX*1/(1*PBX*1+R);
    xBUpdate = xBPred + KBX*(measurementB[0]-xBPred);
    G4double PBUpdateX = (1-KBX)*PBX;
    xBDiff = (measurementB[0] - xBUpdate);
    varianceBX = (analysisManager->GetH1(4)->rms())*(analysisManager->GetH1(4)->rms());
    chiSquaredBX = (measurementB[0]-xBUpdate)*(measurementB[0]-xBUpdate)/(varianceBX*varianceBX);

    G4double PAY = (0.7092/CLHEP::cm)*100*(0.7092/CLHEP::cm)*100;
    G4double yBPred = yA + measurementA[4]*MappingMatrixAB[1][4];
    G4double PBY = MappingMatrixAB[1][4]*PA*MappingMatrixAB[1][4] + QY;
    G4double KBY = PBY*1/(1*PBY*1+R);
    yBUpdate = yBPred + KBY*(measurementB[1]-yBPred);
    G4double PBUpdateY = (1-KBY)*PBY;
    yBDiff = (measurementB[1] - yBUpdate);
    varianceBY = (analysisManager->GetH1(5)->rms())*(analysisManager->GetH1(5)->rms());
    chiSquaredBY = (measurementB[1]-yBUpdate)*(measurementB[1]-yBUpdate)/(varianceBY*varianceBY);

    G4double momentumB = sqrt((siliconBHit->GetMomentum().x()*siliconBHit->GetMomentum().x())+(siliconBHit->GetMomentum().y()*siliconBHit->GetMomentum().y())+(siliconBHit->GetMomentum().z()*siliconBHit->GetMomentum().z()));
    G4double velocityB = siliconBHit->GetVelocity();
    G4double betaB = velocityB/300;

    if (momentumB != 0)
    {
      theta0 = eConst/(momentumB*betaB)*num;
    }

    else if (momentumB==0)
    {
      theta0 = 9999;
    }
    // Fit a line
  
    G4double x,y,z = -999;
    G4double lambda = -999;
    G4double dx, dy, dz = -999;
 
    dx = siliconBHit->GetPos().x()-siliconAHit->GetPos().x();
    dy = siliconBHit->GetPos().y()-siliconAHit->GetPos().y();
    dz = siliconBHit->GetPos().z()-siliconAHit->GetPos().z();

  }
/*
  if(i==0)
  {
    analysisManager->FillH1(13, xBDiff);
  }

  if(i==1)
  {
    analysisManager->FillH1(14, xBDiff);
  }

  if(i==2)
  {
    analysisManager->FillH1(15, xBDiff);
    analysisManager->FillH1(16, QX);
  }

 }
*/

  // Radius value
  rB = std::sqrt(xBDiff*xBDiff + yBDiff*yBDiff);

  if(siliconAHit->GetPos().x() != 0 && siliconBHit->GetPos().x() != 0)
  {
    G4double xHit[2] = {siliconAHit->GetPos().x(),siliconBHit->GetPos().x()}; 
  
    G4cout << "siliconAHitX: " << G4BestUnit(siliconAHit->GetPos().x(), "Length") << G4endl;
    G4cout << "siliconBHitX: " << G4BestUnit(siliconBHit->GetPos().x(), "Length") << G4endl;
 
//  x_parameters.push_back(siliconAHit->GetPos().x());
    x_parameters.insert(x_parameters.end(), xHit, xHit+2);

    for(int i=0; i < x_parameters.size(); i++)
    {
      G4cout << x_parameters.at(i) << G4endl;
    }
  }
//  x_parameters.insert(x_parameters.end(), xHit, xHit+2);

//  G4cout << x_parameters << G4endl;
//  G4cout << G4BestUnit(x_parameters[0], "Length") << G4endl;
//  G4cout << G4BestUnit(xHit[1], "Length") << G4endl;

  // Write values
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;
  }

  // Two dimensional histograms
  analysisManager->FillH2(0, xBDiff, yBDiff);
  analysisManager->FillH2(1, siliconAHit->GetPos().x(), siliconAHit->GetPos().y());
  analysisManager->FillH2(2, siliconBHit->GetPos().x(), siliconBHit->GetPos().y());
  analysisManager->FillH2(3, xAUpdate, yAUpdate);
  analysisManager->FillH2(4, xBUpdate, yBUpdate);

  // One dimensional histograms
  analysisManager->FillH1(0, xBDiff);
  analysisManager->FillH1(1, yBDiff);
  analysisManager->FillH1(2, siliconAHit->GetPos().x());
  analysisManager->FillH1(3, siliconAHit->GetPos().y());
  analysisManager->FillH1(4, siliconBHit->GetPos().x());
  analysisManager->FillH1(5, siliconBHit->GetPos().y());
  analysisManager->FillH1(6, xBUpdate);
  analysisManager->FillH1(7, yBUpdate);
  analysisManager->FillH1(8, chiSquaredBX);
  analysisManager->FillH1(9, chiSquaredBY);
  analysisManager->FillH1(10, theta0);
  analysisManager->FillH1(11, eDepSi);
  analysisManager->FillH1(12, pSi);

  analysisManager->FillH1(17, varianceAX);
  analysisManager->FillH1(18, varianceAY);
}

