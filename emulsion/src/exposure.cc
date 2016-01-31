/**
 *  file: exposure.cc
 *  date: 2016.01.06
 *
 */

#include "exposure.hh"

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <libgen.h>
#include <signal.h>

#include <TFile.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TString.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>

static Event event;

enum e_argc
  {
    k_process,
    k_root_file_name,
    k_argc
  };

bool user_stop = false;
//_____________________________________________________________________
void
catch_signal(int sig)
{
  user_stop = true;
  std::cout<<"\x1b[2D\x1b[1m"
	   << "#D " << __func__ << "() catch signal " << sig
	   << " ... exit process   \x1b[0m" << std::endl;
}

//_____________________________________________________________________
int
main(int argc, char **argv)
{
  std::string process = basename(argv[k_process]);
    
  if(argc!=k_argc){
    std::cerr << "#D Usage: " << process << " [root_file]" << std::endl;
    return EXIT_SUCCESS;
  }

  std::string root_file_name = argv[k_root_file_name];

  ////////// Initialize
  gRandom->SetSeed( time(NULL) );
  TFile *file = new TFile( root_file_name.c_str(), "recreate" );
  TTree *tree = new TTree( "tree", "tree of emulsion" );
  
  tree->Branch("event",   &event.event,   "event/I");
  tree->Branch("spill",   &event.spill,   "spill/I");
  tree->Branch("xcenter", &event.xcenter, "xcenter/D");
  tree->Branch("ycenter", &event.ycenter, "ycenter/D");
  tree->Branch("x",       &event.x,       "x/D");
  tree->Branch("y",       &event.y,       "y/D");
  
  Double_t edge = 0.;
  TH2F *h0  = new TH2F( "h0", "xy dist",
			(int)EmulsionSize[k_x] + edge*2, -edge, EmulsionSize[k_x] +edge,
			(int)EmulsionSize[k_y] + edge*2, -edge, EmulsionSize[k_y] +edge);
  TH1F *h1  = new TH1F("h1", "x dist",
		       (int)EmulsionSize[k_x], 0, EmulsionSize[k_x]);
  TH1F *h2  = new TH1F("h2", "y dist",
		       (int)EmulsionSize[k_y], 0, EmulsionSize[k_y]);
  TH1F *h10 = new TH1F("h10", "beam intensity",
		       (int)BeamIntensity,  0, BeamIntensity*2);

  double BeamCenter[k_xy] = { BeamMargin[k_x], BeamMargin[k_y] };
  unsigned long long ievent = 0;
  int spill = 0;
  ////////// Event Loop Start
  std::cout<<"#D "<<__func__<<"() event loop start"<<std::endl;
  signal( SIGINT, catch_signal );
  while( !user_stop ){
    
    int beam = gRandom->Gaus( BeamIntensity, fluctuation );
    h10->Fill( beam );
    
    if( !emulsion_move( BeamCenter ) ) break;
    spill++;

    for(int i=0; i<beam && !user_stop; i++){
      initialize_event();

      event.event = ievent++;
      event.spill = spill;
    
      double xcenter = BeamCenter[k_x];
      double ycenter = BeamCenter[k_y];
      double x = gRandom->Gaus(BeamCenter[k_x], BeamSigma[k_x]);
      double y = gRandom->Gaus(BeamCenter[k_y], BeamSigma[k_y]);
      event.xcenter = xcenter;
      event.ycenter = ycenter;
      event.x = x;
      event.y = y;
      h0->Fill( x, y );
      h1->Fill( x );
      h2->Fill( y );
      
      tree->Fill();
      ievent++;
    }

    if(spill%1==0){
      dump_count();
      std::cout<<"\x1b[1A"<<"\x1b[1A"<<"\x1b[1A"<<"\x1b[1A";
    }
  }
  
  dump_count();
  
  file->Write();
  file->Close();

  return EXIT_SUCCESS;
}

//_____________________________________________________________________
bool
initialize_event()
{
  event.event   = 0;
  event.spill   = -1;
  event.xcenter = -999.;
  event.ycenter = -999.;
  event.x       = -999.;
  event.y       = -999.;

  return true;
}

//_____________________________________________________________________
bool
emulsion_move(double *beam_center)
{
  do {
    ////////// move x
    beam_center[k_x] += MoverStep[k_x];
    
    ////////// move y
    if( beam_center[k_x]>EmulsionSize[k_x]-BeamMargin[k_x] ){
      beam_center[k_x]  = BeamMargin[k_x];
      beam_center[k_y] += MoverStep[k_y];
    }
  } while( ( beam_center[k_x] < CornerMargin[k_x] &&
	     beam_center[k_y] < CornerMargin[k_y] ) |
	   ( EmulsionSize[k_x]-CornerMargin[k_x] < beam_center[k_x] &&
	     beam_center[k_y] < CornerMargin[k_y] ) |
	   ( beam_center[k_x] < CornerMargin[k_x] &&
	     EmulsionSize[k_y]-CornerMargin[k_y] < beam_center[k_y] ) |
	   ( EmulsionSize[k_x]-CornerMargin[k_x] < beam_center[k_x] &&
	     EmulsionSize[k_y]-CornerMargin[k_y] < beam_center[k_y] ) );

  if( beam_center[k_y]>EmulsionSize[k_y]-BeamMargin[k_y] ) return false;

  return true;
}

//_____________________________________________________________________
bool
dump_count()
{
  std::cout<<"#D "<<__func__<<"()          "<<std::endl
	   <<"  Spill Number: "<<std::setw(8)<<event.spill<<std::endl
	   <<"  Event Number: "<<std::setw(8)<<event.event<<std::endl
	   <<std::fixed
	   <<"   ("<<std::setw(6)<<std::setprecision(2)<<event.xcenter
	   <<", "<<std::setw(6)<<std::setprecision(2)<<event.ycenter
	   <<")"<<std::endl;
  
  return true;
}

