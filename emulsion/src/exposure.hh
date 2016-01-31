/**
 *  file: exposure.hh
 *  date: 2016.01.06
 *
 */

#ifndef EXPOSURE_HH
#define EXPOSURE_HH

enum e_particle { k_pi, k_kaon, k_particle };
static const int BeamIntensity = 3500; // [/spill]
static const int fluctuation   = BeamIntensity*0.02;

enum e_xy { k_x, k_y, k_xy };
static const double EmulsionSize[k_xy] = { 335.0, 340.0 }; // [mm]
static const double BeamSigma[k_xy]    = {  11.0,   4.4 }; // [mm]
static const double MoverStep[k_xy]    = { 335.0/36.5,  340.0/90.5 }; // [mm]
//static const double BeamMargin[k_xy]   = {   0.0,   0.0 }; // [mm]
static const double factor = 2.0;
static const double BeamMargin[k_xy]   = { BeamSigma[k_x] * factor,
					   BeamSigma[k_y] * factor }; // [mm]

//static const double CornerMargin[k_xy] = {  35.0,  20.0 }; // [mm]
static const double CornerMargin[k_xy] = {  38.0,  25.0 }; // [mm]
//static const double CornerMargin[k_xy] = {  50.0,  35.0 }; // [mm]

struct Event
{
  unsigned long long event;
  int spill;
  double xcenter;
  double ycenter;
  double x;
  double y;
};

bool initialize_event();
bool emulsion_move(double *beam_center);
bool dump_count();

#endif
