#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "construction.hh"
#include "event.hh"

#include "G4VParticleChange.hh"
#include "G4SteppingControl.hh"

class MySteppingAction : public G4UserSteppingAction
{
public:
	MySteppingAction(MyEventAction* eventAction);
	~MySteppingAction();

	virtual void UserSteppingAction(const G4Step*);
	void SaveToDataFile(const G4Step* step);
	void ReadOut(const G4Step* step);

private:
	MyEventAction* fEventAction;
	G4int current_track = 0;
};

#endif