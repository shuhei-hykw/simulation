/***********************************
 *  file: showDist.C
 *  date: 2016.01.31
 ***********************************/

#include <iostream>

#include "TCanvas.h"
#include "TCut.h"
#include "TF1.h"
#include "TFile.h"
#include "TLine.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TPaletteAxis.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"

//_____________________________________________________________________
void
showDist( void )
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

  static const Double_t EmulsionSizeX = 335.;
  static const Double_t EmulsionSizeY = 340.;

  std::string root_file = "tmp.root";
  //std::string root_file = "margin_-0.5.root";
  TFile *f    = new TFile(root_file.c_str());
  TTree *tree = (TTree*)gFile->Get("tree");

  TCanvas *c1 = new TCanvas("c1", "c1", 1200, 800);
  c1->Divide(2,2);

  Double_t mean;
  {
    c1->cd(2)->SetGrid();
    TH1F *h_den  = new TH1F( "h_den", "beam density in emulsion", 250, 0, 250 );
    TH2F *h0 = (TH2F*)f->Get("h0");
    for(Int_t x=0; x<EmulsionSizeX; x++){
      for(Int_t y=0; y<EmulsionSizeY; y++){
	Int_t density = h0->Integral( x+1, x+1, y+1, y+1 );
	h_den->Fill( density );
      }
    }
    h_den->GetXaxis()->SetTitle("Counts/mm^{2}");
    TF1 *f = new TF1("f", "gaus");
    Double_t max = h_den->GetBinCenter(h_den->GetMaximumBin());
    h_den->Fit("f", "Q", "", max-10, max+10 );
    mean           = f->GetParameter("Mean");
    Double_t sigma = f->GetParameter("Sigma");
    h_den->Fit("f", "Q", "", mean-2*sigma, mean+5*sigma );      
    c1->Update();
  }

  {
    c1->cd(1)->SetGrid();
    c1->cd(1)->SetRightMargin(0.28);
    Double_t edge = 60.;
    TH2F *h = new TH2F( "h_xy", "XY distribution",
			(Int_t)EmulsionSizeX+edge*2, -edge, EmulsionSizeX+edge,
			(Int_t)EmulsionSizeY+edge*2, -edge, EmulsionSizeY+edge );
    tree->Project("h_xy", "y:x");
    Double_t all = h->GetEntries();
    Double_t beam_acc = tree->GetEntries(Form("0<=x && x<=%lf && 0<=y && y<=%lf",
					 EmulsionSizeX, EmulsionSizeY));
    Double_t emul_acc = EmulsionSizeX*EmulsionSizeY*mean
      * ( 335.0*340.0 - 38.0*25.0*4 ) / ( 335.0*340.0 );
    std::cout<<"beam eff. : "<<beam_acc/all<<std::endl
	     <<"emul eff. : "<<beam_acc/emul_acc<<std::endl;
    h->GetXaxis()->SetTitle("[mm]");
    h->GetYaxis()->SetTitle("[mm]");

    // h->GetXaxis()->SetRangeUser(-10,40);
    // h->GetYaxis()->SetRangeUser(-10,40);
      
    h->Draw("colz");
    c1->Update();
    TPaletteAxis *p
      = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");
    p->SetX2NDC(0.85); p->SetY2NDC(0.62);

    TLine line;
    line.SetLineWidth(2); line.SetLineColor(kMagenta);
    line.DrawLine( 0., 0., EmulsionSizeX, 0. );
    line.DrawLine( 0., 0., 0., EmulsionSizeY );
    line.DrawLine( EmulsionSizeX, 0., EmulsionSizeX, EmulsionSizeY );
    line.DrawLine( 0., EmulsionSizeY, EmulsionSizeX, EmulsionSizeY );
    line.SetLineStyle(2);

    c1->Update();

    Int_t x = h->GetXaxis()->FindBin( (Int_t)EmulsionSizeX/2. );
    Int_t y = h->GetYaxis()->FindBin( (Int_t)EmulsionSizeY/2. );

    c1->cd(3)->SetGrid();
    TH1 *h_px = h->ProjectionX( "h_px", x, x );
    h_px->GetYaxis()->SetRangeUser( 0, h_px->GetMaximum()*1.2 );
    h_px->SetTitle("X Projection");
    h_px->Draw();
    line.DrawLine( 0., 0., 0., 100. );
    line.DrawLine( EmulsionSizeX, 0., EmulsionSizeX, 100. );
    
    c1->cd(4)->SetGrid();
    TH1 *h_py = h->ProjectionY( "h_py", y, y );
    h_py->GetYaxis()->SetRangeUser( 0, h_py->GetMaximum()*1.2 );
    h_py->SetTitle("Y Projection");
    h_py->Draw();
    line.DrawLine( 0., 0., 0., 100. );
    line.DrawLine( EmulsionSizeY, 0., EmulsionSizeY, 100. );
  }


  // {
  //   c1->cd(3)->SetGrid();
  //   TH1F *h = (TH1F*)f->Get("h10");
  //   Int_t beam = h->GetBinCenter(h->GetMaximumBin());
  //   Int_t xmin = beam - beam * 0.4;
  //   Int_t xmax = beam + beam * 0.6;
  //   h->GetXaxis()->SetRangeUser( xmin, xmax );
  //   h->GetXaxis()->SetTitle("Counts/Spill");
  //   h->Draw();
  // }



  c1->Print("showDist.pdf");

}
