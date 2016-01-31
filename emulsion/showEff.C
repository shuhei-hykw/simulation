/**
 *  file: showEff.C
 *  date: 2015.07.14
 *
 */

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <TAxis.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TROOT.h>
#include <TStyle.h>

void showEff()
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

  TCanvas *c1 = new TCanvas("c1", "c1", 1200, 800);
  c1->SetGrid();

  std::string ifs_name = "eff.txt";
  std::ifstream ifs(ifs_name.c_str());
  if( ifs.fail() ){
    std::cerr<<"#E showEff() cannot open file: "<<ifs_name<<std::endl;
    std::exit(-1);
  }
  
  std::vector<Double_t> margin;
  std::vector<Double_t> spill;
  std::vector<Double_t> beam_eff;
  std::vector<Double_t> emul_eff;

  std::string line;
  while( ifs.good() && std::getline( ifs, line ) ){
    if( line.empty() || line[0]=='#' ) continue;
    std::istringstream input_line(line);
    std::istream_iterator<std::string> line_begin(input_line);
    std::istream_iterator<std::string> line_end;
    std::vector<std::string> eff_list(line_begin, line_end);
    if( eff_list.size()!=4 ) continue;
    margin.push_back( std::strtod( eff_list[0].c_str(), NULL ) );
    spill.push_back( std::strtod( eff_list[1].c_str(), NULL ) );
    beam_eff.push_back( std::strtod( eff_list[2].c_str(), NULL ) );
    emul_eff.push_back( std::strtod( eff_list[3].c_str(), NULL ) );
  }

  TGraph *g1 = new TGraph( margin.size(), &margin[0], &beam_eff[0] );
  g1->SetTitle("");
  g1->SetMarkerColor(kRed); g1->SetMarkerStyle(8); g1->SetMarkerSize(3);
  g1->GetXaxis()->SetLimits(-0.5, 2.5);
  g1->GetYaxis()->SetRangeUser(0.8, 1.02);
  g1->GetXaxis()->SetTitle("margin [#sigma]");
  g1->GetYaxis()->SetTitle("Efficiency");
  g1->Draw("AP");

  TGraph *g2 = new TGraph( margin.size(), &margin[0], &emul_eff[0] );
  g2->SetMarkerColor(kGreen); g2->SetMarkerStyle(8); g2->SetMarkerSize(3);
  g2->Draw("P");

  // TGraph *g1 = new TGraph( margin.size(), &margin[0], &spill[0] );
  // g1->SetTitle("graph");
  // g1->SetMarkerStyle(8); g1->SetMarkerSize(.5);
  // // g1->GetXaxis()->SetLimits(-20, 360);
  // g1->GetYaxis()->SetRangeUser(0, 4000);
  // g1->Draw("AP");

  c1->Update();
  // TPaletteAxis *p = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");
  // p->SetX2NDC(0.85); p->SetY2NDC(0.62);

  c1->Update();

  c1->Print("showEff.pdf");

}
