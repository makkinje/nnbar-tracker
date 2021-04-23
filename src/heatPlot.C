
{
gStyle->SetPalette(kBird); 
gStyle->SetOptStat(0); 
gStyle->SetOptTitle(0);

TFile f("nnbar-tracker.root");
/*
TH1D* xa = (TH1D*)f.Get("XYAA");
TH1D* xb = (TH1D*)f.Get("XYBA");
TH1D* xc = (TH1D*)f.Get("XYCA");
TH1D* xd = (TH1D*)f.Get("XYDA");

auto canvasa = new TCanvas("XYAA", "XYA", 800, 800);

xa->GetXaxis()->SetLimits(-10,10);
xa->GetYaxis()->SetLimits(-10,10);
xa->GetXaxis()->SetTitle("x (cm)");
xa->GetYaxis()->SetTitle("y (cm)");
pla = new TPaveLabel(-10,10,-4,9,"Position of Hits in Silicon Layer A","br");
xa->Draw("Colz");
pla->Draw();

auto canvasb = new TCanvas("XYBA", "XYB", 800, 800);

xb->GetXaxis()->SetLimits(-10,10);
xb->GetYaxis()->SetLimits(-10,10);
xb->GetXaxis()->SetTitle("x (cm)");
xb->GetYaxis()->SetTitle("y (cm)");
plb = new TPaveLabel(-10,10,-4,9,"Position of Hits in Silicon Layer B","br");
xb->Draw("Colz");
plb->Draw();

auto canvasc = new TCanvas("XYCA", "XYC", 800, 800);

xc->GetXaxis()->SetLimits(-10,10);
xc->GetYaxis()->SetLimits(-10,10);
xc->GetXaxis()->SetTitle("x (cm)");
xc->GetYaxis()->SetTitle("y (cm)");
plc = new TPaveLabel(-10,10,-4,9,"Position of Hits in Silicon Layer C","br");
xc->Draw("Colz");
plc->Draw();
*/
/*
auto canvasd = new TCanvas("XYDA", "XYD", 800, 800);

xd->GetXaxis()->SetLimits(-10,10);
xd->GetYaxis()->SetLimits(-10,10);
xd->GetXaxis()->SetTitle("x (cm)");
xd->GetYaxis()->SetTitle("y (cm)");
pld = new TPaveLabel(-10,10,-4,9,"Position of Hits in Silicon Layer D","br");
xd->Draw("Colz");
pld->Draw();
*/
/*
TH1D* xau = (TH1D*)f.Get("XYAU");
TH1D* xbu = (TH1D*)f.Get("XYBU");
TH1D* xcu = (TH1D*)f.Get("XYCU");

auto canvasau = new TCanvas("XYAU", "XYAU", 800, 800);

xau->GetXaxis()->SetLimits(-10,10);
xau->GetYaxis()->SetLimits(-10,10);
xau->GetXaxis()->SetTitle("x (cm)");
xau->GetYaxis()->SetTitle("y (cm)");
plau = new TPaveLabel(-10,10,-4,9,"Predicted Position of Hits in Silicon Layer A","br");
xau->Draw("Colz");
plau->Draw();

auto canvasbu = new TCanvas("XYBU", "XYBU", 800, 800);

xbu->GetXaxis()->SetLimits(-10,10);
xbu->GetYaxis()->SetLimits(-10,10);
xbu->GetXaxis()->SetTitle("x (cm)");
xbu->GetYaxis()->SetTitle("y (cm)");
plbu = new TPaveLabel(-10,10,-4,9,"Predicted Position of Hits in Silicon Layer B","br");
xbu->Draw("Colz");
plbu->Draw();

auto canvascu = new TCanvas("XYCU", "XYCU", 800, 800);

xcu->GetXaxis()->SetLimits(-10,10);
xcu->GetYaxis()->SetLimits(-10,10);
xcu->GetXaxis()->SetTitle("x (cm)");
xcu->GetYaxis()->SetTitle("y (cm)");
plcu = new TPaveLabel(-10,10,-4,9,"Predicted Position of Hits in Silicon Layer C","br");
xcu->Draw("Colz");
plcu->Draw();
*/
//TH1D* xcf = (TH1D*)f.Get("XY_Diff_C");
TH1D* xbf = (TH1D*)f.Get("XY_Diff_B");
//TH1D* xaf = (TH1D*)f.Get("XY_Diff_D");

gStyle->SetPadLeftMargin(1.00);

auto canvascf = new TCanvas("XY_Diff_B", "XY_Diff_B", 800, 800);

xbf->GetXaxis()->SetLimits(-500,500);
xbf->GetYaxis()->SetLimits(-500,500);
xbf->GetXaxis()->SetTitle("x-x_{p} (nm)");
xbf->GetYaxis()->SetTitle("y-y_{p} (nm)");
xbf->SetTitle("Difference Between Actual and Predicted Position in Si_{B}");
//plbf = new TPaveLabel(-500,520,-200,450,"Linear Approximation - Difference Between Predicted Position and Actual in Silicon Layer B","br");
xbf->Draw("COL");
xbf->GetYaxis()->SetLabelSize(0.025);
xbf->GetXaxis()->SetLabelSize(0.025);
plbf->Draw();

//auto canvasbf = new TCanvas("XYBF", "XYBF", 800, 800);

//xbf->GetXaxis()->SetLimits(-0.1,0.1);
//xbf->GetYaxis()->SetLimits(-0.1,0.1);
//xbf->GetXaxis()->SetTitle("Diff x (mm)");
//xbf->GetYaxis()->SetTitle("Diff y (mm)");
//plbf = new TPaveLabel(-25,25,0,20,"Difference Between Predicted Position and Actual in Silicon Layer B","br");
//xbf->Draw("Colz");
//plbf->Draw();

//auto canvasaf = new TCanvas("XYAF", "XYAF", 800, 800);

//xaf->GetXaxis()->SetLimits(-1,1);
//xaf->GetYaxis()->SetLimits(-1,1);
//xaf->GetXaxis()->SetTitle("Diff x (mm)");
//xaf->GetYaxis()->SetTitle("Diff x (mm)");
//plaf = new TPaveLabel(-25,25,0,20,"Difference Between Predicted Position and Actual in Silicon Layer D","br");
//xaf->Draw("Colz");
//plaf->Draw();

//test->Draw("Cont1");
//TFile *f = TFile::Open("nnbar-tracker.root");
//TH2D *test = (TH2D *)f->Get("XY_Frac_SiliconA");

//TH2F posSiA_h("Position_Si_A","x;y;Hits", 30,-10,10, // X axis
//                                                         30,-10,10); // Y axis
//TRandom3 rgen_5_3;

//for (int i=0;i<500000;i++)
//{
//  posSiA_h.Fill(i, 2*i, 3*i);  
//  TH1D* x_val = &test[0];
//  posSiA_h.Fill(*test[0], *test[1],0.1);
//  posSiA_h.Fill(rgen_5_3.Gaus(0,2),10-rgen_5_3.Exp(4),.1);
//  posSiA_h.Fill(i,i,i); //test[0], test[1], test[2]);
//}
/*
auto canvas_5_3_1=new TCanvas("canvas_5_3_1","canvas_5_3_1",800,800); 

canvas_5_3_1->Divide(2,2);
canvas_5_3_1->cd(1);
test->Draw("Cont1");
canvas_5_3_1->cd(2);
test->Draw("Colz");
canvas_5_3_1->cd(3);
test->Draw("lego");
canvas_5_3_1->cd(4);
test->Draw("surf3");
canvas_5_3_1->Draw();
*/
}
