/**
 *  file: showGrid.C
 *  date: 2015.07.07
 *
 */

#include <vector>

void showGrid()
{
  gROOT->Reset();
  gStyle->SetOptFit(1);
  gStyle->SetOptStat(1111110);
  gStyle->SetStatH(0.12);
  gStyle->SetStatW(0.2);
  gStyle->SetStatX(0.91);
  gStyle->SetStatY(0.91);
  
  Double_t beam_loss;
  Double_t eff_emul;

  const Double_t EmulsionSizeX = 335.;
  const Double_t EmulsionSizeY = 340.;
  const Double_t Margin        =   0.;

  std::string root_file = "tmp.root";
  TFile *file = new TFile(root_file.c_str());
  TTree *tree = (TTree*)gFile->Get("tree");
  
  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
  //c1->SetRightMargin(0.28);
  c1->SetGrid();

  // TH2F *h = new TH2F("h", "Beam Center Grid",
  // 		     (Int_t)EmulsionSizeX+40, -20., EmulsionSizeX+20,
  // 		     (Int_t)EmulsionSizeY+40, -20., EmulsionSizeY+20 );
  // tree->Project("h", "ycenter:xcenter");
  // h->Draw("colz");

  std::vector<Double_t> xvec;
  std::vector<Double_t> yvec;
  
  Int_t spill;
  Double_t xcenter;
  Double_t ycenter;
  tree->SetBranchAddress("spill",   &spill);
  tree->SetBranchAddress("xcenter", &xcenter);
  tree->SetBranchAddress("ycenter", &ycenter);
  
  Int_t spill_inc = 0;
  Int_t nentries = tree->GetEntries();
  for(Int_t ie=0; ie<nentries; ie++){
    tree->GetEntry(ie);
    if( spill_inc == spill ) continue;
    spill_inc = spill;
    xvec.push_back( xcenter );
    yvec.push_back( ycenter );
  }

  TGraph *g1 = new TGraph( xvec.size(), &xvec[0], &yvec[0] );
  g1->SetTitle(Form("total spill: %d", spill_inc));
  g1->SetMarkerStyle(8); g1->SetMarkerSize(.5);
  g1->GetXaxis()->SetLimits(-20, 360);
  g1->GetYaxis()->SetRangeUser(-20, 360);
  g1->Draw("AP");
  c1->Update();
  // TPaletteAxis *p = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");
  // p->SetX2NDC(0.85); p->SetY2NDC(0.62);

  TLine line;
  //line.SetLineStyle(2);
  line.SetLineWidth(2); line.SetLineColor(kMagenta);
  line.DrawLine( Margin, Margin, EmulsionSizeX-Margin, Margin );
  line.DrawLine( Margin, Margin, Margin, EmulsionSizeY-Margin );
  line.DrawLine( EmulsionSizeX-Margin, Margin, EmulsionSizeX-Margin, EmulsionSizeY-Margin );
  line.DrawLine( Margin, EmulsionSizeY-Margin, EmulsionSizeX-Margin, EmulsionSizeY-Margin );
  c1->Update();

}
