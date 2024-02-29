#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "run.hh"
#include "event.hh"
#include "stepping.hh"

class MyActionInitialization : public G4VUserActionInitialization
{
public:
	MyActionInitialization();
	~MyActionInitialization();

	// The Build function runs the particle guns, stepping computations, etc.
	// Build() for worker threads, BuildForMaster() for master threads
	virtual void Build() const;
	virtual void BuildForMaster() const;
};

#endif