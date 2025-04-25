#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "run.hh"

class MySensentiveDetector : public G4VSensitiveDetector{
public:
	MySensentiveDetector(G4String);
	~MySensentiveDetector();
};

#endif